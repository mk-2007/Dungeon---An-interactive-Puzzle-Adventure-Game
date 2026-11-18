#include "BridgeBuilder.h"

// ---------- Constructor ----------

BridgeBuilder::BridgeBuilder()
{
	siteCount = 0;
	switchCount = 0;
	islandCount = 0;
	startIsland = -1;
	goalIsland = -1;
	moves = 0;
	maxMoves = 0;
	level = Difficulty::easy;
	gameStatus = Status::notStarted;
	lastResultValue = Result::notStarted;

	for (int i = 0; i < maxSwitches; i++)
	{
		switchOn[i] = false;
	}
}

// ---------- Level setup ----------

bool BridgeBuilder::startLevel(int islands, int start, int goal,
	int maxMovesAllowed, Difficulty difficultyLevel)
{
	// Reject nonsense settings
	if (islands < 2 || start < 0 || start >= islands ||
		goal < 0 || goal >= islands || start == goal ||
		maxMovesAllowed < 1)
	{
		lastResultValue = Result::invalidIsland;
		return false;
	}

	// Only allowed once per object: the Graph cannot be rebuilt or copied
	if (graph.vertexCount() != 0)
	{
		lastResultValue = Result::gameOver;
		return false;
	}

	// Create one Graph vertex for every island
	for (int i = 0; i < islands; i++)
	{
		graph.addVertex();
	}

	islandCount = islands;
	startIsland = start;
	goalIsland = goal;
	maxMoves = maxMovesAllowed;
	level = difficultyLevel;
	moves = 0;
	siteCount = 0;
	switchCount = 0;

	for (int i = 0; i < maxSwitches; i++)
	{
		switchOn[i] = false;
	}

	gameStatus = Status::playing;
	lastResultValue = Result::ok;
	return true;
}

int BridgeBuilder::addSwitch()
{
	if (gameStatus != Status::playing || switchCount >= maxSwitches)
	{
		return -1;
	}

	int id = switchCount;
	switchOn[id] = false;
	switchCount++;
	return id;
}

bool BridgeBuilder::addBridgeSite(int a, int b, int switchId)
{
	if (gameStatus != Status::playing)
	{
		lastResultValue = Result::notStarted;
		return false;
	}
	if (!isValidIsland(a) || !isValidIsland(b) || a == b)
	{
		lastResultValue = Result::invalidIsland;
		return false;
	}
	if (switchId < -1 || switchId >= switchCount)
	{
		lastResultValue = Result::invalidSwitch;
		return false;
	}
	if (siteCount >= maxBridges || findSite(a, b) != -1)
	{
		lastResultValue = Result::alreadyBuilt;
		return false;
	}

	sites[siteCount].a = a;
	sites[siteCount].b = b;
	sites[siteCount].switchId = switchId;
	sites[siteCount].blocked = (switchId != -1);
	sites[siteCount].built = false;
	siteCount++;

	lastResultValue = Result::ok;
	return true;
}

// ---------- Player actions ----------

bool BridgeBuilder::buildBridge(int a, int b)
{
	Result why = Result::ok;
	if (!canAct(why))
	{
		lastResultValue = why;
		return false;
	}
	if (!isValidIsland(a) || !isValidIsland(b) || a == b)
	{
		lastResultValue = Result::invalidIsland;
		return false;
	}

	int index = findSite(a, b);
	if (index == -1)
	{
		lastResultValue = Result::noBridgeSite;
		return false;
	}
	if (sites[index].blocked)
	{
		lastResultValue = Result::bridgeBlocked;
		return false;
	}
	if (sites[index].built)
	{
		lastResultValue = Result::alreadyBuilt;
		return false;
	}

	// The move is valid: change the Graph, then count the move
	graph.addEdge(sites[index].a, sites[index].b);
	sites[index].built = true;
	moves++;
	lastResultValue = Result::ok;
	finishMove();
	return true;
}

bool BridgeBuilder::removeBridge(int a, int b)
{
	Result why = Result::ok;
	if (!canAct(why))
	{
		lastResultValue = why;
		return false;
	}
	if (!isValidIsland(a) || !isValidIsland(b) || a == b)
	{
		lastResultValue = Result::invalidIsland;
		return false;
	}

	int index = findSite(a, b);
	if (index == -1)
	{
		lastResultValue = Result::noBridgeSite;
		return false;
	}
	if (!sites[index].built)
	{
		lastResultValue = Result::notBuilt;
		return false;
	}

	graph.removeEdge(sites[index].a, sites[index].b);
	sites[index].built = false;
	moves++;
	lastResultValue = Result::ok;
	finishMove();
	return true;
}

bool BridgeBuilder::activateSwitch(int switchId)
{
	Result why = Result::ok;
	if (!canAct(why))
	{
		lastResultValue = why;
		return false;
	}
	if (switchId < 0 || switchId >= switchCount)
	{
		lastResultValue = Result::invalidSwitch;
		return false;
	}
	if (switchOn[switchId])
	{
		lastResultValue = Result::switchAlreadyOn;
		return false;
	}

	// Unblock every bridge that this switch controls
	switchOn[switchId] = true;
	for (int i = 0; i < siteCount; i++)
	{
		if (sites[i].switchId == switchId)
		{
			sites[i].blocked = false;
		}
	}

	moves++;
	lastResultValue = Result::ok;
	finishMove();
	return true;
}

// ---------- Route checking (BFS on the Graph) ----------

bool BridgeBuilder::hasRoute() const
{
	if (gameStatus == Status::notStarted)
	{
		return false;
	}

	bool* visited = new bool[islandCount];
	for (int i = 0; i < islandCount; i++)
	{
		visited[i] = false;
	}

	// Capacity = island count, so the queue can never overflow
	Queue<int> queue(islandCount);
	visited[startIsland] = true;
	queue.enqueue(startIsland);

	bool found = false;
	int current = 0;

	while (!found && queue.dequeue(current))
	{
		if (current == goalIsland)
		{
			found = true;
		}
		else
		{
			GraphNode* node = graph.neighbors(current);
			while (node != nullptr)
			{
				if (!visited[node->vertex])
				{
					visited[node->vertex] = true;
					queue.enqueue(node->vertex);
				}
				node = node->next;
			}
		}
	}

	delete[] visited;
	return found;
}

// ---------- Private helpers ----------

bool BridgeBuilder::isValidIsland(int island) const
{
	return island >= 0 && island < islandCount;
}

// Returns the index of the bridge site joining a and b (in either order),
// or -1 if there is no such site.
int BridgeBuilder::findSite(int a, int b) const
{
	for (int i = 0; i < siteCount; i++)
	{
		if ((sites[i].a == a && sites[i].b == b) ||
			(sites[i].a == b && sites[i].b == a))
		{
			return i;
		}
	}
	return -1;
}

// Decides whether the player may act right now.
// If not, 'why' says the reason.
bool BridgeBuilder::canAct(Result& why) const
{
	if (gameStatus == Status::notStarted)
	{
		why = Result::notStarted;
		return false;
	}
	if (gameStatus == Status::won || gameStatus == Status::lost)
	{
		why = Result::gameOver;
		return false;
	}
	if (moves >= maxMoves)
	{
		why = Result::noMovesLeft;
		return false;
	}
	return true;
}

// Called after every accepted move (moves has already been increased).
void BridgeBuilder::finishMove()
{
	if (hasRoute())
	{
		gameStatus = Status::won;
	}
	else if (moves >= maxMoves)
	{
		gameStatus = Status::lost;
	}
}
// ---------- Reset ----------

// Puts the level back to its starting state without rebuilding the Graph.
void BridgeBuilder::reset()
{
	if (gameStatus == Status::notStarted)
	{
		lastResultValue = Result::notStarted;
		return;
	}

	// Remove every built bridge from the Graph and re-lock switch bridges
	for (int i = 0; i < siteCount; i++)
	{
		if (sites[i].built)
		{
			graph.removeEdge(sites[i].a, sites[i].b);
			sites[i].built = false;
		}
		sites[i].blocked = (sites[i].switchId != -1);
	}

	for (int i = 0; i < switchCount; i++)
	{
		switchOn[i] = false;
	}

	moves = 0;
	gameStatus = Status::playing;
	lastResultValue = Result::ok;
}

// ---------- Read-only getters ----------

BridgeBuilder::Status BridgeBuilder::status() const
{
	return gameStatus;
}

BridgeBuilder::Result BridgeBuilder::lastResult() const
{
	return lastResultValue;
}

BridgeBuilder::Difficulty BridgeBuilder::difficulty() const
{
	return level;
}

bool BridgeBuilder::isComplete() const
{
	return gameStatus == Status::won || gameStatus == Status::lost;
}

int BridgeBuilder::moveCount() const
{
	return moves;
}

int BridgeBuilder::maxMoveCount() const
{
	return maxMoves;
}

int BridgeBuilder::movesLeft() const
{
	return maxMoves - moves;
}

int BridgeBuilder::islandTotal() const
{
	return islandCount;
}

int BridgeBuilder::startIslandId() const
{
	return startIsland;
}

int BridgeBuilder::goalIslandId() const
{
	return goalIsland;
}

bool BridgeBuilder::isBridgeBuilt(int a, int b) const
{
	int index = findSite(a, b);
	return index != -1 && sites[index].built;
}

bool BridgeBuilder::isBridgeBlocked(int a, int b) const
{
	int index = findSite(a, b);
	return index != -1 && sites[index].blocked;
}

bool BridgeBuilder::isSwitchOn(int switchId) const
{
	if (switchId < 0 || switchId >= switchCount)
	{
		return false;
	}
	return switchOn[switchId];
}
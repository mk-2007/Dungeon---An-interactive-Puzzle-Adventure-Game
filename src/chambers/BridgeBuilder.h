#pragma once

#include "../ds/Graph.h"
#include "../ds/Queue.h"

class BridgeBuilder
{
public:
	enum class Difficulty
	{
		easy,
		hard
	};

	enum class Status
	{
		notStarted,
		playing,
		won,
		lost
	};

	// Why the last action succeeded or failed (used for invalid-move feedback)
	enum class Result
	{
		ok,
		notStarted,
		gameOver,
		invalidIsland,
		invalidSwitch,
		noBridgeSite,
		bridgeBlocked,
		alreadyBuilt,
		notBuilt,
		switchAlreadyOn,
		noMovesLeft
	};

	BridgeBuilder();

	// Level setup
	bool startLevel(int islands, int start, int goal, int maxMoves,
		Difficulty level);
	int addSwitch();
	bool addBridgeSite(int a, int b, int switchId = -1);

	// Player actions (each returns true if the move was accepted)
	bool buildBridge(int a, int b);
	bool removeBridge(int a, int b);
	bool activateSwitch(int switchId);
	void reset();

	// Read-only information (safe for Draw() later)
	Status status() const;
	Result lastResult() const;
	Difficulty difficulty() const;
	bool isComplete() const;
	bool hasRoute() const;
	int moveCount() const;
	int maxMoveCount() const;
	int movesLeft() const;
	int islandTotal() const;
	int startIslandId() const;
	int goalIslandId() const;
	bool isBridgeBuilt(int a, int b) const;
	bool isBridgeBlocked(int a, int b) const;
	bool isSwitchOn(int switchId) const;

private:
	static const int maxBridges = 12;
	static const int maxSwitches = 4;

	struct BridgeSite
	{
		int a;
		int b;
		int switchId;   // -1 = never blocked
		bool blocked;   // true = cannot be built yet
		bool built;     // true = currently an edge in the Graph
	};

	Graph graph;                      // only built, usable bridges
	BridgeSite sites[maxBridges];     // every bridge that can exist
	bool switchOn[maxSwitches];
	int siteCount;
	int switchCount;

	int islandCount;
	int startIsland;
	int goalIsland;
	int moves;
	int maxMoves;
	Difficulty level;
	Status gameStatus;
	Result lastResultValue;

	int findSite(int a, int b) const;
	bool isValidIsland(int island) const;
	bool canAct(Result& why) const;
	void finishMove();
};
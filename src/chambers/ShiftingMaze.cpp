#include "ShiftingMaze.h"

ShiftingMaze::ShiftingMaze()
	: current(nullptr),
	  gameStatus(Status::notStarted),
	  targetKey(0),
	  misplacedValue(0),
	  correctedValue(0),
	  moves(0),
	  incorrectMoves(0),
	  targetFound(false),
	  nodeRepaired(false)
{
}

bool ShiftingMaze::startLevel(const int values[], int count, int target,
	int misplaced, int corrected)
{
	if (values == nullptr || count <= 0 || target == misplaced ||
		misplaced == corrected)
	{
		return false;
	}

	bool hasTarget = false;
	bool hasMisplacedValue = false;
	bool hasCorrectedValue = false;

	for (int i = 0; i < count; i++)
	{
		if (values[i] == target)
		{
			hasTarget = true;
		}
		if (values[i] == misplaced)
		{
			hasMisplacedValue = true;
		}
		if (values[i] == corrected)
		{
			hasCorrectedValue = true;
		}

		for (int j = 0; j < i; j++)
		{
			if (values[i] == values[j])
			{
				return false;
			}
		}
	}

	if (!hasTarget || !hasMisplacedValue || hasCorrectedValue)
	{
		return false;
	}

	tree.clear();
	for (int i = 0; i < count; i++)
	{
		tree.insert(values[i]);
	}

	targetKey = target;
	misplacedValue = misplaced;
	correctedValue = corrected;
	current = tree.root();
	gameStatus = Status::playing;
	moves = 0;
	incorrectMoves = 0;
	targetFound = current != nullptr && current->data == targetKey;
	nodeRepaired = false;
	return true;
}

bool ShiftingMaze::chooseDirection(Direction direction)
{
	if (gameStatus != Status::playing || targetFound || current == nullptr)
	{
		return false;
	}

	++moves;
	const bool shouldGoLeft = targetKey < current->data;
	const BST::Node* next = nullptr;

	if (direction == Direction::left && shouldGoLeft)
	{
		next = current->left;
	}
	else if (direction == Direction::right && !shouldGoLeft)
	{
		next = current->right;
	}

	if (next == nullptr)
	{
		return recordIncorrectMove();
	}

	current = next;
	if (current->data == targetKey)
	{
		targetFound = true;
		updateStatus();
	}

	return true;
}

bool ShiftingMaze::repairMisplacedNode(int valueToReplace, int replacementValue)
{
	if (gameStatus != Status::playing || nodeRepaired)
	{
		return false;
	}

	++moves;
	if (valueToReplace != misplacedValue || replacementValue != correctedValue)
	{
		return recordIncorrectMove();
	}

	if (!tree.remove(misplacedValue))
	{
		return recordIncorrectMove();
	}

	tree.insert(correctedValue);
	nodeRepaired = true;
	current = tree.root();
	updateStatus();
	return true;
}

ShiftingMaze::Status ShiftingMaze::status() const
{
	return gameStatus;
}

bool ShiftingMaze::isComplete() const
{
	return gameStatus == Status::won || gameStatus == Status::lost;
}

bool ShiftingMaze::hasRepairedNode() const
{
	return nodeRepaired;
}

int ShiftingMaze::moveCount() const
{
	return moves;
}

int ShiftingMaze::incorrectMoveCount() const
{
	return incorrectMoves;
}

const BST::Node* ShiftingMaze::currentNode() const
{
	return current;
}

bool ShiftingMaze::containsKey(int value) const
{
	return tree.contains(value);
}

bool ShiftingMaze::recordIncorrectMove()
{
	++incorrectMoves;
	if (incorrectMoves >= maxIncorrectMoves)
	{
		gameStatus = Status::lost;
	}
	return false;
}

void ShiftingMaze::updateStatus()
{
	if (targetFound && nodeRepaired)
	{
		gameStatus = Status::won;
	}
}

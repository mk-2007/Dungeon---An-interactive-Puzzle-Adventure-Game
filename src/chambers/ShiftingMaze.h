#pragma once

#include "../ds/BST.h"

class ShiftingMaze
{
public:
	enum class Direction
	{
		left,
		right
	};

	enum class Status
	{
		notStarted,
		playing,
		won,
		lost
	};

	ShiftingMaze();

	bool startLevel(const int values[], int count, int targetKey,
		int misplacedValue, int correctedValue);

	bool chooseDirection(Direction direction);
	bool repairMisplacedNode(int valueToReplace, int correctedValue);

	Status status() const;
	bool isComplete() const;
	bool hasRepairedNode() const;
	int moveCount() const;
	int incorrectMoveCount() const;
	const BST::Node* currentNode() const;
	bool containsKey(int value) const;

private:
	static const int maxIncorrectMoves = 3;

	BST tree;
	const BST::Node* current;
	Status gameStatus;
	int targetKey;
	int misplacedValue;
	int correctedValue;
	int moves;
	int incorrectMoves;
	bool targetFound;
	bool nodeRepaired;

	bool recordIncorrectMove();
	void updateStatus();
};

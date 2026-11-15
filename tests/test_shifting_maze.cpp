#include <iostream>
#include <string>

#include "../src/chambers/ShiftingMaze.h"

using namespace std;

bool allPassed = true;

void check(bool condition, const string& name)
{
	if (condition)
	{
		cout << "PASS: " << name << endl;
	}
	else
	{
		cout << "FAIL: " << name << endl;
		allPassed = false;
	}
}

int main()
{
	const int values[] = { 50, 30, 70, 20, 40, 60, 80 };
	const int duplicateValues[] = { 50, 50, 70 };
	ShiftingMaze maze;

	check(maze.status() == ShiftingMaze::Status::notStarted,
		"New maze has not started");
	check(!maze.isComplete(), "New maze is not complete");
	check(!maze.startLevel(nullptr, 1, 50, 30, 35),
		"Null level data is rejected");
	check(!maze.startLevel(values, 0, 60, 30, 35),
		"Empty level data is rejected");
	check(maze.startLevel(values, 7, 60, 30, 35),
		"Valid level starts");
	check(maze.currentNode()->data == 50, "Search starts at the BST root");

	check(!maze.startLevel(duplicateValues, 3, 50, 70, 80),
		"Duplicate level values are rejected");
	check(!maze.startLevel(values, 7, 90, 30, 35),
		"Level without the target is rejected");
	check(!maze.startLevel(values, 7, 60, 30, 70),
		"Level with an existing correction value is rejected");
	check(maze.status() == ShiftingMaze::Status::playing &&
		maze.currentNode()->data == 50,
		"Invalid level does not replace the active level");

	check(!maze.chooseDirection(ShiftingMaze::Direction::left),
		"Wrong branch is rejected");
	check(maze.incorrectMoveCount() == 1 && maze.moveCount() == 1,
		"Wrong branch increments mistake and move counts");
	check(maze.currentNode()->data == 50,
		"Wrong branch leaves the player at the current node");

	check(maze.chooseDirection(ShiftingMaze::Direction::right),
		"Correct branch advances the search");
	check(maze.currentNode()->data == 70,
		"Correct branch follows the BST child");
	check(maze.chooseDirection(ShiftingMaze::Direction::left),
		"Search reaches the target");
	check(maze.currentNode()->data == 60,
		"Current node is the target key");
	check(maze.status() == ShiftingMaze::Status::playing,
		"Finding the key alone does not complete the repair objective");

	check(!maze.repairMisplacedNode(30, 36),
		"Incorrect repair values are rejected");
	check(maze.incorrectMoveCount() == 2,
		"Incorrect repair counts toward the three-mistake limit");
	check(maze.repairMisplacedNode(30, 35),
		"Correct repair removes and reinserts the key");
	check(!maze.containsKey(30) && maze.containsKey(35),
		"Repair updates the BST through its operations");
	check(maze.hasRepairedNode(), "Repair state is recorded");
	check(maze.currentNode()->data == 50,
		"Repair resets the current position to the root");
	check(maze.status() == ShiftingMaze::Status::won && maze.isComplete(),
		"Finding the target and repairing the node wins the maze");
	check(maze.moveCount() == 5 && maze.incorrectMoveCount() == 2,
		"Move and mistake totals include each submitted action");
	check(!maze.repairMisplacedNode(30, 35),
		"Repair cannot be repeated after the maze is complete");

	ShiftingMaze lostMaze;
	check(lostMaze.startLevel(values, 7, 70, 20, 25),
		"Second level starts for loss testing");
	check(!lostMaze.chooseDirection(ShiftingMaze::Direction::left),
		"First wrong choice is counted");
	check(!lostMaze.chooseDirection(ShiftingMaze::Direction::left),
		"Second wrong choice is counted");
	check(!lostMaze.chooseDirection(ShiftingMaze::Direction::left),
		"Third wrong choice is counted");
	check(lostMaze.status() == ShiftingMaze::Status::lost &&
		lostMaze.isComplete(),
		"Three incorrect choices lose the maze");
	check(lostMaze.moveCount() == 3 && lostMaze.incorrectMoveCount() == 3,
		"Loss preserves the final move and mistake counts");
	check(!lostMaze.repairMisplacedNode(20, 25),
		"Moves are rejected after a loss");

	ShiftingMaze rootTargetMaze;
	const int rootTargetValues[] = { 5, 3, 7 };
	check(rootTargetMaze.startLevel(rootTargetValues, 3, 5, 3, 4),
		"Level with the target at the root starts");
	check(rootTargetMaze.repairMisplacedNode(3, 4),
		"Root target can complete after the required repair");
	check(rootTargetMaze.status() == ShiftingMaze::Status::won,
		"Repair wins when the target was already at the root");

	return allPassed ? 0 : 1;
}

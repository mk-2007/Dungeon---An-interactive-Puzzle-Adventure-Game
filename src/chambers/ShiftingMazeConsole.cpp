#include <iostream>
#include <string>
#include "ShiftingMazeConsole.h"
#include "ShiftingMaze.h"

using namespace std;

struct MazeLevel
{
	const char* name;
	const int* values;
	int count;
	int target;
	int wrongValue;
	int fixedValue;
};

const int easyValues[] = { 50, 30, 70, 20, 40, 60, 80 };
const int hardValues[] = { 50, 30, 70, 20, 40, 60, 80, 10, 45, 65, 90 };

const MazeLevel easyLevel = { "EASY", easyValues, 7, 60, 30, 35 };
const MazeLevel hardLevel = { "HARD", hardValues, 11, 45, 70, 75 };

enum LevelResult
{
	levelWon,
	levelLost,
	levelQuit
};

void showIntro(const MazeLevel& level)
{
	cout << endl;
	cout << "=== The Shifting Maze - " << level.name << " ===" << endl;
	cout << "Every room has a number. Smaller numbers are through the LEFT door," << endl;
	cout << "bigger numbers are through the RIGHT door (a binary search tree)." << endl;
	cout << "Goal 1: find the key in room " << level.target << "." << endl;
	cout << "Goal 2: room " << level.wrongValue << " is broken. Replace it with "
		<< level.fixedValue << " (type: repair " << level.wrongValue << " "
		<< level.fixedValue << ")." << endl;
	cout << "Three wrong moves and you lose." << endl;
	cout << "Commands: left, right, repair A B, help, quit" << endl;
}

void showRoom(const ShiftingMaze& maze, const MazeLevel& level)
{
	const BST::Node* room = maze.currentNode();

	cout << endl;
	cout << "You are in room " << room->data << ".  Key room: " << level.target
		<< "   Mistakes: " << maze.incorrectMoveCount() << "/3" << endl;

	cout << "  Left door : ";
	if (room->left == nullptr)
	{
		cout << "wall" << endl;
	}
	else
	{
		cout << "room " << room->left->data << endl;
	}

	cout << "  Right door: ";
	if (room->right == nullptr)
	{
		cout << "wall" << endl;
	}
	else
	{
		cout << "room " << room->right->data << endl;
	}
}

void walk(ShiftingMaze& maze, ShiftingMaze::Direction direction)
{
	if (maze.chooseDirection(direction))
	{
		cout << "You walk through the door." << endl;
	}
	else
	{
		cout << "Wrong way! The key is not in that direction." << endl;
	}
}

void repair(ShiftingMaze& maze)
{
	int oldValue = 0;
	int newValue = 0;

	if (!(cin >> oldValue >> newValue))
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Type two numbers, for example: repair 30 35" << endl;
		return;
	}

	if (maze.repairMisplacedNode(oldValue, newValue))
	{
		cout << "The broken room is repaired." << endl;
	}
	else
	{
		cout << "That repair is wrong or not allowed." << endl;
	}
}

LevelResult playLevel(const MazeLevel& level)
{
	ShiftingMaze maze;

	if (!maze.startLevel(level.values, level.count, level.target,
		level.wrongValue, level.fixedValue))
	{
		cout << "This level could not be loaded." << endl;
		return levelQuit;
	}

	showIntro(level);

	while (!maze.isComplete())
	{
		showRoom(maze, level);
		cout << "> ";

		string command;
		if (!(cin >> command) || command == "quit")
		{
			return levelQuit;
		}

		if (command == "left")
		{
			walk(maze, ShiftingMaze::Direction::left);
		}
		else if (command == "right")
		{
			walk(maze, ShiftingMaze::Direction::right);
		}
		else if (command == "repair")
		{
			repair(maze);
		}
		else
		{
			showIntro(level);
		}

		if (maze.currentNode()->data == level.target && !maze.isComplete())
		{
			cout << "You are standing in the key room!" << endl;
		}
	}

	if (maze.status() == ShiftingMaze::Status::won)
	{
		cout << endl << "You found the key and repaired the maze in "
			<< maze.moveCount() << " moves!" << endl;
		return levelWon;
	}

	cout << endl << "Too many wrong moves. The maze shifts and you are lost." << endl;
	return levelLost;
}

bool playShiftingMaze()
{
	const MazeLevel* levels[2] = { &easyLevel, &hardLevel };

	for (int i = 0; i < 2; i++)
	{
		LevelResult result = playLevel(*levels[i]);

		while (result == levelLost)
		{
			cout << "Try this level again? (yes/no): ";

			string answer;
			if (!(cin >> answer) || answer != "yes")
			{
				return false;
			}
			result = playLevel(*levels[i]);
		}

		if (result == levelQuit)
		{
			return false;
		}
	}

	cout << endl << "You solved The Shifting Maze!" << endl;
	return true;
}

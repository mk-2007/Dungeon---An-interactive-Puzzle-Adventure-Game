#include <iostream>
#include "Hub.h"
#include "../chambers/ShiftingMazeConsole.h"

using namespace std;

void Hub::run()
{
	bool running = true;

	while (running)
	{
		cout << endl;
		cout << "===== DUNGEON: A DATA STRUCTURE ADVENTURE =====" << endl;
		cout << "  1. Story mode (open the chambers in order)" << endl;
		cout << "  2. Practice mode (play any chamber)" << endl;
		cout << "  0. Exit" << endl;
		cout << "Your choice: ";

		int choice = readChoice();
		if (choice == 1)
		{
			runStory();
		}
		else if (choice == 2)
		{
			runPractice();
		}
		else if (choice == 0)
		{
			running = false;
		}
		else
		{
			cout << "Please type 1, 2 or 0." << endl;
		}
	}

	cout << "Goodbye!" << endl;
}

bool Hub::openByName(const string& name)
{
	int index = -1;

	if (name == "stack")
	{
		index = stackVault;
	}
	else if (name == "queue")
	{
		index = forgottenQueue;
	}
	else if (name == "tree")
	{
		index = shiftingMaze;
	}
	else if (name == "graph")
	{
		index = bridgeBuilder;
	}

	if (index == -1)
	{
		return false;
	}

	openChamber(index, true);
	return true;
}

void Hub::runStory()
{
	bool inHub = true;

	while (inHub)
	{
		showDoors(false);
		cout << "Which door? ";

		int choice = readChoice();
		if (choice == 0)
		{
			inHub = false;
		}
		else if (choice >= 1 && choice <= GameProgress::chamberCount)
		{
			openChamber(choice - 1, false);
		}
		else
		{
			cout << "Please type a door number." << endl;
		}

		if (progress.allCompleted())
		{
			cout << endl;
			cout << "*** You solved all four chambers and escaped the dungeon! ***"
				<< endl;
			inHub = false;
		}
	}
}

void Hub::runPractice()
{
	bool inHub = true;

	while (inHub)
	{
		showDoors(true);
		cout << "Which door? ";

		int choice = readChoice();
		if (choice == 0)
		{
			inHub = false;
		}
		else if (choice >= 1 && choice <= GameProgress::chamberCount)
		{
			openChamber(choice - 1, true);
		}
		else
		{
			cout << "Please type a door number." << endl;
		}
	}
}

void Hub::showDoors(bool practice) const
{
	cout << endl;
	cout << "===== THE HUB =====" << endl;
	for (int i = 0; i < GameProgress::chamberCount; i++)
	{
		cout << "  " << i + 1 << ". " << chamberName(i)
			<< " (" << chamberTopic(i) << ")  "
			<< doorStatus(i, practice) << endl;
	}
	cout << "  0. Back" << endl;
}

void Hub::openChamber(int index, bool practice)
{
	if (!isReady(index))
	{
		cout << "This chamber is not ready yet." << endl;
		return;
	}

	if (!practice && !progress.isUnlocked(index))
	{
		cout << "This door is locked. Finish the previous chamber first."
			<< endl;
		return;
	}

	bool won = playChamber(index);

	// Practice mode never changes the story progress.
	if (won && !practice)
	{
		progress.completeChamber(index);
		cout << "Chamber complete! The next door is now open." << endl;
	}
}

const char* Hub::doorStatus(int index, bool practice) const
{
	if (!isReady(index))
	{
		return "[not ready]";
	}
	if (!practice && progress.isCompleted(index))
	{
		return "[done]";
	}
	if (practice || progress.isUnlocked(index))
	{
		return "[open]";
	}
	return "[locked]";
}

const char* Hub::chamberName(int index)
{
	switch (index)
	{
	case stackVault:
		return "Stack Vault";
	case forgottenQueue:
		return "Forgotten Queue";
	case shiftingMaze:
		return "The Shifting Maze";
	case bridgeBuilder:
		return "The Bridge Builder";
	}
	return "Unknown";
}

const char* Hub::chamberTopic(int index)
{
	switch (index)
	{
	case stackVault:
		return "Stack";
	case forgottenQueue:
		return "Queue";
	case shiftingMaze:
		return "Binary Search Tree";
	case bridgeBuilder:
		return "Graph";
	}
	return "";
}

// A chamber is ready when its play function is connected in playChamber.
bool Hub::isReady(int index)
{
	switch (index)
	{
	case shiftingMaze:
		return true;
	}
	return false;
}

// Starts a chamber and returns true when the player wins it.
// When a teammate finishes a chamber, add their include at the top of this
// file, call their function here, and add the chamber to isReady above.
bool Hub::playChamber(int index)
{
	switch (index)
	{
	case shiftingMaze:
		return playShiftingMaze();
	}
	return false;
}

int Hub::readChoice()
{
	string text;
	if (!(cin >> text))
	{
		return 0;
	}

	if (text.size() == 1 && text[0] >= '0' && text[0] <= '9')
	{
		return text[0] - '0';
	}
	return -1;
}

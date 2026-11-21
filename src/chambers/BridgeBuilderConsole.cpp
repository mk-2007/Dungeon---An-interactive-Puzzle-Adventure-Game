#include <iostream>
#include <string>
#include "BridgeBuilderConsole.h"
#include "BridgeBuilder.h"

using namespace std;

// Everything below except playBridgeBuilder() is private to this file,
// so its names can never clash with other chambers at link time.
namespace
{
	typedef BridgeBuilder BB;

	// One bridge that can exist in a level: islands a and b,
	// and the switch that unlocks it (-1 = never blocked).
	struct SiteDef
	{
		int a;
		int b;
		int sw;
	};

	// Everything needed to describe one level.
	struct LevelDef
	{
		const char* name;
		BB::Difficulty difficulty;
		int islands;
		int start;
		int goal;
		int maxMoves;
		int switches;
		const SiteDef* sites;
		int siteCount;
	};

	// EASY: 5 islands, start 0, goal 4, 6 moves.
	// Shortest route is 0-1-3-4 (3 moves). The switch gives a second route.
	const SiteDef easySites[] = {
		{ 0, 1, -1 },
		{ 1, 2, -1 },
		{ 2, 4,  0 },
		{ 1, 3, -1 },
		{ 3, 4, -1 }
	};

	// HARD: 7 islands, start 0, goal 6, only 5 moves.
	// The goal can only be reached through a locked bridge, so a switch is
	// needed. Best solution: switch 1, then 0-3, 3-4, 4-6 (4 moves).
	const SiteDef hardSites[] = {
		{ 0, 1, -1 },
		{ 1, 2, -1 },
		{ 2, 5, -1 },
		{ 5, 6,  0 },
		{ 0, 3, -1 },
		{ 3, 4, -1 },
		{ 4, 6,  1 },
		{ 1, 3, -1 }
	};

	const LevelDef easyLevel = { "EASY", BB::Difficulty::easy, 5, 0, 4, 6, 1, easySites, 5 };
	const LevelDef hardLevel = { "HARD", BB::Difficulty::hard, 7, 0, 6, 5, 2, hardSites, 8 };

	bool inputEnded = false;

	// Creates the islands, switches and bridge sites of a level.
	bool loadLevel(BB& game, const LevelDef& level)
	{
		if (!game.startLevel(level.islands, level.start, level.goal,
			level.maxMoves, level.difficulty))
		{
			return false;
		}
		for (int i = 0; i < level.switches; i++)
		{
			game.addSwitch();
		}
		for (int i = 0; i < level.siteCount; i++)
		{
			if (!game.addBridgeSite(level.sites[i].a, level.sites[i].b, level.sites[i].sw))
			{
				return false;
			}
		}
		return true;
	}

	// Turns a Result code into a sentence for the player.
	const char* explain(BB::Result r)
	{
		switch (r)
		{
		case BB::Result::ok:              return "OK";
		case BB::Result::notStarted:      return "The level has not started.";
		case BB::Result::gameOver:        return "The game is over. Type 'reset' or 'quit'.";
		case BB::Result::invalidIsland:   return "That island does not exist.";
		case BB::Result::invalidSwitch:   return "That switch does not exist.";
		case BB::Result::noBridgeSite:    return "No bridge can be built between those islands.";
		case BB::Result::bridgeBlocked:   return "That bridge is blocked. Activate its switch first.";
		case BB::Result::alreadyBuilt:    return "That bridge is already built.";
		case BB::Result::notBuilt:        return "That bridge is not built yet.";
		case BB::Result::switchAlreadyOn: return "That switch is already on.";
		case BB::Result::noMovesLeft:     return "No moves left.";
		}
		return "Unknown problem.";
	}

	// Draws the board as text. It only READS the game (nothing is changed here).
	void showBoard(const BB& game, const LevelDef& level)
	{
		cout << endl;
		cout << "=== Bridge Builder - " << level.name << " ===" << endl;
		cout << "Islands 0 to " << game.islandTotal() - 1
			<< "   START = " << game.startIslandId()
			<< "   GOAL = " << game.goalIslandId() << endl;
		cout << "Moves: " << game.moveCount() << "/" << game.maxMoveCount()
			<< " (" << game.movesLeft() << " left)   Route: "
			<< (game.hasRoute() ? "YES" : "no") << endl;

		cout << "Bridges:" << endl;
		for (int i = 0; i < level.siteCount; i++)
		{
			int a = level.sites[i].a;
			int b = level.sites[i].b;
			cout << "  " << a << " - " << b << "  ";
			if (game.isBridgeBuilt(a, b))
			{
				cout << "BUILT";
			}
			else if (game.isBridgeBlocked(a, b))
			{
				cout << "BLOCKED (needs switch " << level.sites[i].sw << ")";
			}
			else
			{
				cout << "free (you can build it)";
			}
			cout << endl;
		}

		cout << "Switches:" << endl;
		for (int i = 0; i < level.switches; i++)
		{
			cout << "  switch " << i << ": " << (game.isSwitchOn(i) ? "ON" : "off") << endl;
		}
	}

	void showHelp()
	{
		cout << "Commands:" << endl;
		cout << "  build A B   build the bridge between islands A and B" << endl;
		cout << "  remove A B  remove a built bridge" << endl;
		cout << "  switch N    turn on switch N (unlocks its bridges)" << endl;
		cout << "  reset       start this level again" << endl;
		cout << "  help        show this list" << endl;
		cout << "  quit        back to the menu" << endl;
		cout << "Every build, remove and switch costs 1 move. Refused moves are free." << endl;
	}

	// Reads two integers. Returns false (and cleans the input) if that fails.
	bool readTwoInts(int& x, int& y)
	{
		if (cin >> x >> y)
		{
			return true;
		}
		if (cin.eof())
		{
			inputEnded = true;
			return false;
		}
		cin.clear();
		cin.ignore(10000, '\n');
		return false;
	}

	bool readOneInt(int& x)
	{
		if (cin >> x)
		{
			return true;
		}
		if (cin.eof())
		{
			inputEnded = true;
			return false;
		}
		cin.clear();
		cin.ignore(10000, '\n');
		return false;
	}

	// Plays one level. Returns true if the player won it at any point.
	bool playLevel(const LevelDef& level)
	{
		// A new object for every play: the Graph inside cannot be copied or restarted.
		BB* game = new BB();
		if (!loadLevel(*game, level))
		{
			cout << "Could not load the level." << endl;
			delete game;
			return false;
		}

		showHelp();
		bool leave = false;
		bool wonOnce = false;

		while (!leave && !inputEnded)
		{
			showBoard(*game, level);

			if (game->status() == BB::Status::won)
			{
				wonOnce = true;
				cout << "*** YOU WIN! The islands are connected. ***" << endl;
			}
			else if (game->status() == BB::Status::lost)
			{
				cout << "*** OUT OF MOVES. Type 'reset' to try again. ***" << endl;
			}

			cout << "> ";
			string command;
			if (!(cin >> command))
			{
				inputEnded = true;
				break;
			}

			if (command == "build" || command == "remove")
			{
				int a = 0;
				int b = 0;
				if (!readTwoInts(a, b))
				{
					cout << "Usage: " << command << " A B   (two island numbers)" << endl;
				}
				else
				{
					bool accepted = (command == "build") ? game->buildBridge(a, b)
						: game->removeBridge(a, b);
					if (!accepted)
					{
						cout << "Not allowed: " << explain(game->lastResult()) << endl;
					}
				}
			}
			else if (command == "switch")
			{
				int id = 0;
				if (!readOneInt(id))
				{
					cout << "Usage: switch N   (a switch number)" << endl;
				}
				else if (!game->activateSwitch(id))
				{
					cout << "Not allowed: " << explain(game->lastResult()) << endl;
				}
			}
			else if (command == "reset")
			{
				game->reset();
				cout << "Level reset." << endl;
			}
			else if (command == "help")
			{
				showHelp();
			}
			else if (command == "quit")
			{
				leave = true;
			}
			else
			{
				cout << "Unknown command. Type 'help'." << endl;
			}
		}

		delete game;
		return wonOnce;
	}
}

// ---------- The only public function of this file ----------

bool playBridgeBuilder()
{
	inputEnded = false;
	bool wonAny = false;
	bool done = false;

	while (!done && !inputEnded)
	{
		cout << endl;
		cout << "##### BRIDGE BUILDER #####" << endl;
		cout << "1) Easy" << endl;
		cout << "2) Hard" << endl;
		cout << "3) Back" << endl;
		cout << "Choose: ";

		int choice = 0;
		if (!readOneInt(choice))
		{
			if (!inputEnded)
			{
				cout << "Please type 1, 2 or 3." << endl;
			}
			continue;
		}

		if (choice == 1)
		{
			if (playLevel(easyLevel)) wonAny = true;
		}
		else if (choice == 2)
		{
			if (playLevel(hardLevel)) wonAny = true;
		}
		else if (choice == 3)
		{
			done = true;
		}
		else
		{
			cout << "Please type 1, 2 or 3." << endl;
		}
	}

	// Remove the rest of the last typed line so the next menu starts clean.
	if (cin.good())
	{
		cin.ignore(10000, '\n');
	}

	return wonAny;
}
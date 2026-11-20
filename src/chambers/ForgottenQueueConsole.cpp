#include <iostream>
#include <string>
#include <cctype>
#include "ForgottenQueueConsole.h"
#include "SpiritConveyerBelt.h"

using namespace std;

namespace
{
	struct SpiritRule
	{
		const char* a;
		const char* b;
		bool mustTouch;
	};

	struct SpiritLevel
	{
		const char* name;
		Difficulty diff;
		const char* const* roster;
		int rosterCount;
		const SpiritRule* rules;
		int ruleCount;
	};

	const char* const easyRoster[] = { "Water", "Grass", "Earth", "Fire" };
	const char* const hardRoster[] = { "Water", "Grass", "Light", "Fire", "Earth", "Dark", "Ice" };

	const SpiritRule easyRules[] = {
		{ "Grass", "Water", true  },
		{ "Water", "Fire",  false },
		{ "Grass", "Fire",  false }
	};

	const SpiritRule hardRules[] = {
		{ "Grass", "Water", true  },
		{ "Grass", "Light", true  },
		{ "Water", "Fire",  false },
		{ "Grass", "Fire",  false },
		{ "Ice",   "Fire",  false },
		{ "Dark",  "Fire",  false },
		{ "Light", "Dark",  false },
		{ "Earth", "Ice",   false },
		{ "Earth", "Water", false }
	};

	const SpiritLevel easyLevel = { "EASY", Difficulty::EASY, easyRoster, 4, easyRules, 3 };
	const SpiritLevel hardLevel = { "HARD", Difficulty::HARD, hardRoster, 7, hardRules, 9 };

	string normalise(string s)
	{
		for (size_t i = 0; i < s.size(); i++)
		{
			s[i] = (i == 0) ? (char)toupper((unsigned char)s[i]) : (char)tolower((unsigned char)s[i]);
		}
		return s;
	}

	bool onRoster(const SpiritLevel& level, const string& name)
	{
		for (int i = 0; i < level.rosterCount; i++)
		{
			if (name == level.roster[i]) return true;
		}
		return false;
	}

	bool onBelt(const SpiritConveyerBelt& belt, const string& name)
	{
		for (QNode<Spirit>* cur = belt.conveyerBelt.front; cur != nullptr; cur = cur->next)
		{
			if (cur->data.name == name) return true;
		}
		return false;
	}

	void printRule(const SpiritRule& rule)
	{
		if (rule.mustTouch)
		{
			cout << rule.a << " wants to be next to " << rule.b << "." << endl;
		}
		else
		{
			cout << rule.a << " cannot be next to " << rule.b << "." << endl;
		}
	}

	void showIntro(const SpiritLevel& level)
	{
		cout << endl;
		cout << "=== The Forgotten Queue - " << level.name << " ===" << endl;
		cout << "The spirits queue up to ride the conveyer belt, one after another." << endl;
		cout << "Line all " << level.rosterCount << " of them up so that nobody is unhappy with their neighbours:" << endl;
		for (int i = 0; i < level.ruleCount; i++)
		{
			cout << " - ";
			printRule(level.rules[i]);
		}
		cout << "Spirits are placed on the belt in order, first one added = front of the line." << endl;
		cout << "When the belt is full it is checked automatically." << endl;
		cout << endl << "Commands:" << endl;
		cout << "  add <spirit> : Put a spirit on the end of the belt." << endl;
		cout << "  undo         : Remove the spirit you added last." << endl;
		cout << "  reset        : Clear the whole belt." << endl;
		cout << "  help         : Show these instructions again." << endl;
		cout << "  quit         : Exit the game." << endl;
	}

	void showBelt(const SpiritConveyerBelt& belt, const SpiritLevel& level)
	{
		cout << endl << "Belt (front -> back): ";

		QNode<Spirit>* cur = belt.conveyerBelt.front;
		for (int i = 0; i < belt.beltSize; i++)
		{
			string label = "";
			if (cur != nullptr)
			{
				label = cur->data.name;
				cur = cur->next;
			}
			while (label.size() < 5) label += ' ';
			cout << "[" << label << "] ";
		}
		cout << endl;

		cout << "Waiting spirits:      ";
		bool any = false;
		for (int i = 0; i < level.rosterCount; i++)
		{
			if (!onBelt(belt, level.roster[i]))
			{
				cout << level.roster[i] << "  ";
				any = true;
			}
		}
		if (!any) cout << "(none)";
		cout << endl;
	}

	int positionOf(const SpiritConveyerBelt& belt, const char* name)
	{
		for (int i = 0; i < belt.beltSize; i++)
		{
			if (belt.UserAnswer[i].name == name) return i;
		}
		return -1;
	}

	void explainFailure(const SpiritConveyerBelt& belt, const SpiritLevel& level)
	{
		cout << "The belt jams! The spirits squabble and the line scatters." << endl;

		cout << "Your order was: ";
		for (int i = 0; i < belt.beltSize; i++)
		{
			cout << belt.UserAnswer[i].name << (i + 1 < belt.beltSize ? ", " : "");
		}
		cout << endl;

		cout << "Unhappy spirits:" << endl;
		for (int i = 0; i < level.ruleCount; i++)
		{
			const SpiritRule& rule = level.rules[i];
			int pa = positionOf(belt, rule.a);
			int pb = positionOf(belt, rule.b);
			bool touching = (pa - pb == 1 || pb - pa == 1);

			if (rule.mustTouch != touching)
			{
				cout << " - ";
				printRule(rule);
			}
		}
	}

	bool playLevel(const SpiritLevel& level)
	{
		SpiritConveyerBelt belt(level.diff);
		int attempts = 0;

		showIntro(level);

		while (!belt.isSolved())
		{
			showBelt(belt, level);
			cout << "> ";

			string command;
			if (!(cin >> command) || command == "quit")
			{
				return false;
			}

			if (command == "help")
			{
				showIntro(level);
			}
			else if (command == "add")
			{
				string name;
				if (!(cin >> name))
				{
					return false;
				}
				name = normalise(name);

				if (!onRoster(level, name))
				{
					cout << "There is no spirit called " << name << " in this level." << endl;
				}
				else if (onBelt(belt, name))
				{
					cout << name << " is already on the belt." << endl;
				}
				else
				{
					belt.insert(Spirit(name));
					cout << name << " steps onto the belt." << endl;

					if (belt.wasLastAttemptWrong())
					{
						attempts++;
						explainFailure(belt, level);
					}
				}
			}
			else if (command == "undo")
			{
				if (belt.conveyerBelt.isEmpty())
				{
					cout << "The belt is already empty." << endl;
				}
				else
				{
					cout << belt.conveyerBelt.rear->data.name << " steps back off the belt." << endl;
					belt.undo();
				}
			}
			else if (command == "reset")
			{
				belt.conveyerBelt.clear();
				cout << "The belt is cleared." << endl;
			}
			else
			{
				cout << "Unknown command. Type 'help' for instructions." << endl;
			}
		}

		cout << endl << "The belt hums along happily - every spirit is content!" << endl;
		cout << "Failed attempts: " << attempts << endl;
		return true;
	}
}

bool playForgottenQueue()
{
	if (!playLevel(easyLevel))
	{
		return false;
	}

	cout << endl << "Get ready for HARD difficulty..." << endl;
	if (!playLevel(hardLevel))
	{
		return false;
	}

	cout << endl << "You solved The Forgotten Queue!" << endl;
	return true;
}

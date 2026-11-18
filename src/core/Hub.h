#pragma once

#include <string>
#include "GameProgress.h"

// One name for each chamber, in the order they open in story mode.
enum ChamberId
{
	stackVault,
	forgottenQueue,
	shiftingMaze,
	bridgeBuilder
};

// The hub is the main hall with one door for each chamber.
// Story mode: doors open one by one, in order.
// Practice mode: every door is open and story progress is not changed.
class Hub
{
public:
	// Shows the main menu until the player exits.
	void run();

	// Opens one chamber directly (used by the --scene option).
	// Names: stack, queue, tree, graph. Returns false for an unknown name.
	bool openByName(const std::string& name);

private:
	GameProgress progress;

	void runStory();
	void runPractice();
	void showDoors(bool practice) const;
	void openChamber(int index, bool practice);
	const char* doorStatus(int index, bool practice) const;

	// Everything that is different for each chamber is kept here.
	static const char* chamberName(int index);
	static const char* chamberTopic(int index);
	static bool isReady(int index);
	static bool playChamber(int index);

	static int readChoice();
};

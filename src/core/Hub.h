#pragma once

#include <string>
#include "GameProgress.h"

enum ChamberId
{
	stackVault,
	forgottenQueue,
	shiftingMaze,
	bridgeBuilder
};

class Hub
{
public:
	void run();

	bool openByName(const std::string& name);

private:
	GameProgress progress;

	void runStory();
	void runPractice();
	void showDoors(bool practice) const;
	void openChamber(int index, bool practice);
	const char* doorStatus(int index, bool practice) const;

	static const char* chamberName(int index);
	static const char* chamberTopic(int index);
	static bool isReady(int index);
	static bool playChamber(int index);

	static int readChoice();
};

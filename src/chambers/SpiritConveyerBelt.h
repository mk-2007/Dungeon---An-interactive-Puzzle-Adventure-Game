#pragma once
#include <string>
#include "../ds/Queue.h"

class Spirit
{
public:
	std::string name;
	bool used;

	Spirit() : name(""), used(false) {}
	Spirit(std::string name) : name(name), used(false) {}
};

enum Difficulty
{
	EASY,
	HARD
};

class SpiritConveyerBelt
{
public:
	Queue<Spirit> conveyerBelt;
	Spirit** ArrayofSolutions;
	Spirit* UserAnswer;
	int numSolutions;
	int beltSize;
	bool solutionFound;
	bool lastAttemptWrong;      

	SpiritConveyerBelt(Difficulty diff);
	~SpiritConveyerBelt();

	SpiritConveyerBelt(const SpiritConveyerBelt&) = delete;
	SpiritConveyerBelt& operator=(const SpiritConveyerBelt&) = delete;

	bool checkAnswer();
	bool match(const Spirit& user, const Spirit& system);
	bool insert(Spirit key); 
	void undo();
	bool isSolved() const { return solutionFound; }
	bool wasLastAttemptWrong() const { return lastAttemptWrong; }
};

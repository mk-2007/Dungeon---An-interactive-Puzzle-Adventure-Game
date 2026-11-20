#include "SpiritConveyerBelt.h"
#include "../ds/Queue.h"

SpiritConveyerBelt::SpiritConveyerBelt(Difficulty diff)
	: conveyerBelt(diff == EASY ? 4 : 7),
	ArrayofSolutions(nullptr),
	UserAnswer(nullptr),
	numSolutions(0),
	beltSize(0),
	solutionFound(false),
	lastAttemptWrong(false)
{
	if (diff == EASY)
	{
		beltSize = 4;
		numSolutions = 4;
		ArrayofSolutions = new Spirit * [numSolutions];
		UserAnswer = new Spirit[beltSize];

		ArrayofSolutions[0] = new Spirit[4]{ Spirit("Water"), Spirit("Grass"), Spirit("Earth"), Spirit("Fire") };
		ArrayofSolutions[1] = new Spirit[4]{ Spirit("Fire"), Spirit("Earth"), Spirit("Grass"), Spirit("Water") };
		ArrayofSolutions[2] = new Spirit[4]{ Spirit("Grass"), Spirit("Water"), Spirit("Earth"), Spirit("Fire") };
		ArrayofSolutions[3] = new Spirit[4]{ Spirit("Fire"), Spirit("Earth"), Spirit("Water"), Spirit("Grass") };
	}
	else
	{
		beltSize = 7;
		numSolutions = 20;
		ArrayofSolutions = new Spirit * [numSolutions];
		UserAnswer = new Spirit[beltSize];

		ArrayofSolutions[0] = new Spirit[7]{ Spirit("Water"), Spirit("Grass"), Spirit("Light"), Spirit("Fire"), Spirit("Earth"), Spirit("Dark"), Spirit("Ice") };
		ArrayofSolutions[1] = new Spirit[7]{ Spirit("Water"), Spirit("Grass"), Spirit("Light"), Spirit("Ice"), Spirit("Dark"), Spirit("Earth"), Spirit("Fire") };
		ArrayofSolutions[2] = new Spirit[7]{ Spirit("Ice"), Spirit("Water"), Spirit("Grass"), Spirit("Light"), Spirit("Fire"), Spirit("Earth"), Spirit("Dark") };
		ArrayofSolutions[3] = new Spirit[7]{ Spirit("Dark"), Spirit("Ice"), Spirit("Water"), Spirit("Grass"), Spirit("Light"), Spirit("Fire"), Spirit("Earth") };
		ArrayofSolutions[4] = new Spirit[7]{ Spirit("Dark"), Spirit("Ice"), Spirit("Water"), Spirit("Grass"), Spirit("Light"), Spirit("Earth"), Spirit("Fire") };
		ArrayofSolutions[5] = new Spirit[7]{ Spirit("Ice"), Spirit("Dark"), Spirit("Water"), Spirit("Grass"), Spirit("Light"), Spirit("Fire"), Spirit("Earth") };
		ArrayofSolutions[6] = new Spirit[7]{ Spirit("Ice"), Spirit("Dark"), Spirit("Water"), Spirit("Grass"), Spirit("Light"), Spirit("Earth"), Spirit("Fire") };
		ArrayofSolutions[7] = new Spirit[7]{ Spirit("Earth"), Spirit("Dark"), Spirit("Ice"), Spirit("Water"), Spirit("Grass"), Spirit("Light"), Spirit("Fire") };
		ArrayofSolutions[8] = new Spirit[7]{ Spirit("Fire"), Spirit("Earth"), Spirit("Dark"), Spirit("Water"), Spirit("Grass"), Spirit("Light"), Spirit("Ice") };
		ArrayofSolutions[9] = new Spirit[7]{ Spirit("Fire"), Spirit("Earth"), Spirit("Dark"), Spirit("Ice"), Spirit("Water"), Spirit("Grass"), Spirit("Light") };
		ArrayofSolutions[10] = new Spirit[7]{ Spirit("Light"), Spirit("Grass"), Spirit("Water"), Spirit("Ice"), Spirit("Dark"), Spirit("Earth"), Spirit("Fire") };
		ArrayofSolutions[11] = new Spirit[7]{ Spirit("Fire"), Spirit("Light"), Spirit("Grass"), Spirit("Water"), Spirit("Ice"), Spirit("Dark"), Spirit("Earth") };
		ArrayofSolutions[12] = new Spirit[7]{ Spirit("Ice"), Spirit("Light"), Spirit("Grass"), Spirit("Water"), Spirit("Dark"), Spirit("Earth"), Spirit("Fire") };
		ArrayofSolutions[13] = new Spirit[7]{ Spirit("Fire"), Spirit("Earth"), Spirit("Light"), Spirit("Grass"), Spirit("Water"), Spirit("Dark"), Spirit("Ice") };
		ArrayofSolutions[14] = new Spirit[7]{ Spirit("Fire"), Spirit("Earth"), Spirit("Light"), Spirit("Grass"), Spirit("Water"), Spirit("Ice"), Spirit("Dark") };
		ArrayofSolutions[15] = new Spirit[7]{ Spirit("Earth"), Spirit("Fire"), Spirit("Light"), Spirit("Grass"), Spirit("Water"), Spirit("Dark"), Spirit("Ice") };
		ArrayofSolutions[16] = new Spirit[7]{ Spirit("Earth"), Spirit("Fire"), Spirit("Light"), Spirit("Grass"), Spirit("Water"), Spirit("Ice"), Spirit("Dark") };
		ArrayofSolutions[17] = new Spirit[7]{ Spirit("Dark"), Spirit("Earth"), Spirit("Fire"), Spirit("Light"), Spirit("Grass"), Spirit("Water"), Spirit("Ice") };
		ArrayofSolutions[18] = new Spirit[7]{ Spirit("Fire"), Spirit("Earth"), Spirit("Dark"), Spirit("Ice"), Spirit("Light"), Spirit("Grass"), Spirit("Water") };
		ArrayofSolutions[19] = new Spirit[7]{ Spirit("Ice"), Spirit("Dark"), Spirit("Earth"), Spirit("Fire"), Spirit("Light"), Spirit("Grass"), Spirit("Water") };
	}
}

SpiritConveyerBelt::~SpiritConveyerBelt()
{
	for (int i = 0; i < numSolutions; i++)
	{
		delete[] ArrayofSolutions[i];
	}
	delete[] ArrayofSolutions;
	delete[] UserAnswer;
}

bool SpiritConveyerBelt::insert(Spirit key)
{

	if (solutionFound || conveyerBelt.isFull()) return false;

	
	for (QNode<Spirit>* cur = conveyerBelt.front; cur != nullptr; cur = cur->next)
	{
		if (cur->data.name == key.name)
		{
			return false;
		}
	}

	lastAttemptWrong = false;
	conveyerBelt.enqueue(key);

	if (conveyerBelt.isFull())
	{
		checkAnswer();
	}
	return true;
}

void SpiritConveyerBelt::undo()
{
	if (conveyerBelt.isEmpty()) return;

	int n = conveyerBelt.getSize();
	Spirit temp;
	for (int i = 0; i < n - 1; i++)
	{
		conveyerBelt.dequeue(temp);
		conveyerBelt.enqueue(temp);
	}
	conveyerBelt.dequeue(temp);
}

bool SpiritConveyerBelt::checkAnswer()
{
	if (!conveyerBelt.isFull()) return false;

	for (int i = 0; i < beltSize; i++)
	{
		conveyerBelt.dequeue(UserAnswer[i]);
	}
	conveyerBelt.clear();

	for (int i = 0; i < numSolutions; i++)
	{
		bool correct = true;
		for (int j = 0; j < beltSize; j++)
		{
			if (!match(UserAnswer[j], ArrayofSolutions[i][j]))
			{
				correct = false;
				break;
			}
		}
		if (correct)
		{
			solutionFound = true;
			lastAttemptWrong = false;
			return true;
		}
	}

	lastAttemptWrong = true;
	return false;
}

bool SpiritConveyerBelt::match(const Spirit& user, const Spirit& system)
{
	return user.name == system.name;
}

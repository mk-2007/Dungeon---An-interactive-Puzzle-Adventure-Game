#include "SpiritConveyerBelt.h"
#include "../ds/Queue.h"

SpiritConveyerBelt::SpiritConveyerBelt(Difficulty diff) : conveyerBelt(7), SolutionFound(false)
{
	solutionFound = false;
	if (diff == EASY)
	{
		ArrayofSolutions = new Spirit * [2];
		UserAnswer = new Spirit[4];
			
		ArrayofSolutions[0] = new Spirit[4]{ Spirit("Water"), Spirit("Grass"), Spirit("Earth"), Spirit("Fire") };
		ArrayofSolutions[1] = new Spirit[4]{ Spirit("Fire"), Spirit("Earth"), Spirit("Grass"), Spirit("Water") };
	}
	else if (diff == HARD)
	{
		conveyerBelt.capacity = 7;
		ArrayofSolutions = new Spirit * [20];
		UserAnswer = new Spirit[7];
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
	for (int i = 0; i < 20; i++)
	{
		delete[] ArrayofSolutions[i];
	}
	delete[] ArrayofSolutions;
	delete[] UserAnswer;
}

void SpiritConveyerBelt::insert(Spirit key)
{
	if (!key.used)
	{
		conveyerBelt.enqueue(key);
	}
	if (conveyerBelt.isFull())
	{
		checkAnswer();
	}
}

void SpiritConveyerBelt::undo()
{
	if (conveyerBelt.isEmpty()) return;
	Spirit temp;
	conveyerBelt.dequeue(temp);
}

bool SpiritConveyerBelt::checkAnswer()
{
	for (int i = 0; i < conveyerBelt.capacity; i++)
	{
		conveyerBelt.dequeue(UserAnswer[i]);
	}
	conveyerBelt.clear();

	for (int i = 0; i < 20; i++)
	{
		bool correct = true;
		for (int j = 0; j < 7; j++)
		{
			if (!match(UserAnswer[j], ArrayofSolutions[i][j]))
			{
				correct = false;
			}
		}
		if (correct)
		{
			solutionFound = true;
			return true;
		}
	}
	return false;
}

bool match(Spirit User, Spirit System)
{
	return User.name == System.name;
}
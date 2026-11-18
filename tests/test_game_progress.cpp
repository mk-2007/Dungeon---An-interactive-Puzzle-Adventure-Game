#include <iostream>
#include <string>

#include "../src/core/GameProgress.h"

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
	GameProgress progress;

	check(progress.isUnlocked(0), "First chamber is open at the start");
	check(!progress.isUnlocked(1) && !progress.isUnlocked(2) &&
		!progress.isUnlocked(3), "Other chambers are locked at the start");
	check(progress.completedCount() == 0, "Nothing is completed at the start");

	check(!progress.completeChamber(2), "A locked chamber cannot be completed");
	check(!progress.isCompleted(2), "Locked chamber stays not completed");

	check(progress.completeChamber(0), "First chamber can be completed");
	check(progress.isCompleted(0), "First chamber is marked completed");
	check(progress.isUnlocked(1), "Completing chamber 1 unlocks chamber 2");
	check(!progress.isUnlocked(2), "Chamber 3 is still locked");

	check(progress.completeChamber(1) && progress.completeChamber(2),
		"Chambers 2 and 3 can be completed in order");
	check(!progress.allCompleted(), "Game is not finished after three chambers");
	check(progress.completeChamber(3), "Last chamber can be completed");
	check(progress.allCompleted() && progress.completedCount() == 4,
		"All four chambers completed finishes the game");

	check(!progress.isUnlocked(-1) && !progress.isCompleted(4),
		"Invalid chamber numbers are rejected");
	check(!progress.completeChamber(7), "Completing an invalid chamber fails");

	progress.reset();
	check(progress.completedCount() == 0 && !progress.isUnlocked(1),
		"Reset returns to the start");

	return allPassed ? 0 : 1;
}

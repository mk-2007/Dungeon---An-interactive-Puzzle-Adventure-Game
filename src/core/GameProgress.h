#pragma once

class GameProgress
{
public:
	static const int chamberCount = 4;

	GameProgress()
	{
		reset();
	}

	void reset()
	{
		for (int i = 0; i < chamberCount; i++)
		{
			unlocked[i] = false;
			completed[i] = false;
		}
		unlocked[0] = true;
	}

	bool isUnlocked(int chamber) const
	{
		return isValid(chamber) && unlocked[chamber];
	}

	bool isCompleted(int chamber) const
	{
		return isValid(chamber) && completed[chamber];
	}

	bool completeChamber(int chamber)
	{
		if (!isUnlocked(chamber))
		{
			return false;
		}

		completed[chamber] = true;
		if (chamber + 1 < chamberCount)
		{
			unlocked[chamber + 1] = true;
		}
		return true;
	}

	int completedCount() const
	{
		int total = 0;
		for (int i = 0; i < chamberCount; i++)
		{
			if (completed[i])
			{
				total++;
			}
		}
		return total;
	}

	bool allCompleted() const
	{
		return completedCount() == chamberCount;
	}

private:
	bool unlocked[chamberCount];
	bool completed[chamberCount];

	static bool isValid(int chamber)
	{
		return chamber >= 0 && chamber < chamberCount;
	}
};

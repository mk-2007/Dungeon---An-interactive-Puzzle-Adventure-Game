#include <iostream>
#include <string>
#include "core/Hub.h"

using namespace std;

int main(int argc, char* argv[])
{
	Hub hub;

	// Open one chamber directly: dungeon --scene tree
	if (argc == 3 && string(argv[1]) == "--scene")
	{
		if (!hub.openByName(argv[2]))
		{
			cout << "Unknown chamber. Use: stack, queue, tree or graph." << endl;
			return 1;
		}
		return 0;
	}

	hub.run();
	return 0;
}

#include <iostream>
#include "../src/chambers/BridgeBuilderConsole.h"

int main()
{
	bool won = playBridgeBuilder();
	std::cout << "playBridgeBuilder() returned " << (won ? "true" : "false") << std::endl;
	return 0;
}
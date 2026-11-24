#include <iostream>
#include <string>
#include "core/Game.h"
#include "core/Hub.h"

using namespace std;

int main(int argc, char* argv[])
{
    // Console fallback flag
    if (argc >= 2 && string(argv[1]) == "--console")
    {
        Hub consoleHub;
        if (argc == 4 && string(argv[2]) == "--scene")
        {
            if (!consoleHub.openByName(argv[3]))
            {
                cout << "Unknown chamber. Use: stack, queue, tree or graph." << endl;
                return 1;
            }
            return 0;
        }
        consoleHub.run();
        return 0;
    }

    Game game;

    // CLI Scene Switcher: --scene <stack|queue|tree|graph>
    if (argc == 3 && string(argv[1]) == "--scene")
    {
        string sceneName = argv[2];
        if (sceneName == "stack")
        {
            game.run(SceneId::STACK_VAULT);
            return 0;
        }
        else if (sceneName == "queue")
        {
            game.run(SceneId::FORGOTTEN_QUEUE);
            return 0;
        }
        else if (sceneName == "tree")
        {
            game.run(SceneId::SHIFTING_MAZE);
            return 0;
        }
        else if (sceneName == "graph")
        {
            game.run(SceneId::BRIDGE_BUILDER);
            return 0;
        }
        else
        {
            cout << "Unknown chamber. Use: stack, queue, tree or graph." << endl;
            return 1;
        }
    }

    // Default: Launch 2D Raylib GUI Adventure
    game.run(SceneId::HUB);
    return 0;
}

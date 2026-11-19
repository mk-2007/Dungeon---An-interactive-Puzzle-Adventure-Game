#include <iostream>
#include <string>
#include "StackVaultConsole.h"
#include "StackChamber.h"

using namespace std;

void showIntro(Difficulty diff)
{
    cout << "\n=== Stack Vault - " << (diff == Difficulty::EASY ? "EASY" : "HARD") << " ===\n";
    cout << "Goal: Reveal the key (1000) by moving elements to the Helper stack.\n\n";
    cout << "Rules for moving to Helper:\n\n";
    cout << " - You can only move the top element of a stack.\n";
    cout << " - The signs must alternate (+ve, -ve, +ve, -ve...).\n";
    if (diff == Difficulty::HARD)
    {
        cout << " - The parity must ALSO alternate (even, odd, even, odd...).\n";
    }
    cout << "\n''Commands:\n";
    cout << "  move <0-3> : Move the top of the chosen stack to the Helper.\n";
    cout << "  back <0-3> : Move the top of the Helper back to the chosen stack.\n";
    cout << "  help       : Show these instructions again.\n";
    cout << "  quit       : Exit the game.\n";
}

void showStacks(const StackChamber& chamber)
{
    cout << "\n";
    bool topDrawn[5] = { false, false, false, false, false };

   
    for (int row = 4; row >= 0; row--) 
    {
        
        for (int i = 0; i < 5; ++i) 
        {
          
            int size = (i == 4) ? chamber.helper.size() : chamber.s[i].size();

           
            if (size > 0 && !topDrawn[i] && row == size - 1) 
            {
              
                int val = (i == 4) ? chamber.helper.Arr[row] : chamber.s[i].Arr[row];
                string s = to_string(val);
                
                
                int pad = 7 - s.length();
                cout << "|" << string(pad / 2, ' ') << s << string(pad - pad / 2, ' ') << "|   ";
                
                topDrawn[i] = true; 
            }
            
            else if (row < size) 
            {
                cout << "|       |   ";
            }
           
            else 
            {
                cout << "            ";
            }
        }
        cout << "\n";
    }

    cout << "+-------+   +-------+   +-------+   +-------+   +-------+\n";
    cout << " Stack 1     Stack 2     Stack 3     Stack 4     Helper  \n\n";
}

bool playLevel(Difficulty diff)
{
    StackChamber chamber(diff);
    showIntro(diff);

    while (!chamber.isWin())
    {
        showStacks(chamber);
        cout << "> ";

        string command;
        if (!(cin >> command) || command == "quit")
        {
            return false;
        }

        if (command == "help")
        {
            showIntro(diff);
        }
        else if (command == "move")
        {
            int index;
            if (cin >> index)
            {
                if (chamber.canMovetohelper(index))
                {
                    chamber.moveStackToHelper(index);
                }
                else
                {
                    cout << "Invalid move! Remember the sign/parity rules.\n";
                }
            }
            else
            {
                cin.clear(); cin.ignore(1000, '\n');
                cout << "Invalid input. Type: move <0-3>\n";
            }
        }
        else if (command == "back")
        {
            int index;
            if (cin >> index)
            {
                if (chamber.canMovetoStack(index))
                {
                    chamber.moveHelperToStack(index);
                }
                else
                {
                    cout << "Invalid move! Helper is empty or index is out of bounds.\n";
                }
            }
            else
            {
                cin.clear(); cin.ignore(1000, '\n');
                cout << "Invalid input. Type: back <0-3>\n";
            }
        }
        else
        {
            cout << "Unknown command. Type 'help' for instructions.\n";
        }
    }

    showStacks(chamber); 
    cout << "\nYou revealed the key and won!\n";
    return true;
}

bool playStackChamber()
{
  
    if (!playLevel(Difficulty::EASY))
    {
        return false;
    }

   
    cout << "\nGet ready for HARD difficulty...\n";
    if (!playLevel(Difficulty::HARD))
    {
        return false;
    }

    cout << "\nYou solved the entire Stack Vault!\n";
    return true;
}

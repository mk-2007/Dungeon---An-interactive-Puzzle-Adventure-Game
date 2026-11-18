#pragma once
#include "../ds/Stack.h"

enum class Difficulty { EASY, HARD };

class StackChamber
{
public:
    Difficulty difficulty;
    Stack <int> s[4], helper;
    bool keyFound;

    StackChamber(Difficulty Diff);
    void moveStackToHelper(int index);
    void moveHelperToStack(int index);
    bool canMovetohelper(int index) const;
    bool canMovetoStack(int index) const;

    
    bool isWin() const;                
    bool isKeyRevealed(int index);      
};
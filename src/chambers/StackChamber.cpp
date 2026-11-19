#include <iostream>
#include <cmath>
#include "StackChamber.h"

using namespace std;

StackChamber::StackChamber(Difficulty Diff)
{
    difficulty = Diff;
    keyFound = false;

    if (Diff == Difficulty::EASY)
    {
        s[0].push(3); s[0].push(4); s[0].push(-5); s[0].push(6);
        s[1].push(1); s[1].push(-2); s[1].push(6); s[1].push(-7);
        s[2].push(1000); s[2].push(-8); s[2].push(9); s[2].push(-10);
        s[3].push(-1); s[3].push(2); s[3].push(-3); s[3].push(4);
    }
    else
    {
        s[0].push(-4); s[0].push(5); s[0].push(-6); s[0].push(7);
        s[1].push(1000); s[1].push(-2); s[1].push(3); s[1].push(-4);
        s[2].push(-8); s[2].push(9); s[2].push(-2); s[2].push(3);
        s[3].push(5); s[3].push(-4); s[3].push(1); s[3].push(-3);
    }
}

bool StackChamber::isWin() const
{
    return keyFound;
}

bool StackChamber::isKeyRevealed(int index)
{
    if (index < 0 || index >= 4) return false;
    if (s[index].isEmpty()) return false;

    int temp;
    s[index].peek(temp);

    if (temp == 1000)
    {
        keyFound = true;
        return true;
    }
    return false;
}

bool StackChamber::canMovetohelper(int index) const
{
    if (index < 0 || index >= 4) return false;
    if (s[index].isEmpty()) return false;
    if (helper.isEmpty()) return true;

    int stackTop, helperTop;
    s[index].peek(stackTop);
    helper.peek(helperTop);

    bool oppositeSigns = (stackTop > 0 && helperTop < 0) || (stackTop < 0 && helperTop > 0);
    if (!oppositeSigns) return false;

    if (difficulty == Difficulty::HARD)
    {
        bool isStackTopEven = (abs(stackTop) % 2 == 0);
        bool isHelperTopEven = (abs(helperTop) % 2 == 0);

        if (isStackTopEven == isHelperTopEven) return false;
    }

    return true;
}

bool StackChamber::canMovetoStack(int index) const
{
    if (index < 0 || index >= 4) return false;
    if (helper.isEmpty()) return false;
    return true;
}

void StackChamber::moveStackToHelper(int index)
{
    if (index < 0 || index >= 4) return;
    if (canMovetohelper(index))
    {
        int val;
        s[index].pop(val);
        helper.push(val);
        isKeyRevealed(index);
    }
}

void StackChamber::moveHelperToStack(int index)
{
    if (index < 0 || index >= 4) return;
    if (canMovetoStack(index))
    {
        int val;
        helper.pop(val);
        s[index].push(val);
        isKeyRevealed(index);
    }
}

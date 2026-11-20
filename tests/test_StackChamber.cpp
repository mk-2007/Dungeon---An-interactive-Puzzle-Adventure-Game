#include <iostream>
#include <string>
#include "../src/chambers/StackChamber.h"

using namespace std;

void check(bool condition, string name)
{
    if (condition) cout << "PASS: " << name << endl;
    else           cout << "FAIL: " << name << endl;
}

int main()
{
    cout << "====================================\n";
    cout << "    TESTING EASY DIFFICULTY         \n";
    cout << "====================================\n";
    StackChamber easy(Difficulty::EASY);

    check(!easy.isWin(), "EASY: Game starts without a win.");
    
   
    check(easy.canMovetohelper(2), "EASY: Can move to empty helper (-10).");
    check(!easy.canMovetoStack(0), "EASY: Cannot move from empty helper.");

    easy.moveStackToHelper(2); 
    check(!easy.isWin(), "EASY: Key not found after moving -10.");

   
    check(easy.canMovetohelper(2), "EASY: Can move 9 onto -10 (opposite signs).");
    easy.moveStackToHelper(2); 

   
    check(easy.canMovetohelper(2), "EASY: Can move -8 onto 9 (opposite signs).");
    easy.moveStackToHelper(2); 

   
    check(easy.isWin(), "EASY: Win condition met (Key 1000 revealed).");
    check(easy.isKeyRevealed(2), "EASY: isKeyRevealed returns true for key stack.");

    check(easy.canMovetoStack(0), "EASY: Can move from helper to stack.");
    easy.moveHelperToStack(0);

    check(!easy.isWin() == false, "EASY: Win state persists even after moving crates.");

   
    check(easy.canMovetohelper(1), "EASY: Can move -7 onto 9 (opposite signs).");


    cout << "\n====================================\n";
    cout << "    TESTING HARD DIFFICULTY         \n";
    cout << "====================================\n";
    StackChamber hard(Difficulty::HARD);

    check(!hard.isWin(), "HARD: Game starts without a win.");

    
    check(hard.canMovetohelper(1), "HARD: Can move -4 to empty helper.");
    hard.moveStackToHelper(1); 

    
    check(hard.canMovetohelper(1), "HARD: Can move 3 onto -4 (opp signs, opp parity).");
    hard.moveStackToHelper(1);

   
    check(hard.canMovetohelper(1), "HARD: Can move -2 onto 3 (opp signs, opp parity).");
    hard.moveStackToHelper(1); 

    
    check(hard.isWin(), "HARD: Win condition met (Key 1000 revealed).");

   
    check(hard.canMovetohelper(0), "HARD: Can move 7 onto -2 (opp signs, opp parity).");
    hard.moveStackToHelper(0);

    
    check(!hard.canMovetohelper(3), "HARD: Cannot move -3 onto 7 (opp signs, SAME parity - both odd).");

    
    check(!hard.canMovetohelper(2), "HARD: Cannot move 3 onto 7 (same signs).");


    cout << "\n====================================\n";
    cout << "    TESTING EDGE CASES & ERRORS     \n";
    cout << "====================================\n";

    StackChamber emptyTest(Difficulty::EASY);
    check(!emptyTest.canMovetohelper(5), "Cannot move from out-of-bounds stack index (5).");

    return 0;
}

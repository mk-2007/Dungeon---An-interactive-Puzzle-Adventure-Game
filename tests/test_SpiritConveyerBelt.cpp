#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "../src/chambers/SpiritConveyerBelt.h"

using namespace std;

int passed = 0;
int failed = 0;

void check(bool condition, string name)
{
    if (condition)
    {
        cout << "PASS: " << name << endl;
        passed++;
    }
    else
    {
        cout << "FAIL: " << name << endl;
        failed++;
    }
}

void feed(SpiritConveyerBelt& belt, const vector<string>& names)
{
    for (size_t i = 0; i < names.size(); i++)
    {
        belt.insert(Spirit(names[i]));
    }
}

bool feedSolves(Difficulty diff, const vector<string>& names)
{
    SpiritConveyerBelt belt(diff);
    feed(belt, names);
    return belt.isSolved();
}

bool adjacent(const vector<string>& order, string a, string b)
{
    for (size_t i = 0; i + 1 < order.size(); i++)
    {
        if ((order[i] == a && order[i + 1] == b) || (order[i] == b && order[i + 1] == a))
        {
            return true;
        }
    }
    return false;
}

bool followsRules(Difficulty diff, const vector<string>& order)
{
    if (diff == EASY)
    {
        return !adjacent(order, "Fire", "Water")
            && !adjacent(order, "Grass", "Fire")
            && !adjacent(order, "Water", "Earth")
            && adjacent(order, "Grass", "Water");
    }

    return !adjacent(order, "Fire", "Water")
        && !adjacent(order, "Grass", "Fire")
        && !adjacent(order, "Ice", "Fire")
        && !adjacent(order, "Light", "Dark")
        && !adjacent(order, "Grass", "Dark")
        && !adjacent(order, "Fire", "Dark")
        && !adjacent(order, "Earth", "Ice")
        && !adjacent(order, "Earth", "Water")
        && !adjacent(order, "Grass", "Ice")
        && adjacent(order, "Grass", "Water")
        && adjacent(order, "Grass", "Light");
}

void sweepAllPermutations(Difficulty diff, vector<string> names, int expectedValid, string label)
{
    sort(names.begin(), names.end());

    int accepted = 0;
    int ruleValid = 0;
    int disagreements = 0;

    do
    {
        bool beltAccepts = feedSolves(diff, names);
        bool rulesAllow = followsRules(diff, names);

        if (beltAccepts) accepted++;
        if (rulesAllow) ruleValid++;

        if (beltAccepts != rulesAllow)
        {
            disagreements++;
            cout << "  MISMATCH (" << (beltAccepts ? "belt accepts, rules forbid" : "rules allow, belt rejects") << "): ";
            for (size_t i = 0; i < names.size(); i++) cout << names[i] << " ";
            cout << endl;
        }
    } while (next_permutation(names.begin(), names.end()));

    check(ruleValid == expectedValid, label + ": rules produce exactly " + to_string(expectedValid) + " valid orders (got " + to_string(ruleValid) + ").");
    check(accepted == expectedValid, label + ": belt accepts exactly " + to_string(expectedValid) + " orders (got " + to_string(accepted) + ").");
    check(disagreements == 0, label + ": belt and rules agree on every permutation.");
}

int main()
{
    cout << "=================\n";
    cout << "EASY\n";
    cout << "=================\n";

    SpiritConveyerBelt easy(EASY);

    check(!easy.isSolved(), "EASY: Game starts unsolved.");
    easy.undo();
    check(!easy.isSolved(), "EASY: Undo on empty belt does nothing.");
    check(easy.match(Spirit("Fire"), Spirit("Fire")), "EASY: match true for same name.");
    check(!easy.match(Spirit("Fire"), Spirit("Water")), "EASY: match false for different names.");

    Spirit usedFire("Fire");
    usedFire.used = true;
    check(easy.match(usedFire, Spirit("Fire")), "EASY: match ignores the used flag.");

    check(!easy.checkAnswer(), "EASY: checkAnswer on empty belt returns false.");
    easy.insert(Spirit("Water"));
    easy.insert(Spirit("Grass"));
    check(!easy.checkAnswer(), "EASY: checkAnswer on partial belt returns false.");
    check(!easy.isSolved(), "EASY: Partial belt does not solve.");

    check(feedSolves(EASY, { "Water", "Grass", "Earth", "Fire" }), "EASY: Solution 1 (Water Grass Earth Fire) solves.");
    check(feedSolves(EASY, { "Fire", "Earth", "Grass", "Water" }), "EASY: Solution 2 (Fire Earth Grass Water) solves.");

    check(!feedSolves(EASY, { "Water", "Grass", "Earth" }), "EASY: Three correct spirits (not full) does not solve.");
    check(!feedSolves(EASY, { "Water", "Earth", "Grass", "Fire" }), "EASY: Water next to Earth is wrong.");
    check(!feedSolves(EASY, { "Fire", "Water", "Grass", "Earth" }), "EASY: Fire next to Water is wrong.");
    check(!feedSolves(EASY, { "Grass", "Fire", "Earth", "Water" }), "EASY: Grass next to Fire is wrong.");
    check(!feedSolves(EASY, { "Earth", "Fire", "Grass", "Water" }), "EASY: Right spirits, wrong order does not solve.");
    check(!feedSolves(EASY, { "Water", "Grass", "Earth", "Ice" }), "EASY: Spirit not in easy mode does not solve.");
    check(!feedSolves(EASY, { "Earth", "Grass", "Water", "Fire" }), "EASY: Reverse of a wrong order does not solve.");

    check(feedSolves(EASY, { "Water", "Water", "Grass", "Grass", "Earth", "Earth", "Fire", "Fire" }), "EASY: Duplicate inserts are ignored and solution still solves.");
    check(!feedSolves(EASY, { "Water", "Water", "Water", "Water" }), "EASY: Four identical spirits never fill the belt.");

    {
        SpiritConveyerBelt belt(EASY);
        feed(belt, { "Water", "Grass", "Fire" });
        belt.undo();
        feed(belt, { "Earth", "Fire" });
        check(belt.isSolved(), "EASY: Undo removes the most recent spirit, not the first.");
    }

    {
        SpiritConveyerBelt belt(EASY);
        belt.insert(Spirit("Water"));
        belt.undo();
        feed(belt, { "Water", "Grass", "Earth", "Fire" });
        check(belt.isSolved(), "EASY: A spirit can be reinserted after undo.");
    }

    {
        SpiritConveyerBelt belt(EASY);
        feed(belt, { "Fire", "Earth", "Grass" });
        belt.undo();
        belt.undo();
        belt.undo();
        belt.undo();
        belt.undo();
        feed(belt, { "Water", "Grass", "Earth", "Fire" });
        check(belt.isSolved(), "EASY: Undoing past empty is safe and belt still works.");
    }

    {
        SpiritConveyerBelt belt(EASY);
        feed(belt, { "Water", "Earth", "Grass", "Fire" });
        check(!belt.isSolved(), "EASY: Wrong full answer is not solved.");
        feed(belt, { "Fire", "Earth", "Grass", "Water" });
        check(belt.isSolved(), "EASY: Belt resets after a wrong answer and accepts a correct one.");
    }

    {
        SpiritConveyerBelt belt(EASY);
        feed(belt, { "Water", "Grass", "Earth", "Fire" });
        feed(belt, { "Water", "Earth", "Grass", "Fire" });
        check(belt.isSolved(), "EASY: Solved state persists after a later wrong answer.");
    }

    {
        SpiritConveyerBelt belt(EASY);
        feed(belt, { "Water", "Grass", "Earth", "Fire" });
        feed(belt, { "Fire", "Earth", "Grass", "Water" });
        check(belt.isSolved(), "EASY: Two solutions back to back stay solved.");
    }

    sweepAllPermutations(EASY, { "Fire", "Water", "Grass", "Earth" }, 2, "EASY sweep");


    cout << "\n=================\n";
    cout << "HARD\n";
    cout << "=================\n";

    SpiritConveyerBelt hard(HARD);

    check(!hard.isSolved(), "HARD: Game starts unsolved.");
    hard.undo();
    check(!hard.isSolved(), "HARD: Undo on empty belt does nothing.");
    check(hard.match(Spirit("Ice"), Spirit("Ice")), "HARD: match true for same name.");
    check(!hard.match(Spirit("Ice"), Spirit("Light")), "HARD: match false for different names.");
    check(!hard.checkAnswer(), "HARD: checkAnswer on empty belt returns false.");

    hard.insert(Spirit("Water"));
    hard.insert(Spirit("Grass"));
    hard.insert(Spirit("Light"));
    check(!hard.checkAnswer(), "HARD: checkAnswer on partial belt returns false.");
    check(!hard.isSolved(), "HARD: Partial belt does not solve.");

    check(feedSolves(HARD, { "Water", "Grass", "Light", "Fire", "Earth", "Dark", "Ice" }), "HARD: First listed solution solves.");
    check(feedSolves(HARD, { "Ice", "Dark", "Earth", "Fire", "Light", "Grass", "Water" }), "HARD: Last listed solution solves.");
    check(feedSolves(HARD, { "Dark", "Ice", "Water", "Grass", "Light", "Fire", "Earth" }), "HARD: Solution 4 (Dark Ice Water ...) solves.");
    check(feedSolves(HARD, { "Fire", "Earth", "Dark", "Ice", "Water", "Grass", "Light" }), "HARD: Solution 10 (Fire Earth Dark ...) solves.");

    check(!feedSolves(HARD, { "Water", "Grass", "Light", "Fire", "Earth", "Dark" }), "HARD: Six correct spirits (not full) does not solve.");
    check(!feedSolves(HARD, { "Fire", "Water", "Grass", "Light", "Earth", "Dark", "Ice" }), "HARD: Fire next to Water is wrong.");
    check(!feedSolves(HARD, { "Grass", "Fire", "Water", "Light", "Earth", "Dark", "Ice" }), "HARD: Grass next to Fire is wrong.");
    check(!feedSolves(HARD, { "Ice", "Fire", "Earth", "Dark", "Water", "Grass", "Light" }), "HARD: Ice next to Fire is wrong.");
    check(!feedSolves(HARD, { "Light", "Dark", "Water", "Grass", "Ice", "Earth", "Fire" }), "HARD: Light next to Dark is wrong.");
    check(!feedSolves(HARD, { "Dark", "Grass", "Water", "Light", "Ice", "Earth", "Fire" }), "HARD: Grass next to Dark is wrong.");
    check(!feedSolves(HARD, { "Earth", "Ice", "Water", "Grass", "Light", "Fire", "Dark" }), "HARD: Earth next to Ice is wrong.");
    check(!feedSolves(HARD, { "Earth", "Water", "Grass", "Light", "Ice", "Dark", "Fire" }), "HARD: Earth next to Water is wrong.");
    check(!feedSolves(HARD, { "Water", "Ice", "Grass", "Light", "Fire", "Earth", "Dark" }), "HARD: Grass next to Ice is wrong.");
    check(!feedSolves(HARD, { "Grass", "Water", "Ice", "Dark", "Earth", "Fire", "Light" }), "HARD: Grass not next to Light is wrong.");
    check(feedSolves(HARD, { "Ice", "Dark", "Earth", "Fire", "Light", "Grass", "Water", "Fire" }), "HARD: Valid solution followed by extra input stays solved.");
    check(!feedSolves(HARD, { "Water", "Grass", "Light", "Fire", "Earth", "Dark", "Water" }), "HARD: Missing Ice with a duplicate Water does not solve.");
    check(!feedSolves(HARD, { "Water", "Water", "Water", "Water", "Water", "Water", "Water" }), "HARD: Seven identical spirits never fill the belt.");
    check(!feedSolves(HARD, { "Water", "Grass", "Light", "Fire", "Earth", "Dark", "Ghost" }), "HARD: Unknown spirit does not solve.");

    check(feedSolves(HARD, { "Water", "Water", "Water", "Grass", "Light", "Fire", "Earth", "Dark", "Ice" }), "HARD: Duplicate inserts are ignored and solution still solves.");

    {
        SpiritConveyerBelt belt(HARD);
        feed(belt, { "Water", "Grass", "Light", "Fire", "Earth", "Dark" });
        belt.undo();
        belt.undo();
        feed(belt, { "Earth", "Dark", "Ice" });
        check(belt.isSolved(), "HARD: Two undos remove the two most recent spirits.");
    }

    {
        SpiritConveyerBelt belt(HARD);
        feed(belt, { "Water", "Grass", "Light", "Fire" });
        belt.undo();
        feed(belt, { "Earth", "Dark", "Ice" });
        check(!belt.isSolved(), "HARD: Undo leaves belt one short, so it does not solve.");
    }

    {
        SpiritConveyerBelt belt(HARD);
        belt.insert(Spirit("Water"));
        belt.undo();
        feed(belt, { "Water", "Grass", "Light", "Fire", "Earth", "Dark", "Ice" });
        check(belt.isSolved(), "HARD: A spirit can be reinserted after undo.");
    }

    {
        SpiritConveyerBelt belt(HARD);
        feed(belt, { "Ice", "Dark", "Earth" });
        for (int i = 0; i < 10; i++) belt.undo();
        feed(belt, { "Ice", "Dark", "Earth", "Fire", "Light", "Grass", "Water" });
        check(belt.isSolved(), "HARD: Undoing past empty is safe and belt still works.");
    }

    {
        SpiritConveyerBelt belt(HARD);
        feed(belt, { "Water", "Grass", "Light", "Fire", "Earth", "Ice", "Dark" });
        check(!belt.isSolved(), "HARD: Wrong full answer is not solved.");
        feed(belt, { "Water", "Grass", "Light", "Fire", "Earth", "Dark", "Ice" });
        check(belt.isSolved(), "HARD: Belt resets after a wrong answer and accepts a correct one.");
    }

    {
        SpiritConveyerBelt belt(HARD);
        feed(belt, { "Water", "Grass", "Light", "Fire", "Earth", "Dark", "Ice" });
        feed(belt, { "Ice", "Dark", "Earth", "Fire", "Light", "Grass", "Water" });
        check(belt.isSolved(), "HARD: Two solutions back to back stay solved.");
    }

    {
        SpiritConveyerBelt belt(HARD);
        feed(belt, { "Water", "Grass", "Light", "Fire", "Earth", "Dark", "Ice" });
        feed(belt, { "Fire", "Water", "Grass", "Light", "Earth", "Dark", "Ice" });
        check(belt.isSolved(), "HARD: Solved state persists after a later wrong answer.");
    }

    sweepAllPermutations(HARD, { "Fire", "Water", "Grass", "Earth", "Ice", "Light", "Dark" }, 20, "HARD sweep");


    cout << "\n=================\n";
    cout << "SUMMARY\n";
    cout << "=================\n";
    cout << "Passed: " << passed << endl;
    cout << "Failed: " << failed << endl;

    return failed == 0 ? 0 : 1;
}
#include <iostream>
#include "../src/chambers/BridgeBuilder.h"

using namespace std;

int passCount = 0;
int failCount = 0;

// Prints PASS or FAIL for one check and keeps the totals
void check(const char* name, bool condition)
{
	if (condition)
	{
		cout << "PASS: " << name << endl;
		passCount++;
	}
	else
	{
		cout << "FAIL: " << name << endl;
		failCount++;
	}
}

int main()
{
	typedef BridgeBuilder BB;

	// ---------- Before the level starts ----------
	BB game;
	check("new game is notStarted", game.status() == BB::Status::notStarted);
	check("buildBridge before start is refused", !game.buildBridge(0, 1));
	check("reason is notStarted", game.lastResult() == BB::Result::notStarted);
	check("hasRoute false before start", !game.hasRoute());

	// ---------- startLevel validation ----------
	check("1 island rejected", !game.startLevel(1, 0, 0, 5, BB::Difficulty::easy));
	check("start == goal rejected", !game.startLevel(5, 2, 2, 5, BB::Difficulty::easy));
	check("start out of range rejected", !game.startLevel(5, 7, 1, 5, BB::Difficulty::easy));
	check("goal out of range rejected", !game.startLevel(5, 0, 9, 5, BB::Difficulty::easy));
	check("0 max moves rejected", !game.startLevel(5, 0, 4, 0, BB::Difficulty::easy));
	check("still notStarted after bad setups", game.status() == BB::Status::notStarted);

	// ---------- Valid level: 5 islands, start 0, goal 4, 6 moves ----------
	check("valid startLevel accepted", game.startLevel(5, 0, 4, 6, BB::Difficulty::easy));
	check("status is playing", game.status() == BB::Status::playing);
	check("island total is 5", game.islandTotal() == 5);
	check("start island is 0", game.startIslandId() == 0);
	check("goal island is 4", game.goalIslandId() == 4);
	check("difficulty is easy", game.difficulty() == BB::Difficulty::easy);
	check("moves start at 0", game.moveCount() == 0);
	check("max moves is 6", game.maxMoveCount() == 6);
	check("moves left is 6", game.movesLeft() == 6);
	check("second startLevel refused", !game.startLevel(5, 0, 4, 6, BB::Difficulty::easy));

	// ---------- Level layout ----------
	check("first switch has id 0", game.addSwitch() == 0);
	check("site 0-1", game.addBridgeSite(0, 1));
	check("site 1-2", game.addBridgeSite(1, 2));
	check("site 2-4 locked by switch 0", game.addBridgeSite(2, 4, 0));
	check("site 1-3", game.addBridgeSite(1, 3));
	check("site 3-4", game.addBridgeSite(3, 4));
	check("duplicate site (1-0) refused", !game.addBridgeSite(1, 0));
	check("unknown switch refused", !game.addBridgeSite(0, 2, 5));
	check("reason is invalidSwitch", game.lastResult() == BB::Result::invalidSwitch);
	check("site to itself refused", !game.addBridgeSite(2, 2));
	check("bridge 2-4 starts blocked", game.isBridgeBlocked(2, 4));
	check("blocked works in reverse order", game.isBridgeBlocked(4, 2));
	check("bridge 0-1 is not blocked", !game.isBridgeBlocked(0, 1));

	// ---------- Invalid moves cost nothing ----------
	check("build without a site refused", !game.buildBridge(0, 2));
	check("reason is noBridgeSite", game.lastResult() == BB::Result::noBridgeSite);
	check("build blocked bridge refused", !game.buildBridge(2, 4));
	check("reason is bridgeBlocked", game.lastResult() == BB::Result::bridgeBlocked);
	check("build invalid island refused", !game.buildBridge(0, 9));
	check("reason is invalidIsland", game.lastResult() == BB::Result::invalidIsland);
	check("remove unbuilt bridge refused", !game.removeBridge(1, 2));
	check("reason is notBuilt", game.lastResult() == BB::Result::notBuilt);
	check("invalid switch refused", !game.activateSwitch(7));
	check("invalid moves used no moves", game.moveCount() == 0);

	// ---------- Build and remove ----------
	check("build 0-1 accepted", game.buildBridge(0, 1));
	check("0-1 is built", game.isBridgeBuilt(0, 1));
	check("built works in reverse order", game.isBridgeBuilt(1, 0));
	check("one move used", game.moveCount() == 1);
	check("no route yet", !game.hasRoute());
	check("build 0-1 again refused", !game.buildBridge(1, 0));
	check("reason is alreadyBuilt", game.lastResult() == BB::Result::alreadyBuilt);
	check("refused move not counted", game.moveCount() == 1);
	check("remove 0-1 accepted", game.removeBridge(0, 1));
	check("0-1 no longer built", !game.isBridgeBuilt(0, 1));
	check("remove counted as a move", game.moveCount() == 2);
	check("rebuild 0-1 accepted", game.buildBridge(0, 1));
	check("three moves used", game.moveCount() == 3);

	// ---------- Switch ----------
	check("switch 0 starts off", !game.isSwitchOn(0));
	check("activate switch 0 accepted", game.activateSwitch(0));
	check("switch 0 is on", game.isSwitchOn(0));
	check("bridge 2-4 unlocked", !game.isBridgeBlocked(2, 4));
	check("unlocking does not build it", !game.isBridgeBuilt(2, 4));
	check("switch counted as a move", game.moveCount() == 4);
	check("activate again refused", !game.activateSwitch(0));
	check("reason is switchAlreadyOn", game.lastResult() == BB::Result::switchAlreadyOn);
	check("moves still 4", game.moveCount() == 4);

	// ---------- Win on the very last move ----------
	check("build 1-2 accepted", game.buildBridge(1, 2));
	check("still playing, no route", game.status() == BB::Status::playing && !game.hasRoute());
	check("build 4-2 accepted (reverse order)", game.buildBridge(4, 2));
	check("route 0-1-2-4 found", game.hasRoute());
	check("status is won on the last move", game.status() == BB::Status::won);
	check("isComplete is true", game.isComplete());
	check("no moves left", game.movesLeft() == 0);
	check("moves after win refused", !game.buildBridge(1, 3));
	check("reason is gameOver", game.lastResult() == BB::Result::gameOver);

	// ---------- Reset ----------
	game.reset();
	check("reset: status playing", game.status() == BB::Status::playing);
	check("reset: moves 0", game.moveCount() == 0);
	check("reset: 0-1 removed", !game.isBridgeBuilt(0, 1));
	check("reset: 2-4 locked again", game.isBridgeBlocked(2, 4));
	check("reset: switch off again", !game.isSwitchOn(0));
	check("reset: no route", !game.hasRoute());
	check("reset: layout kept", game.islandTotal() == 5 && game.maxMoveCount() == 6);

	// ---------- Lose by wasting all moves ----------
	game.buildBridge(0, 1);
	game.removeBridge(0, 1);
	game.buildBridge(0, 1);
	game.removeBridge(0, 1);
	game.buildBridge(0, 1);
	check("5 moves used, still playing", game.status() == BB::Status::playing);
	game.removeBridge(0, 1);
	check("status is lost after 6 moves", game.status() == BB::Status::lost);
	check("lost counts as complete", game.isComplete());
	check("moves after loss refused", !game.buildBridge(0, 1));

	// ---------- Reset after a loss, then win without the switch ----------
	game.reset();
	check("reset after loss: playing", game.status() == BB::Status::playing);
	game.buildBridge(0, 1);
	game.buildBridge(1, 3);
	check("two moves, no route yet", !game.hasRoute());
	game.buildBridge(3, 4);
	check("route 0-1-3-4 found", game.hasRoute());
	check("won with 3 moves", game.status() == BB::Status::won && game.moveCount() == 3);

	// ---------- Second level: hard, 1 move only ----------
	BB hard;
	check("hard level started", hard.startLevel(3, 0, 2, 1, BB::Difficulty::hard));
	check("difficulty is hard", hard.difficulty() == BB::Difficulty::hard);
	hard.addBridgeSite(0, 1);
	hard.addBridgeSite(1, 2);
	check("build 0-1 uses the only move", hard.buildBridge(0, 1));
	check("lost because the limit is reached", hard.status() == BB::Status::lost);

	cout << endl;
	cout << "Passed: " << passCount << "  Failed: " << failCount << endl;
	return failCount == 0 ? 0 : 1;
}
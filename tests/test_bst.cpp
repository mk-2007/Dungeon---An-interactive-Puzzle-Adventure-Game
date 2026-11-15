#include <iostream>
#include <sstream>
#include <string>

#include "../src/ds/BST.h"

using namespace std;

bool allPassed = true;

void check(bool condition, const string& name)
{
	if (condition)
	{
		cout << "PASS: " << name << endl;
	}
	else
	{
		cout << "FAIL: " << name << endl;
		allPassed = false;
	}
}

string inorderOutput(const BST& tree)
{
	ostringstream output;
	tree.inorder(output);
	return output.str();
}

int main()
{
	BST tree;

	check(tree.root() == nullptr, "New tree has no root");
	check(tree.height() == -1, "Empty tree height is -1");
	check(!tree.contains(10), "Search returns false for an empty tree");
	check(!tree.remove(10), "Removing from an empty tree returns false");

	tree.insert(50);
	tree.insert(30);
	tree.insert(70);
	tree.insert(20);
	tree.insert(40);
	tree.insert(60);
	tree.insert(80);
	tree.insert(30);

	check(tree.root()->data == 50, "First inserted value is the root");
	check(tree.contains(40), "Search finds an existing value");
	check(!tree.contains(90), "Search returns false for a missing value");
	check(tree.height() == 2, "Height counts edges from the root");
	check(inorderOutput(tree) == "20 30 40 50 60 70 80\n",
		"In-order traversal is sorted and ignores duplicate keys");

	check(tree.remove(20), "Leaf node is removed");
	check(!tree.contains(20), "Removed leaf is no longer present");

	check(tree.remove(30), "Node with one child is removed");
	check(tree.contains(40), "One-child removal preserves the child");

	check(tree.remove(70), "Node with two children is removed");
	check(!tree.contains(70), "Removed two-child node is no longer present");
	check(tree.contains(60) && tree.contains(80),
		"Two-child removal preserves both subtrees");

	check(tree.remove(50), "Root with two children is removed");
	check(tree.root()->data == 60, "Root is replaced with its in-order successor");
	check(inorderOutput(tree) == "40 60 80\n",
		"Traversal remains sorted after removals");
	check(!tree.remove(50), "Removing a missing value returns false");

	tree.clear();
	check(tree.root() == nullptr, "Clear removes the root");
	check(tree.height() == -1, "Cleared tree has empty height");
	check(inorderOutput(tree) == "\n", "Cleared tree has an empty traversal");

	tree.insert(5);
	check(tree.contains(5), "Tree can be reused after clear");
	tree.clear();

	return allPassed ? 0 : 1;
}

#pragma once

#include <iostream>
#include <ostream>

class BST
{
public:
	struct Node
	{
		int data;
		Node* left;
		Node* right;

		explicit Node(int value) : data(value), left(nullptr), right(nullptr) {}
	};

	BST() : rootNode(nullptr) {}

	~BST()
	{
		clear();
	}

	BST(const BST&) = delete;
	BST& operator=(const BST&) = delete;

	void insert(int value)
	{
		rootNode = insertNode(rootNode, value);
	}

	bool remove(int value)
	{
		bool removed = false;
		rootNode = removeNode(rootNode, value, removed);
		return removed;
	}

	bool contains(int value) const
	{
		return containsNode(rootNode, value);
	}

	int height() const
	{
		return getHeight(rootNode);
	}

	Node* root() const
	{
		return rootNode;
	}

	void clear()
	{
		clearNodes(rootNode);
		rootNode = nullptr;
	}

	void inorder(std::ostream& output = std::cout) const
	{
		bool first = true;
		writeInorder(rootNode, output, first);
		output << '\n';
	}

private:
	Node* rootNode;

	static Node* insertNode(Node* node, int value)
	{
		if (node == nullptr)
		{
			return new Node(value);
		}

		if (value < node->data)
		{
			node->left = insertNode(node->left, value);
		}
		else if (value > node->data)
		{
			node->right = insertNode(node->right, value);
		}

		return node;
	}

	static Node* removeNode(Node* node, int value, bool& removed)
	{
		if (node == nullptr)
		{
			return nullptr;
		}

		if (value < node->data)
		{
			node->left = removeNode(node->left, value, removed);
		}
		else if (value > node->data)
		{
			node->right = removeNode(node->right, value, removed);
		}
		else
		{
			removed = true;

			if (node->left == nullptr)
			{
				Node* rightChild = node->right;
				delete node;
				return rightChild;
			}

			if (node->right == nullptr)
			{
				Node* leftChild = node->left;
				delete node;
				return leftChild;
			}

			Node* successor = node->right;
			while (successor->left != nullptr)
			{
				successor = successor->left;
			}

			node->data = successor->data;
			bool successorRemoved = false;
			node->right = removeNode(node->right, successor->data, successorRemoved);
		}

		return node;
	}

	static bool containsNode(const Node* node, int value)
	{
		if (node == nullptr)
		{
			return false;
		}

		if (value == node->data)
		{
			return true;
		}

		return value < node->data
			? containsNode(node->left, value)
			: containsNode(node->right, value);
	}

	static int getHeight(const Node* node)
	{
		if (node == nullptr)
		{
			return -1;
		}

		int leftHeight = getHeight(node->left);
		int rightHeight = getHeight(node->right);
		return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
	}

	static void clearNodes(Node* node)
	{
		if (node == nullptr)
		{
			return;
		}

		clearNodes(node->left);
		clearNodes(node->right);
		delete node;
	}

	static void writeInorder(const Node* node, std::ostream& output, bool& first)
	{
		if (node == nullptr)
		{
			return;
		}

		writeInorder(node->left, output, first);
		if (!first)
		{
			output << ' ';
		}
		output << node->data;
		first = false;
		writeInorder(node->right, output, first);
	}
};

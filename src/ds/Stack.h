#pragma once
#include <iostream>
using namespace std;

template<class T> class Stack
{
public:
	T* Arr;
	int capacity;
	int top;
	
	Stack()
	{
		top = -1;
		capacity = 100;
		Arr = new T[capacity];
	}

	~Stack() { delete[] Arr; }

	Stack(const Stack&) = delete;
		Stack & operator= (const Stack&) = delete;

	bool isEmpty() const { return top == -1; }

	bool isFull() const { return top == capacity - 1; }

	bool push(const T& item)
	{
		if (isFull()) { return false; }
		Arr[++top] = item;
		return true;
	}

	bool pop(T& out)
	{
		if (isEmpty()) { return false; }
		out = Arr[top--];
		return true;
	}

	bool peek(T& out) const
	{
		if(isEmpty()) { return false; }
		out = Arr[top];
		return true;
	}

	void clear()
	{
		top = -1;
	}

	int size() const { return top + 1; }
};
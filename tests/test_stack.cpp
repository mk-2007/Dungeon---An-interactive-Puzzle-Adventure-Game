#include <iostream>
#include "../src/ds/Stack.h"
#include <string>
using namespace std;

void check(bool condition, string name)
{
	if (condition) cout << "PASS: " << name << endl;
	else cout << "FAIL: " << name << endl;
}

int main()
{
	Stack <int> s;
	int temp;
	int arr[3];


	check(s.isEmpty(), "new Stack is Empty.");
	check(!s.pop(temp), "pop() on empty returns false");
	check(!s.peek(temp), "peek() on empty returns false");
	s.push(1);
	check(s.size() == 1, "Size is 1 after 1 push.");

	
	s.peek(temp);
	check(s.size() == 1, "Peek doesn't remove element.");
	check(temp == 1, "Peek returns last added element.");

	s.push(2);
	s.push(3);

	check(s.size() == 3, "size() works correctly with push().");

	
	s.pop(arr[0]);
	s.pop(arr[1]);
	s.pop(arr[2]);
	
	check(s.size() == 0, "size() works correctly with pop().");
	check(arr[0] == 3 && arr[1] == 2 && arr[2] == 1, "Has LIFO nature");
	check(s.isEmpty(), "isEmpty() works with pop()");
	s.push(1);
	s.push(2);
	s.push(3);
	s.push(4);

	check(!s.push(5), "Doesn't push when full");
	s.peek(temp);
	check(s.isFull(), "IsFull() is true on full stack");
	check(temp == 4, "Pushing on full stack doesn't overwrite anything.");
	check(s.size() == 4, "Pushing on full stack doesn't affect size.");

	s.clear();

	check(s.size() == 0, "clear() and size() work correctly together");
	check(s.isEmpty(), "Stack is Empty after clear()");

	s.push(1);
	s.peek(temp);
	check(temp == 1, "Stack works after clear()");
	s.push(2);
	s.push(3);

	s.pop(temp);
	s.push(3);
	s.push(4);

	s.pop(temp);
	check(temp == 4, "pop() returns element pushed by an earlier push()");
	s.pop(temp);
	s.pop(temp);
	s.pop(temp);

	s.push(1);
	s.push(2);
	s.push(3);
	s.push(4);

	s.pop(temp);
	s.pop(temp);
	s.pop(temp);
	s.pop(temp);

	check(s.size() == 0, "No off by one errors with mixed use of pop() and push()");


}
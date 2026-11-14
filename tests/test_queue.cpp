#include <iostream>
#include "../src/ds/Queue.h"
#include <string>
using namespace std;

void check(bool condition, string name)
{
	if (condition) cout << "PASS: " << name << endl;
	else cout << "FAIL: " << name << endl;
}

int main()
{
	Queue <int> q;
	int temp;

	check(q.capacity == 4, "No parameters constructor working");
	check(q.isEmpty(), "Is empty on new queue");
	check(q.getSize() == 0, "getSize is 0 on new queue");
	check(!q.peek(temp), "Peek doesn't work on new queue");
	check(!q.dequeue(temp), "dequeue doesn't work on new queue");

	q.enqueue(1);

	check(q.getSize() == 1, "Enqueue updates size");
	q.enqueue(2);
	q.enqueue(3);
	q.enqueue(4);

	check(!q.enqueue(5), "No Queue overflow");
	check(q.isFull(), "isFull() works correctly with enqueue()");
	check(q.getSize() == 4, "Size does not update when enqueue() returns false");
	q.dequeue(temp);
	bool tempb = temp == 1;
	q.dequeue(temp);
	check(tempb && temp == 2, "FIFO nature preserved");
	check(q.getSize() == 2, "dequeue() updates size");

	q.enqueue(5);
	q.peek(temp);
	check(temp == 3, "peek returns front");
	check(q.getSize() == 3, "Peek does not affect size");

	q.dequeue(temp);
	q.dequeue(temp);
	q.dequeue(temp);

	check(q.getSize() == 0, "dequeue() can empty queue");

	q.enqueue(1);
	check(q.enqueue(2), "Enqueue works after dequeue");
	q.dequeue(temp);

	q.clear();
	check(q.getSize() == 0, "Clear resets size");
	check(!q.dequeue(temp), "Clear empties queue");

	check(q.enqueue(1), "Clear works with enqueue()");



	


}
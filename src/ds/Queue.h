#pragma once

template<class T> class QNode
{
public:
	T data;
	QNode* next;

	QNode(T key)
	{
		data = key;
		next = nullptr;
	}

};

template <class T> class Queue
{
	public:
		QNode<T>* front;
		QNode<T>* rear;
		int capacity;
		int size;

		Queue(int cap=4)
		{
			size = 0;
			capacity = cap;
			front = rear = nullptr;
		}
		~Queue() { clear(); }
		Queue(const Queue&) = delete;
		Queue& operator=(const Queue&) = delete;
		int getSize() const { return size; }
		void clear()
		{
			while (front != nullptr)
			{
				QNode<T>* toDelete = front;
				front = front->next;
				delete toDelete;
			}

			front = rear = nullptr;
			size = 0;
		}

		bool isFull() const { return size == capacity; }
		bool isEmpty() const { return size == 0; }

		bool enqueue(T key)
		{
			if (isFull()) return false;

			QNode<T>* toInsert = new QNode<T>(key);
			if (isEmpty())
			{
				front = rear = toInsert;
			}
			else
			{
				rear->next = toInsert;
				rear = rear->next;
			}
			size++;
			return true;
		}

		bool dequeue(T& out)
		{
			if (isEmpty()) return false;
			out = front->data;

			QNode<T>* todequeue = front;
			
			front = front->next;
			size--;
			delete todequeue;

			if (size==0) { front = rear = nullptr; }
			return true;
		}

		bool peek(T& out)
		{
			if (isEmpty()) return false;
			out = front->data;
			return true;
		}
};
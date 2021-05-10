#pragma once

#include "Debug/Logging.h"

template<class T>
struct node
{
	T data;
	node* next = nullptr;
};


template<class T>
class LinkedList
{
public:
	LinkedList() : head(nullptr), tail(nullptr) { };

	void insert_back(T data);
	bool remove(T data);

	T& get(int i);

	int size();

private:
	node<T>* head;
	node<T>* tail;
};


template <class T>
T& LinkedList<T>::get(int i)
{
	int index = 0;
	node<T>* current_node = head;

	while (current_node)
	{
		if (index == i)
		{
			return current_node->data;
		}
		else
		{
			current_node = current_node->next;
			index++;
		}
	}

	DebugPrint(Error, "Index %d out of bounds, linked list has size %d\n", i, size());
	return head->data;
}


template <class T>
void LinkedList<T>::insert_back(T data)
{
	// Create a new node to be inserted at the back
	node<T>* newNode = new node<T>();
	newNode->data = data;

	if (!head)
	{
		head = newNode;
		tail = newNode;
	}
	else
	{
		// new node becomes the tail
		tail->next = newNode;
		tail = newNode;
	}
}


template <class T>
bool LinkedList<T>::remove(T data)
{
	node<T>* current = head;
	node<T>* previous = nullptr;

	while (current)
	{
		if (current->data == data)
		{
			if (previous)
			{
				// remove the link to this node
				previous->next = current->next;

				// if we're removing the tail, set the previous node as the new tail
				if (current == tail)
				{
					tail = previous;
					ASSERT(Error, tail->next == nullptr, "The new linked list tail isn't the tail, there is a problem\n");
				}
			}
			else // head
			{
				// the next node is now the new head
				head = current->next;

				// if we have no nodes left, our list is empy so also clear the tail
				if (!head)
				{
					tail = nullptr;
				}
			}

			return true;
		}

		// Increment current and previous nodes
		previous = current;
		current = current->next;
	}

	// No node with the given data was found
	return false;
}


template <class T>
int LinkedList<T>::size()
{
	if (!head)
	{
		return 0;
	}
	else
	{
		intptr_t diff = tail - head;
		return (int)(diff / sizeof(node<T>*)) + 1;
	}
}
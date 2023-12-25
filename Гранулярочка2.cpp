
#include <iostream>
#include<mutex>
#include<thread>
#include<list>
#include<queue>
using namespace std;



class FineGrainedQueue
{
		public:
			struct Node
				{
						int value;
						Node* next;
						std::mutex* node_mutex;
				};
				FineGrainedQueue() : head(nullptr), tail(nullptr)
						{
							size = 0;
						}
			Node* head; Node* tail; 
			int size = 0;
			std::mutex* queue_mutex;
	

void remove(int value)
{
	Node* prev, * cur;
	queue_mutex->lock();
	prev = this->head;
	cur = this->head->next;
	prev->node_mutex->lock();
	queue_mutex->unlock();
	if (cur) { cur->node_mutex->lock(); }
	while (cur)
	{
		if (cur->value == value)
		{
			prev->next = cur->next;
			prev->node_mutex->unlock();
			cur->node_mutex->unlock();
			delete cur;
			return;
		}
		Node* old_prev = prev;
		prev = cur; cur = cur->next;
		old_prev->node_mutex->unlock();
		if (cur) { cur->node_mutex->lock(); }
	}
	prev->node_mutex->unlock();
}
void insertInTOend(int intovalue)
		{
	        queue_mutex->lock();
			if (size == 0)
			{	head->node_mutex->lock();
				head = tail = new Node;
				head->node_mutex = new std::mutex();
				head->value = intovalue;
				head->next = NULL;
				head->node_mutex->unlock();
			}
			else
			{
		
				Node* tail, *head = new Node;
				Node* old_head = new Node;
				head->node_mutex->lock();
				tail->node_mutex->lock();
				old_head->node_mutex->lock();
				old_head = head->next;
				head->next = tail;
				tail->next = NULL;
				head = old_head;
				head->value = intovalue;
				head->node_mutex->unlock();
				tail->node_mutex->unlock();
				old_head->node_mutex->unlock();
			}
			size++;
			queue_mutex->unlock();
		}

void DOPinsertINTOmiddle(int intovalue, int pos)
	{
		int curPosition=1;
		Node* cur, * newNode = new Node;
		newNode->node_mutex= new std::mutex();
		queue_mutex->lock();
		newNode->value = intovalue;
		cur = this->head;
		queue_mutex->unlock();
		if (cur) { cur->node_mutex->lock(); }
			while (cur)
				{
				newNode->node_mutex->lock();
						if (curPosition == pos)
							{
								newNode->next = cur->next;
								cur->next = newNode;
								newNode->node_mutex->unlock();

								return;
							}
						else { 
							newNode->node_mutex->lock();
							Node* old_cur = cur;
							old_cur->node_mutex->lock();
							cur = cur->next;
							newNode->next = old_cur->next;
							old_cur->next = newNode;
							newNode->node_mutex->unlock();
							cur->node_mutex->unlock();
							old_cur->node_mutex->unlock();
							return;
							}
						curPosition++;
						queue_mutex->unlock();
				}	
	}

void showGrainedQueue()
	{
		Node* cur = head;
			while (cur->next != nullptr)
				{
				std::cout << cur->value << " ";
				cur = cur->next;
				}
	std::cout << cur->value << std::endl;
	}
};

int main()
{
	FineGrainedQueue A;
	A.insertInTOend(8);
	A.insertInTOend(5);
	A.insertInTOend(6);
	A.insertInTOend(12);
	A.insertInTOend(32);

	A.showGrainedQueue();

	std::thread t1(&FineGrainedQueue::DOPinsertINTOmiddle, A, 20, 3);
	std::thread t2(&FineGrainedQueue::DOPinsertINTOmiddle, A, 30, 7);

	t1.join();
	t2.join();

	A.showGrainedQueue();

	return 0;
}


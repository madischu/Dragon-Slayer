#include "EnemyQueue.h"
#include <iostream>

EnemyQueue::EnemyQueue()
{
    front = nullptr;
    rear = nullptr;
    count = 0;
}

EnemyQueue::~EnemyQueue()
{
    clear();
}

void EnemyQueue::enqueue(const Monster& monster)
{
    Node* newNode = new Node(monster);

    if (rear == nullptr)
    {
        front = newNode;
        rear = newNode;
    }
    else
    {
        rear->next = newNode;
        rear = newNode;
    }

    count++;
}

Monster EnemyQueue::dequeue()
{
    if (isEmpty())
    {
        return Monster("None", 0, 0, "none");
    }

    Node* temp = front;
    Monster removedMonster = temp->monster;

    front = front->next;

    if (front == nullptr)
    {
        rear = nullptr;
    }

    delete temp;
    count--;

    return removedMonster;
}

Monster& EnemyQueue::peek()
{
    return front->monster;
}

bool EnemyQueue::isEmpty() const
{
    return front == nullptr;
}

int EnemyQueue::size() const
{
    return count;
}

void EnemyQueue::clear()
{
    while (!isEmpty())
    {
        dequeue();
    }
}

void EnemyQueue::buildCaveWave()
{
    clear();

    enqueue(Monster("Slime", 2, 15, "blunt"));
    enqueue(Monster("Fanged Beast", 8, 60, "sharp"));
}

void EnemyQueue::buildCaveWaveForWeapon(const Weapon& weapon)
{
    clear();

    if (weapon.getType() == "sharp")
    {
        enqueue(Monster("Fanged Beast", 8, 60, "sharp"));
        enqueue(Monster("Fanged Beast", 8, 60, "sharp"));
    }
    else
    {
        enqueue(Monster("Slime", 2, 15, "blunt"));
        enqueue(Monster("Slime", 2, 15, "blunt"));
    }
}

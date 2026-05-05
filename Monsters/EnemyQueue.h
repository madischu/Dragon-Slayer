#ifndef ENEMYQUEUE_H
#define ENEMYQUEUE_H

#include "Monster.h"
#include "Weapon.h"

class EnemyQueue
{
private:
    struct Node
    {
        Monster monster;
        Node* next;

        Node(const Monster& newMonster)
            : monster(newMonster), next(nullptr)
        {
        }
    };

    Node* front;
    Node* rear;
    int count;

public:
    EnemyQueue();
    ~EnemyQueue();

    void enqueue(const Monster& monster);
    Monster dequeue();
    Monster& peek();

    bool isEmpty() const;
    int size() const;
    void clear();

    void buildCaveWave();
    void buildCaveWaveForWeapon(const Weapon& weapon);
};

#endif

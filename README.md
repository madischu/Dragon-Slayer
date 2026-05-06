# Dragon-Slayer

## Program Description
This project expands on my existing C++ text-based adventure game called Dragon Slayer. In the original game, the player explores locations, buys weapons, fights monsters, and attempts to defeat a dragon. The expanded version will solve the problem of managing a larger and more dynamic game world by using data structures to organize inventory, enemies, quests, and movement more effectively.

The updated game will address the problem of managing a more complex game world by incorporating dynamic data structures. The enhances version will include a more advanced inventory system, a larger interconnected map, and more structured combat encounters. Instead of storing game data in only fixed arrays and simple variables, the new version will use data structures such as linked lists, queues, stacks, trees, or graphs. This will make the game more interactive and better organized while also showing how data structures can be applied in a practical and fun setting.

**Unique Features Included:**

· Based on an existing custom-built game

· Integrates multiple data structures in one system

· Expands gameplay with inventory, map navigation, and combat systems

· Demonstrates practical application of data structures in game design

· Moves beyond static data storage to dynamic scalable systems

## User Interface Parameters
The game can still be command-line based, which fits the assignment well, though graphics may still be included if time allows. The menu system would simply become more detailed.

Parameters:

· Main menu choice

· Movement choice

· Combat action choice

· Inventory selection

· Item use or equip choice

· Quest menu choice

· Player name

· Save/quit option

## Structures & Algorithms
**Structures**

Graph (World Map)

· Nodes represent locations (town, caves, lair, etc.)

· Edges represent paths between locations

· Enables flexible navigation and expansion of the game world

Linked List (Inventory System)

· Stores player items dynamically (weapons, potions, etc.)

· Allows efficient insertion and removal

· Removes limitation of fixed-size arrays

Queue (Enemy Encounters / Combat Order)

· Manages multiple enemies in sequence

· Ensures first-in, first-out combat order

· Useful for wave-based encounters

**Algorithms**

· Traversal of linked list for inventory display and management

· Enqueue and dequeue operations for enemy handling

· Graph traversal for movement between locations

· Search operations for locating items or enemies

## Compile and Run

**Command Line Version**

*cmake --build build --target DragonSlayer*

*.\build\Debug\DragonSlayer.exe*

**GUI Version**

*cmake --build build --target DragonSlayerGui*

*.\build\Debug\DragonSlayerGui.exe*

## Program Execution Evidence
<img width="1323" height="658" alt="image" src="https://github.com/user-attachments/assets/7ffd9e90-dee4-4dcc-8323-329207933064" />

<img width="448" height="1048" alt="image" src="https://github.com/user-attachments/assets/a59b3c5f-2d8f-4e91-a3b5-0ed72a207876" />

<img width="1103" height="686" alt="image" src="https://github.com/user-attachments/assets/8fb49f52-7ad7-4b2f-99a4-b88ae67aebd5" />



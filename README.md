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

## Detailed Psuedocode
```
PROGRAM Dragon Slayer

START program

CREATE Game object
CALL game.start()

CLASS Game

FUNCTION start()
    DISPLAY welcome message
    CREATE Player object
    SET gameRunning to true

    WHILE gameRunning is true AND player health is greater than 0
        DISPLAY main menu
        DISPLAY player choices:
            1. Go to store
            2. Go to caves
            3. Go to dragon's lair
            4. View player stats
            5. Quit game

        GET user choice

        IF choice is 1
            CALL Store menu
        ELSE IF choice is 2
            CALL Caves menu
        ELSE IF choice is 3
            CALL Dragon's Lair menu
        ELSE IF choice is 4
            CALL player.displayStats()
        ELSE IF choice is 5
            SET gameRunning to false
        ELSE
            DISPLAY invalid input message
        END IF
    END WHILE

    IF player health is 0
        DISPLAY game over message
    ELSE
        DISPLAY goodbye message
    END IF
END FUNCTION
```
```
CLASS Player

FUNCTION Player()
    SET xp to 0
    SET health to 100
    SET gold to 25
    SET currentWeaponIndex to 0
    ADD Stick weapon to weapons list
END FUNCTION

FUNCTION takeDamage(damage)
    SUBTRACT damage from health

    IF health is less than 0
        SET health to 0
    END IF
END FUNCTION

FUNCTION addHealth(amount)
    ADD amount to health

    IF health is greater than 300
        SET health to 300
    END IF
END FUNCTION

FUNCTION addGold(amount)
    ADD amount to gold
END FUNCTION

FUNCTION subtractGold(amount)
    SUBTRACT amount from gold

    IF gold is less than 0
        SET gold to 0
    END IF
END FUNCTION

FUNCTION addXP(amount)
    ADD amount to xp
END FUNCTION

FUNCTION setCurrentWeaponIndex(index)
    IF index is valid
        SET currentWeaponIndex to index
    END IF
END FUNCTION

FUNCTION getCurrentWeapon()
    RETURN weapon at currentWeaponIndex
END FUNCTION

FUNCTION displayStats()
    DISPLAY health
    DISPLAY gold
    DISPLAY xp
    DISPLAY current weapon
END FUNCTION
```
```
CLASS Store

FUNCTION enterStore(player)
    DISPLAY store menu

    WHILE player stays in store
        DISPLAY available items and weapons
        DISPLAY player gold
        GET player choice

        IF player chooses health item
            CHECK if player has enough gold
            IF yes
                SUBTRACT item cost from player gold
                ADD health to player
            ELSE
                DISPLAY not enough gold
            END IF

        ELSE IF player chooses weapon
            CHECK if player has enough gold
            IF yes
                SUBTRACT weapon cost from player gold
                ADD weapon to player's weapon list
            ELSE
                DISPLAY not enough gold
            END IF

        ELSE IF player chooses to leave
            RETURN to main menu

        ELSE
            DISPLAY invalid choice
        END IF
    END WHILE
END FUNCTION
```
```
CLASS Caves

FUNCTION enterCaves(player)
    DISPLAY cave description
    CREATE or SELECT monster encounter

    IF player can fight selected monster
        CALL CombatSystem.fight(player, monster)
    ELSE
        DISPLAY XP requirement message
        RETURN to main menu
    END IF
END FUNCTION
```
```
CLASS Dragon's Lair

FUNCTION enterDragonsLair(player)
    DISPLAY dragon's lair description

    CREATE Dragon monster

    IF player does not have enough XP
        DISPLAY message that player needs more XP
        RETURN to main menu
    END IF

    IF player does not have required weapon
        DISPLAY message that player needs the correct weapon
        RETURN to main menu
    END IF

    CALL CombatSystem.fight(player, dragon)

    IF dragon is defeated
        DISPLAY winning message
        END game
    ELSE IF player health is 0
        DISPLAY losing message
    END IF
END FUNCTION
```
```
CLASS CombatSystem

FUNCTION fight(player, monster)
    DISPLAY monster name and stats

    WHILE player health > 0 AND monster health > 0
        DISPLAY combat options:
            1. Attack
            2. Dodge
            3. Run

        GET choice

        IF choice is Attack
            GET player's current weapon
            CALCULATE weapon effectiveness against monster

            IF player hits monster
                SUBTRACT damage from monster health
                DISPLAY damage message
            ELSE
                DISPLAY missed attack message
            END IF

            IF monster health <= 0
                CALL player.defeatMonster(monster)
                END fight
            END IF

            MONSTER attacks player
            PLAYER takes damage unless dodge succeeds

        ELSE IF choice is Dodge
            CALL player.dodge(monster)

            IF dodge fails
                MONSTER attacks player
            END IF

        ELSE IF choice is Run
            DISPLAY escape message
            END fight

        ELSE
            DISPLAY invalid choice
        END IF
    END WHILE

    IF player health <= 0
        DISPLAY player defeated message
    END IF
END FUNCTION
```

## UML Diagram
<img width="342" height="855" alt="image" src="https://github.com/user-attachments/assets/15c55ab1-fdfb-45ab-9606-d3447f13a1f0" />

## Use Case Diagram
<img width="510" height="770" alt="image" src="https://github.com/user-attachments/assets/c898e41b-9e66-4024-ad50-712ed8acd36e" />

## Program Execution Evidence
<img width="1323" height="658" alt="image" src="https://github.com/user-attachments/assets/7ffd9e90-dee4-4dcc-8323-329207933064" />

<img width="448" height="1048" alt="image" src="https://github.com/user-attachments/assets/a59b3c5f-2d8f-4e91-a3b5-0ed72a207876" />

<img width="1103" height="686" alt="image" src="https://github.com/user-attachments/assets/8fb49f52-7ad7-4b2f-99a4-b88ae67aebd5" />



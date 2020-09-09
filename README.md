# Project4

Developed with Unreal Engine 4.25+ by:
- Elliot Couvignou (Programmer)
- Nick Hand (Artist)
![Test Image 1](https://cdn.discordapp.com/attachments/140582294950903809/746611723737497700/Rough_BaseChar.jpg)

This passion project is quite far from done. We have already learned a ton about game design workflows and general concepts about optimization, performance, and "just" having good, stable systems to build off of for large scale development. This game has very high ambitions but we hope to finish the basic systems soon and then build on those to reach the game we dreamed about long ago.

# Our Goals:
- Create multiplayer, open-world-esque enviroments filled with NPC's 
- Include a polished Action-styled combat 
- Progression system tied to upgradeable items, equippment, weapons, and abilities. 
- Player freedom over how they want to build/upgrade their abilities via items and skill trees. 

# How To Play (WIP):
- Currently trying to create a release branch with our most stable 

Important: *This game runs on dedicated servers hosted on AWS Gamelift, if you cannot get into the game then the servers most likely aren't up.*



# Implemented Features:
As stated before the goal is to create the foundation of each major system and then build on those to eventaully create the game we want. With that said most of these features below will still be worked on but are listed as features that were worked on to the point of reaching that "base" level. 

- Ability System using Epic's "Gameplay Ability System" plugin.
-- Prebuilt system for creating and editing player attribute sets in multiplayer setting.
-- Allows local prediction and rollback.

- Inventory, Equippment and Weapons
-- Players can pickup items into their inventory to carry with them.
-- Armor and Weapons can be equipped, granting bonus attribute stats.
-- Weight system included although no check and penalty for going overweight. (to be determined)

- AI/NPC Behavior
-- NPC can patrol predetermined points or walk around randomly.
-- Upon seeing players they will begin to chase the player and attack when in range.

- Dedicated Server hosting
-- Uses AWS Gamelift Service for hosting services
-- More on this later on for seamless travel, SQL data storage, etc.


![Test Image 2](https://media.discordapp.net/attachments/140582294950903809/751672646969852015/screenshot080.png?width=1911&height=1075)
# Features to Implement:

- Abilities
-- More abilities for skill trees.
-- Skill Trees
-- Polish animations by using our own.

- Inventory and Items:
-- Currency item(s) and collection in inventory.
-- Consumable items (e.g. Health/Mana Potions).
-- Spawn items on mob death.
-- More Items.

- AI/NPC Behavior:
-- Overall more polish and responsiveness.
-- Increase "Intelligence" through bigger and/or smarter behavior trees.

- Server Stuff:
-- Save/Load Player info into SQL Database (AWS Dynamo DB).
-- Server travel between 

- General:
-- Add Main Menu (need to fix error on joining game session bug)
-- Fix bugs lmao.

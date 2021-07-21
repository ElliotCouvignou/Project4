# Project4

Developed with Unreal Engine 4.26.1+ by:
- Elliot Couvignou (Programmer)
- Nick Hand (Artist)
 

![Test Image 1](https://media.discordapp.net/attachments/140582294950903809/753352645917278228/KL_GemColor_Exploration.jpg)

This passion project is quite far from done. We have already learned a ton about game design workflows and general concepts about optimization, performance, and "just" having good, stable systems to build off of for large scale development. This game has very high ambitions but we hope to finish the basic systems soon and then build on those to reach the game we dreamed about long ago.

# Our Goals:
- Create multiplayer RPG equivalent of risk of rain 2 rougelike gameplay
- Include a polished Action-styled combat 
- Progression system tied to upgradeable items, equippment, weapons, and abilities. 
- Player freedom over how they want to build/upgrade their abilities via items and skill trees. 


# Implemented Features:
As stated before the goal is to create the foundation of each major system and then build on those to eventaully create the game we want. With that said most of these features below will still be worked on but are listed as features that were worked on to the point of reaching that "base" level. 

- Ability System using Epic's "Gameplay Ability System" plugin.
    - Prebuilt system for creating and editing player attribute sets in multiplayer setting.
    - Allows local prediction and rollback.

- Ability Modifiers
    - Dynamically change funcitonality of gameplay abilities (e.g. bleed damage now heals for % dmg done)

- Player Objectives
    - still kinda WIP.

- Inventory, Equippment and Weapons
    - Players can pickup items into their inventory to carry with them.
    - Armor and Weapons can be equipped, granting bonus attribute stats.
    - Weight system included although no check and penalty for going overweight. (to be determined)

- Procedural Item Attribute generation system

- AI/NPC Behavior
-- NPC can patrol predetermined points or walk around randomly.
-- Upon seeing players they will begin to chase the player and attack when in range.


- Dedicated Server hosting
    - Uses AWS Gamelift Service for hosting services
    - More on this later on for seamless travel, SQL data storage, etc.
    - Not necessary for this game (can be listen servers) but I spent like 6 months figuring this stuff out so....


![Test Image 2](https://media.discordapp.net/attachments/140582294950903809/751672646969852015/screenshot080.png?width=1911&height=1075)


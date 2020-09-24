AzerothCore Battle Royale
---------------------

AzerothCore Battle Royale is a custom outdoor pvp experience for azerothCore-wotlk. 

Tons of Players can engage in a massive free for all where there is only one winner. 

Players will battle across Moonglade seeking out protective relics to save them from the deadly infection in the air while attempting to survive from their fellow players. 


Features
----------

- Battle Royale World PvP mode
- Rewards Vendor
- Battle Royale Season Quests
- Battle Royale Teleporter

<a href="http://www.youtube.com/watch?feature=player_embedded&v=vsTW7kQ1CNc
" target="_blank"><img src="http://img.youtube.com/vi/vsTW7kQ1CNc/0.jpg" 
alt="Battle Royale" width="240" height="180" border="10" /></a>



Details
--------

- Customiziable Player limit allows server owners to specify how many players are allowed in the battle royale at once

- PvP kills during the battle royale grant 1 `Bloodshed Token` used as reward currency. 

- Winning a Battle Royale match grants 1 `King's Badge` which is used as premium reward currency.

- Instead of typical zone-style protection, Players will have to interact with relics that appear throughout the match. By interacting with a relic, they will recieve a temporary buff that protects them from the Infection.

- The Infection is Professor Putricide's concoction that has suffocated the air of the battle royale. Without the relic's protection, you'll perish!

- Last player standing wins the battle royale!


How to install
----------------


1. Copy the `Battle_Royale` directory to `/modules` in your azerothcore directory

```
cp -R Battle_Royale/* $AZEROTHCORE_ROOT/modules/
```


where $AZEROTHCORE_ROOT is the root of your azerothcore code.

2. Run the `production.sql` scripts under `./sql/world/base` in your Database for custom reward items, npcs, and quests.

3. Add `OUTDOOR_PVP_BR` value to `OutdoorPvPTypes` enum inside `/server/game/OutdoorPvP/OutdoorPvP.h` and increase the max types by 1.
```
OutdoorPvP.h
enum OutdoorPvPTypes
{
    OUTDOOR_PVP_HP = 1,
    OUTDOOR_PVP_NA = 2,
    OUTDOOR_PVP_TF = 3,
    OUTDOOR_PVP_ZM = 4,
    OUTDOOR_PVP_SI = 5,
    OUTDOOR_PVP_EP = 6,
    OUTDOOR_PVP_GH = 7,
    OUTDOOR_PVP_BR = 8, <-- Here
};

#define MAX_OUTDOORPVP_TYPES 9 <-- Here
```


Contributing
--------------

Feel free to put any PR against this repo, a community driven approach is very encouraged.


Credits
--------

##### Created by [git://pygnode](https://github.com/pygnode/)

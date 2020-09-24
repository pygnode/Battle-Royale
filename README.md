Trinity Battle Royale 
---------------------

## Looking for a AzerothCore compatible version? Use the [azerothcore](https://github.com/pygnode/Battle-Royale/tree/azerothcore) branch. 

<p>
  <a href="https://github.com/pygnode/Battle-Royale"><img alt="GitHub Actions status" src="https://github.com/pygnode/Battle-Royale/workflows/CI/badge.svg"></a>
</p>


Trinity Battle Royale is a custom outdoor pvp experience for TrinityCore 3.3.5a. 

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


1. Copy all the follow directories under `src/server/scripts/Custom` in your trinitycore directory

```
cp -R battle_royal* $TRINITY_ROOT/src/server/scripts/Custom
```


where $TRINITY_ROOT is the root of your trinitycore code.

2. Add the loading function definitions to your `custom_script_loader.cpp` (See uploaded example)

```
...
void AddSC_battle_royal();
void AddSC_battle_royal_rewards();
void AddSC_battle_royal_gamemaster();

void AddCustomScripts()
{
    ...
    AddSC_battle_royal();
    AddSC_battle_royal_gamemaster();
    AddSC_battle_royal_rewards();
}
```

3. Run the `.sql` scripts under `./sql` in your Database for custom reward items, npcs, and quests.

4. Add `OUTDOOR_PVP_BR` value to `OutdoorPvPTypes` enum inside `/server/game/OutdoorPvP/OutdoorPvP.h`
```
OutdoorPvP.h
enum OutdoorPvPTypes
{
    OUTDOOR_PVP_HP = 1,
    OUTDOOR_PVP_NA,
    OUTDOOR_PVP_TF,
    OUTDOOR_PVP_ZM,
    OUTDOOR_PVP_SI,
    OUTDOOR_PVP_EP,
    OUTDOOR_PVP_BR, // <-- Here

    MAX_OUTDOORPVP_TYPES
};
```


Contributing
--------------

Feel free to put any PR against this repo, a community driven approach is very encouraged.


Credits
--------

##### Created by [git://pygnode](https://github.com/pygnode/)

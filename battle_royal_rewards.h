#ifndef TRINITYCORE_BATTLE_ROYAL_REWARDS_H
#define TRINITYCORE_BATTLE_ROYAL_REWARDS_H


#include <string>
#include <vector>
#include <map>

/* REWARD TITLE ITEMS */


enum REWARD_TITLES {
    // Kill Rewards
        BATTLEMASTER = 72,
    BLOODSAIL = 144,
    BLOOD_GUARD = 21,
    BRUTAL_GLADIATOR = 80,
    COMMANDER = 11,
    DEADLY_GLADIATOR = 157,
    FLAME_KEEPER = 76,
    GRUNT = 16,
    KNIGHT_CHAMPION = 9,
    MERRYMAKER = 134,
    VANQUISHER = 163,

    // Win Rewards
        BANE_OF_FALLEN_KING = 174,
    CELESTIAL_DEFENDER = 159,
    CRUSADER = 156,
    CHAMPION = 24

};

enum REWARD_MOUNTS {
    MAGIC_ROOSTER = 65917,
    BREWFEST_RAM = 43900,
    AMANI_WAR_BEAR = 43688,
    ZULIAN_TIGER = 24252,
    FIERY_WARHORSE = 36702,
    WAR_WOLF = 22724,
    WAR_TIGER = 22723,
    DARK_RIDING_TALBUK = 39316,
    SPECTRAL_TIGER = 42777,
    RAVEN_LORD = 41252,
    CELESTIAL_STEED = 75614,
    INVINCIBLE = 72286,
    ASHES_OF_ALAR = 40192,
    BREWFEST_KODO = 49379
};

enum SEASON_SHIRT_EFFECTS {
    SEASON_1_EFFECT = 62579
};

enum REWARD_VENDOR_OPTIONS {
    TITLE_REWARDS = 0,
    MOUNT_REWARDS = 1,
    EXCHANGE_REWARDS = 2,
    ITEM_REWARDS = 3
};


enum REWARD_CURRENCY {
    BLOODSHED_TOKEN = 19063,
    KINGS_BADGE = 1995
};

enum REWARD_TYPE {
    REWARD_TYPE_TITLE = 1,
    REWARD_TYPE_MOUNT = 2,
    REWARD_TYPE_ITEM = 3
};

struct Reward {
    int id;
    std::string name;
    int cost;
    REWARD_CURRENCY currency;
    REWARD_TYPE type;
};

bool compareByName(const Reward &a, const Reward &b)
{
    return a.name < b.name;
}

std::map<int, Reward> _rewards;

const std::vector<Reward> rewards{

    /* So we don't have to use a custom dbc patch, we use unused item IDS */
    /*
     *
     */
//
//    39971, 0, 1, Potion
//    37926, 0, 1, Potion
//    42548, 0, 1, Potion
//    30793, 0, 1, Potion
//    34646, 0, 1, Potion
//    23696, 0, 1, Potion
//    23698, 0, 1, Potion
//    2461, 0, 1, Potion
//    2462, 0, 1, Potion
//    1450, 0, 1, Potion
//    40677, 0, 1, Potion
//    40413, 0, 2, Elixir
//    36845, 0, 2, Elixir
//    46336, 0, 3, Flask
//    34641, 0, 3, Flask
//    49768, 0, 4, Scroll
//    43468, 0, 4, Scroll
//    44572, 0, 5, Food & Drink
//    33822, 0, 5, Food & Drink
//    17199, 0, 5, Food & Drink
    /* Titles */
    {12763, "Battlemaster",                            20, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
    {29402, "Blood Guard",                             25, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
    {34770, "Grunt",                                   5,  BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
    {49863, "Blood Sail Admiral",                      40, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
    {12211, "Brutual Gladiator",                       75, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},

    {20365, "Knight-Champion",                         30, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
    {35710, "Commander",                               20, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
    {23704, "Deadly Gladiator",                        75, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
    {29293, "Flame Keeper",                            10, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
    {4418,  "Merrymaker",                              5,  BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
    {23172, "Vanquisher",                              30, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},

    {8243,  "Bane of the Fallen King",                 10, KINGS_BADGE,     REWARD_TYPE_TITLE},
    {1176,  "Celestial Defender",                      5,  KINGS_BADGE,     REWARD_TYPE_TITLE},
    {1072,  "Crusader",                                3,  KINGS_BADGE,     REWARD_TYPE_TITLE},
    {17199, "Champion",                                1,  KINGS_BADGE,     REWARD_TYPE_TITLE},

    /* Mounts */
    {12385, "Magic Rooster",                           30, BLOODSHED_TOKEN, REWARD_TYPE_MOUNT},
    {12439, "Brewfest Ram",                            30, BLOODSHED_TOKEN, REWARD_TYPE_MOUNT},
    {12441, "Amani War Bear",                          25, BLOODSHED_TOKEN, REWARD_TYPE_MOUNT},
    {13149, "Zulian Tiger",                            25, BLOODSHED_TOKEN, REWARD_TYPE_MOUNT},
    {13151, "Fiery Warhorse",                          25, BLOODSHED_TOKEN, REWARD_TYPE_MOUNT},
    {13152, "Celestial Steed",                         15, BLOODSHED_TOKEN, REWARD_TYPE_MOUNT},
    {13153, "War Wolf (Horde) / War Tiger (Alliance)", 10, BLOODSHED_TOKEN, REWARD_TYPE_MOUNT},
    {13154, "Dark Riding Talbuk",                      10, BLOODSHED_TOKEN, REWARD_TYPE_MOUNT},

    {44572, "Spectral Tiger",                          3,  KINGS_BADGE,     REWARD_TYPE_MOUNT},
    {17350, "Raven Lord",                              1,  KINGS_BADGE,     REWARD_TYPE_MOUNT},
    {17729, "Brewfest Kodo",                           1,  KINGS_BADGE,     REWARD_TYPE_MOUNT},
    {18209, "Invincible",                              5,  KINGS_BADGE,     REWARD_TYPE_MOUNT},
    {18595, "Ashes of Al'ar",                          3,  KINGS_BADGE,     REWARD_TYPE_MOUNT},

    /* Items */
    {23162, "Foror's Create (36 Slot Bag)",                          2,  BLOODSHED_TOKEN,     REWARD_TYPE_ITEM},


    // 696905 - Season 1 Shirt - spell effect: 62579

};


#endif //TRINITYCORE_BATTLE_ROYAL_REWARDS_H

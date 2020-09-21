#ifndef TRINITYCORE_BATTLE_ROYAL_REWARDS_H
#define TRINITYCORE_BATTLE_ROYAL_REWARDS_H


#include <string>
#include <vector>
#include <map>

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
    BLOODSHED_TOKEN = 69600,
    KINGS_BADGE = 69601
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

bool compareByName(const Reward &a, const Reward &b) {
    return a.name < b.name;
}

std::map<int, Reward> _rewards;

const std::vector<Reward> rewards{
        /* Titles */
        {69602, "Battlemaster",                            20, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
        {69603, "Blood Guard",                             25, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
        {69604, "Grunt",                                   5,  BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
        {69605, "Blood Sail Admiral",                      40, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
        {69606, "Brutual Gladiator",                       75, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},

        {69607, "Knight-Champion",                         30, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
        {69608, "Commander",                               20, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
        {69609, "Deadly Gladiator",                        75, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
        {69610, "Flame Keeper",                            10, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
        {69611,  "Merrymaker",                              5,  BLOODSHED_TOKEN, REWARD_TYPE_TITLE},
        {69612, "Vanquisher",                              30, BLOODSHED_TOKEN, REWARD_TYPE_TITLE},

        {69613,  "Bane of the Fallen King",                 10, KINGS_BADGE,     REWARD_TYPE_TITLE},
        {69614,  "Celestial Defender",                      5,  KINGS_BADGE,     REWARD_TYPE_TITLE},
        {69615,  "Crusader",                                3,  KINGS_BADGE,     REWARD_TYPE_TITLE},
        {69616, "Champion",                                1,  KINGS_BADGE,     REWARD_TYPE_TITLE},

        /* Mounts */
        {69617, "Magic Rooster",                           30, BLOODSHED_TOKEN, REWARD_TYPE_MOUNT},
        {69618, "Brewfest Ram",                            30, BLOODSHED_TOKEN, REWARD_TYPE_MOUNT},
        {69619, "Amani War Bear",                          25, BLOODSHED_TOKEN, REWARD_TYPE_MOUNT},
        {69620, "Zulian Tiger",                            25, BLOODSHED_TOKEN, REWARD_TYPE_MOUNT},
        {69621, "Fiery Warhorse",                          25, BLOODSHED_TOKEN, REWARD_TYPE_MOUNT},
        {69622, "Celestial Steed",                         15, BLOODSHED_TOKEN, REWARD_TYPE_MOUNT},
        {69623, "War Wolf (Horde) / War Tiger (Alliance)", 10, BLOODSHED_TOKEN, REWARD_TYPE_MOUNT},
        {69624, "Dark Riding Talbuk",                      10, BLOODSHED_TOKEN, REWARD_TYPE_MOUNT},

        {69625, "Spectral Tiger",                          3,  KINGS_BADGE,     REWARD_TYPE_MOUNT},
        {69626, "Raven Lord",                              1,  KINGS_BADGE,     REWARD_TYPE_MOUNT},
        {69627, "Brewfest Kodo",                           1,  KINGS_BADGE,     REWARD_TYPE_MOUNT},
        {69628, "Invincible",                              5,  KINGS_BADGE,     REWARD_TYPE_MOUNT},
        {69629, "Ashes of Al'ar",                          3,  KINGS_BADGE,     REWARD_TYPE_MOUNT},

        /* Items */
        {23162, "Foror's Create (36 Slot Bag)",            2,  BLOODSHED_TOKEN, REWARD_TYPE_ITEM},


        // 696905 - Season 1 Shirt - spell effect: 62579

};


#endif //TRINITYCORE_BATTLE_ROYAL_REWARDS_H

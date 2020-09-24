#include "battle_royale.h"

#include <utility>
#include "GameObject.h"
#include "GameObjectAI.h"
#include "Player.h"
#include "Chat.h"
#include "ScriptMgr.h"
#include "MapManager.h"
#include "Creature.h"
#include "World.h"
#include "WorldSession.h"
#include "OutdoorPvP.h"
#include <cstdlib>
#include <ctime>
#include <random>
#include "Configuration/Config.h"

enum REWARD_CURRENCY {
    BLOODSHED_TOKEN = 69600,
    KINGS_BADGE = 69601
};


uint32 const ITEM_KILL_TOKEN = BLOODSHED_TOKEN;
uint32 const ITEM_WINNER_TOKEN = KINGS_BADGE;
int const SPELL_RELIC_BUFF_1 = 63130;
int const SPELL_RELIC_BUFF_2 = 63131;
int const SPELL_RELIC_BUFF_3 = 63132;
int const SPELL_INFECT_DMG4 = 11821;


int const FIVE_MINUTES_IN_MS = 300000;
int const ONE_MINUTE_IN_MS = 60000;
int RELIC = 1;
//int const ROYALE_ZONE_ID = 493; // Moonglade
int const ROYALE_MAP_ID = 1;

class BattleRoyaleWorldScript : public WorldScript

{
public:

    BattleRoyaleWorldScript() : WorldScript("BattleRoyaleConfig") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload) {
            std::string conf_path = _CONF_DIR;
            std::string cfg_file = conf_path + "/battle_royale.conf";
            sConfigMgr->LoadMore(cfg_file.c_str());
        }
    }
};

class BattleRoyalRelic : public GameObjectScript {
public:
    BattleRoyalRelic() : GameObjectScript("battle_royale_relic") {}

    bool OnGossipHello(Player *player, GameObject* /*go*/) override {
        int spellID;
        switch (RELIC) {
            case 1:
                spellID = SPELL_RELIC_BUFF_1;
                break;
            case 2:
                spellID = SPELL_RELIC_BUFF_2;
                break;
            case 3:
                spellID = SPELL_RELIC_BUFF_3;
                break;
            default:
                return true;
        }

        if (!player->HasAura(spellID)) {
            player->SetAuraStack(spellID, player, 1);
        } else {
            player->RemoveAura(spellID);
        }
        return true;
    }
};

class OutdoorPvP_moonglade_battle_royal : public OutdoorPvPScript {
public:
    OutdoorPvP_moonglade_battle_royal() : OutdoorPvPScript("outdoorpvp_mbr") {}

    OutdoorPvP *GetOutdoorPvP() const override {
        return new BattleRoyale();
    }
};

BattleRoyale::BattleRoyale() {
    m_TypeId = OUTDOOR_PVP_BR;
    m_zoneId = 493;
    royaleEnabled = sConfigMgr->GetBoolDefault("Battle_Royale.Core.Enable", true);
    requiredPlayers = sConfigMgr->GetIntDefault("Battle_Royale.Core.RequiredPlayers", 2);
    spawnPointNighthaven = Position(7830.34f, -2433.79f, 488.01f, 4.74f);
    spawnPointShrine = Position(7842.36f, -2225.37f, 468.40, 4.05f);
    spawnPointDens = Position(7563.89f, -2951.61f, 466.69f, 1.51f);
}

void BattleRoyale::HandleInfectionKill(Player *player) {
    char msg[250];
    snprintf(msg, 250,
             "%s was killed by the infection! Quicker next time!",
             player->GetName().c_str());
    sWorld->SendZoneText(m_zoneId, msg);
    RemovePlayerFromRoyale(player);
}

void BattleRoyale::HandleKill(Player *player, Unit *killed) {

    if (killed->GetTypeId() == TYPEID_PLAYER) {
        player->AddItem(ITEM_KILL_TOKEN, 1);
        auto *victim = dynamic_cast<Player *>(killed);
        RemovePlayerFromRoyale(victim);
        char msg[250];
        snprintf(msg, 250,
                 "%s killed %s! Better luck next time!",
                 player->GetName().c_str(), victim->GetName().c_str());
        sWorld->SendZoneText(m_zoneId, msg);
    }
}

bool BattleRoyale::SetupOutdoorPvP() {
//    SetMapFromZone(493); -- tc carryover
    // Moonglade
    RegisterZone(m_zoneId);
    return true;
}

void BattleRoyale::setPlayerFFAFlags(Player *player) {
    if (player) {
        player->pvpInfo.IsInFFAPvPArea = true;
        player->UpdatePvPState(true);
    }
}

void BattleRoyale::removePlayerFFAFlags(Player *player) {
    if (player) {
        player->pvpInfo.IsInFFAPvPArea = false;
        player->UpdatePvPState(false);
    }
}

void BattleRoyale::NotifyPlayerEnteredQueue(Player *player) {
    player->GetSession()->SendAreaTriggerMessage(
            "You've joined the queue for the battle royale. (Total in queue: %zu)",
            queue.size());
}

void BattleRoyale::NotifyPlayerGameInProgress(Player *player) {
    player->GetSession()->SendAreaTriggerMessage(
            "The royale is already in progress. You'll have to wait for the next one. Players Remaining: (%lu)",
            playersInRoyale.size());
}

void BattleRoyale::NotifyPlayerMissedQueue(Player *player) {
    player->GetSession()->SendAreaTriggerMessage(
            "Sorry, the royale is already at the player limit. Try again next time! Players Remaining: (%lu)",
            playersInRoyale.size());
}

void BattleRoyale::TeleportPlayerToHS(Player *player) {
    if (!player->IsBeingTeleported()) {
        player->TeleportTo(player->m_homebindMapId, player->m_homebindX,
                           player->m_homebindY, player->m_homebindZ, 0.0f);
    }
}

void BattleRoyale::HandlePlayerEnterZone(Player *player, uint32 zone) {
    if (player->IsGameMaster()) {
        return;
    }
    if (gameStarted) {
        if (IsPlayerInRoyale(player)) {
            setPlayerFFAFlags(player);
        } else {
            // Remove player from Moonglade
            NotifyPlayerGameInProgress(player);
            RemovePlayerFromRoyale(player);
        }
    } else {
        // add player to queue
        if (!IsPlayerInQueue(player)) {
            queue.push_back(player->GetGUID());
        }
        NotifyPlayerEnteredQueue(player);
    }
    OutdoorPvP::HandlePlayerEnterZone(player, zone);
}

void BattleRoyale::HandlePlayerLeaveZone(Player *player, uint32 zone) {
    if (player->IsGameMaster()) {
        return;
    }
    if (gameStarted) {
        RemovePlayerFromRoyaleTracker(player);
    } else {
        RemovePlayerFromQueue(player);
    }
    removePlayerFFAFlags(player);
    OutdoorPvP::HandlePlayerLeaveZone(player, zone);
}

void BattleRoyale::resetRoyale() {
    gameStarted = false;
    aggressivePlayers = false;
    startInfecting = false;
    currentIteration = 1;
    startTimer = 60000;
    relicSwitchTimer = 60000;
    playersInRoyale.clear();
    if (currentRelic != nullptr) {
        currentRelic->RemoveFromWorld();
    }
}


void BattleRoyale::setNewRelic() {

    // 5 min -> 5 min -> 3:20 -> 2:20 -> 2:30 -> 1:30 -> 2:00 -> 1:00 -> 2:00 -> 0:40 -> 1:30 -> 1:30 -> 0:30 -> 1:00 -> 0:30
    switch (currentIteration) {
        case 1:
            relicSwitchTimer = FIVE_MINUTES_IN_MS;
            break;
        case 2:
            relicSwitchTimer = ONE_MINUTE_IN_MS * 4;
            break;
        case 3:
            relicSwitchTimer = ONE_MINUTE_IN_MS * 3;
            break;
        case 4:
        case 5:
            relicSwitchTimer = ONE_MINUTE_IN_MS * 2;
            break;
        case 6:
        case 7:
            relicSwitchTimer = ONE_MINUTE_IN_MS;
            break;
        default:
            relicSwitchTimer = ONE_MINUTE_IN_MS / 2;
            break;
    }
    currentIteration += 1;

    SelectRandomRelic();
}

void BattleRoyale::SelectRandomRelic() {
    std::mt19937 mersenne(
            static_cast<std::mt19937::result_type>(std::time(nullptr)));
    std::uniform_int_distribution<> die(1, 3);
    int newRelic = (int) die(mersenne);
    while (newRelic == RELIC) {
        newRelic = (int) die(mersenne);
    }
    RELIC = newRelic;
}


void BattleRoyale::setGracePeriod() {
    gracePeriod = (int) (relicSwitchTimer * .25);
}

void BattleRoyale::NotifyNewRelic() {
    char msg[250];
    std::string s;
    switch (RELIC) {
        case 1:
            s = "New infection released! A relic in Nighthaven has the cure! (Time before infection: %i seconds)";
            break;
        case 2:
            s = "New infection released! The Shrine of Remulos' relic has the cure! (Time before infection: %i seconds)";
            break;
        case 3:
            s = "New infection released! A relic was spotted at Stormrage Barrow Dens! (Time before infection: %i seconds)";
            break;
        default:
            s = "This shouldnt happen, something is broken!";
            break;
    }
    snprintf(msg, 250,
             s.c_str(),
             gracePeriod / 1000);
    sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
}

void BattleRoyale::spawnNewRelic() {
    if (currentRelic != nullptr) {
        currentRelic->RemoveFromWorld();
    }

    switch (RELIC) {
        case 1:
            currentRelic = SpawnGameObject(133702, spawnPointNighthaven);
            break;
        case 2:
            currentRelic = SpawnGameObject(133703, spawnPointShrine);
            break;
        case 3:
            currentRelic = SpawnGameObject(133704, spawnPointDens);
            break;
        default:
            break;
    }
}

bool BattleRoyale::Update(uint32 diff) {
    int m_diff = (int) diff;
    if (!gameStarted && checkShouldStart()) {
        SelectQueuedPlayers();
        RemoveExtraPlayers();
        startRoyale();
    }

    if (gameStarted) {

        if (aggressivePlayers) {
            TurnPlayersAggressive(); // DBC makes moonglade revert FFA flags on Player::UpdateArea().
        }

        if ((int) playersInRoyale.size() == 1 && aggressivePlayers) {
            AwardWinner();
            resetRoyale();
            return true;
        }

        relicSwitchTimer -= m_diff;

        if (relicSwitchTimer <= m_diff) { // switch Relic to new location
            if (currentIteration == 1) {
                startInfecting = true;
            }
            clearExistingRelicBuffs();
            setNewRelic();
            spawnNewRelic();
            setGracePeriod();
            NotifyNewRelic();
        }

        if (gracePeriod <= m_diff &&
            startInfecting) { // Damage players without relic buffs
            infectPlayers();
        }

        if (startInfecting) {
            gracePeriod -= m_diff;
        }
    }

    startTimer -= m_diff;

    if (startTimer <= m_diff && gameStarted && !aggressivePlayers) {
        TurnPlayersAggressive();
        aggressivePlayers = true;
        char msg[250];
        snprintf(msg, 250,
                 "The Battle Royale has begun! Good luck! (Players: [%lu])",
                 playersInRoyale.size());
        sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
        return true;
    }

    return true;
}

int BattleRoyale::getRelicBuffSpellID() {
    int spellID;
    switch (RELIC) {
        case 1:
            spellID = SPELL_RELIC_BUFF_1;
            break;
        case 2:
            spellID = SPELL_RELIC_BUFF_2;
            break;
        case 3:
            spellID = SPELL_RELIC_BUFF_3;
            break;
        default:
            spellID = SPELL_RELIC_BUFF_3;
            break;
    }
    return spellID;
}

void BattleRoyale::AwardWinner() {
    if (!playersInRoyale.empty()) {
        for (auto itr = playersInRoyale.begin();
             itr != playersInRoyale.end();) {
            uint64 playerGuid = *itr;
            if (Player *player = ObjectAccessor::FindPlayer(playerGuid)) {
                char msg[250];
                snprintf(msg, 250,
                         "%s has won the Battle Royale! A legend is born!",
                         player->GetName().c_str());
                sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
                player->AddItem(ITEM_WINNER_TOKEN, 1);
                RemovePlayerFromRoyale(player);
            }
            break;
        }
    }
}

void BattleRoyale::TurnPlayersAggressive() {
    for (auto itr = playersInRoyale.begin();
         itr != playersInRoyale.end();) {
        uint64 playerGuid = *itr;
        ++itr;

        if (Player *player = ObjectAccessor::FindPlayer(playerGuid)) {
            if (player->IsAlive()) {
                setPlayerFFAFlags(player);
            }
        }
    }

}

void BattleRoyale::clearExistingRelicBuffs() {
    int spellID = getRelicBuffSpellID();
    for (auto itr = playersInRoyale.begin();
         itr != playersInRoyale.end();) {
        uint64 playerGuid = *itr;
        ++itr;
        if (Player *player = ObjectAccessor::FindPlayer(playerGuid)) {
            if (player->HasAura(spellID)) {
                player->RemoveAura(spellID);
            }
        }
    }
}

void BattleRoyale::infectPlayers() {
    int relicAuraSpellID = getRelicBuffSpellID();
    for (auto itr = playersInRoyale.begin();
         itr != playersInRoyale.end();) {
        uint64 playerGuid = *itr;
        ++itr;

        if (Player *player = ObjectAccessor::FindPlayer(playerGuid)) {
            if (player->IsAlive() && !player->HasAura(relicAuraSpellID)) {
                player->CastSpell(player, SPELL_INFECT_DMG4, true);
                if (!player->IsAlive() && (int) playersInRoyale.size() != 1) {
                    HandleInfectionKill(player);
                }
            }
        }
    }
}

bool BattleRoyale::checkShouldStart() {
    return royaleEnabled && !gameStarted && (int) queue.size() >= requiredPlayers;
}

void BattleRoyale::RemoveExtraPlayers() {
    while (!queue.empty()) {
        uint64 playerGuid = queue.front();

        if (Player *player = ObjectAccessor::FindPlayer(playerGuid)) {
            if (player->IsAlive()) {
                TeleportPlayerToHS(player);
                NotifyPlayerMissedQueue(player);
            }
        }
        queue.pop_front();
    }
}

void BattleRoyale::SelectQueuedPlayers() {

    while (!queue.empty() || playersInRoyale.size() == (unsigned  long) playerLimit) {
        uint64 playerGuid = queue.front();

        if (Player *player = ObjectAccessor::FindPlayer(playerGuid)) {
            if (player->IsAlive() && !player->IsBeingTeleported()) {
                playersInRoyale.insert(player->GetGUID());
            }
        }
        queue.pop_front();
    }
}

void BattleRoyale::startRoyale() {
    gameStarted = true;
    char msg[250];
    snprintf(msg, 250,
             "The Battle Royale is about to begin in 1 minute! (Players: [%lu])",
             playersInRoyale.size());
    sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
    this->startTimer = 60000;
}

bool BattleRoyale::IsPlayerInRoyale(Player *player) {
    return playersInRoyale.find(player->GetGUID()) != playersInRoyale.end();
}

void BattleRoyale::RemovePlayerFromRoyaleTracker(Player *player) {
    if (IsPlayerInRoyale(player)) {
        playersInRoyale.erase(player->GetGUID());
    }
}

void BattleRoyale::RemovePlayerFromRoyale(Player *player) {
    RemovePlayerFromQueue(player);
    RemovePlayerFromRoyaleTracker(player);
    player->ResurrectPlayer(1.0f);
    TeleportPlayerToHS(player);
}

void BattleRoyale::RemovePlayerFromQueue(Player *player) {
    if (IsPlayerInQueue(player)) {
        auto itr = std::find(queue.begin(), queue.end(), player->GetGUID());
        queue.erase(itr);
    }
}

bool BattleRoyale::IsPlayerInQueue(Player *player) {
    return std::find(queue.begin(), queue.end(), player->GetGUID()) !=
           queue.end();
}

GameObject *BattleRoyale::SpawnGameObject(uint32 entry, Position const &pos) {
    // Get map object
    Map *map = sMapMgr->CreateBaseMap(ROYALE_MAP_ID);
    if (!map) {
        return nullptr;
    }

    auto *go = new GameObject();
    if (!go->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_GAMEOBJECT), entry,
                    map, PHASEMASK_NORMAL, pos.GetPositionX(),
                    pos.GetPositionY(),
                    pos.GetPositionZ(), pos.GetOrientation(), G3D::Quat(), 100,
                    GO_STATE_READY)) {
        delete go;
        sLog->outError(
                "BattleRoyale::SpawnGameObject: Cannot create gameobject %u! Battle Royale not created!",
                entry);
        return nullptr;
    }

    map->AddToMap(go);
    go->setActive(true);
    return go;
}


void Battle_RoyaleScripts() {
    new BattleRoyaleWorldScript();
    new OutdoorPvP_moonglade_battle_royal();
    new BattleRoyalRelic();
}

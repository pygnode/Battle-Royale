#include "battle_royal.h"

#include <utility>
#include <ScriptPCH.h>
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
#include "UnitDefines.h"

enum REWARD_CURRENCY {
    BLOODSHED_TOKEN = 19063,
    KINGS_BADGE = 1995
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


class BattleRoyalRelic : public GameObjectScript {
public:
    BattleRoyalRelic() : GameObjectScript("battle_royale_relic") {}

    struct BattleRoyalRelicAI : public GameObjectAI {
        explicit BattleRoyalRelicAI(GameObject *go) : GameObjectAI(go) {}

        bool GossipHello(Player* player) override {
            int spellID = -1;
            switch(RELIC) {
                case 1:
                    spellID = SPELL_RELIC_BUFF_1;
                    break;
                case 2:
                    spellID = SPELL_RELIC_BUFF_2;
                    break;
                case 3:
                    spellID = SPELL_RELIC_BUFF_3;
                    break;
            }
            if(spellID == -1) {
                return true;
            }

            if(!player->HasAura(spellID)) {
                player->SetAuraStack(spellID, player, 1);
            } else {
                player->RemoveAura(spellID);
            }
            return true;
        }
    };

    GameObjectAI* GetAI(GameObject *go) const override {
        return new BattleRoyalRelicAI(go);
    }

};

class OutdoorPvP_moonglade_battle_royal : public OutdoorPvPScript {
public:
    OutdoorPvP_moonglade_battle_royal() : OutdoorPvPScript("outdoorpvp_mbr") {}

    OutdoorPvP *GetOutdoorPvP() const override {
        return new BattleRoyale();
    }
};

//https://community.trinitycore.org/topic/14891-share-callback-between-two-custom-scripts-question/
BattleRoyale::BattleRoyale() {
    m_TypeId = OUTDOOR_PVP_BR;
    m_zoneId = 493;
    spawnPointNighthaven = Position(7830.34f, -2433.79f, 488.01f, 4.74f);
    spawnPointShrine = Position(7842.36f, -2225.37f, 468.40, 4.05f);
    spawnPointDens = Position(7563.89f, -2951.61f, 466.69f, 1.51f);

}

void BattleRoyale::HandleInfectionKill(Player* player)
{
    char msg[250];
    snprintf(msg, 250,
             "%s was killed by the infection! Quicker next time!",
             player->GetName().c_str());
    sWorld->SendZoneText(m_zoneId, msg);
    RemovePlayerFromRoyal(player);
}

void BattleRoyale::HandleKill(Player* player, Unit *killed) {

    if (killed->GetTypeId() == TYPEID_PLAYER) {
         player->AddItem(ITEM_KILL_TOKEN, 1);
         Player* victim = static_cast<Player*>(killed);
         RemovePlayerFromRoyal(victim);
        char msg[250];
        snprintf(msg, 250,
                 "%s killed %s! Better luck next time!",
                 player->GetName().c_str(), victim->GetName().c_str());
        sWorld->SendZoneText(m_zoneId, msg);
    }
}

bool BattleRoyale::SetupOutdoorPvP() {
    // Moonglade
    SetMapFromZone(493);
    RegisterZone(m_zoneId);
    return true;
}

void BattleRoyale::setPlayerFFAFlags(Player* player) {
    if(player) {
        player->pvpInfo.IsInFFAPvPArea = true;
        player->UpdatePvPState(true);
    }
}

void BattleRoyale::removePlayerFFAFlags(Player* player) {
    if(player) {
        player->pvpInfo.IsInFFAPvPArea = false;
        player->UpdatePvPState(false);
    }
}

void BattleRoyale::NotifyPlayerEnteredQueue(Player* player) {
    player->GetSession()->SendAreaTriggerMessage(
        "You've joined the queue for the battle royale. (Total in queue: %zu)",
        queue.size());
}

void BattleRoyale::NotifyPlayerGameInProgress(Player* player) {
    player->GetSession()->SendAreaTriggerMessage(
        "The royale is already in progress. You'll have to wait for the next one. Players Remaining: (%lu)",
        playersInRoyale.size());
}

void BattleRoyale::NotifyPlayerMissedQueue(Player* player) {
    player->GetSession()->SendAreaTriggerMessage(
        "Sorry, the royale is already at the player limit. Try again next time! Players Remaining: (%lu)",
        playersInRoyale.size());
}

void BattleRoyale::TeleportPlayerToHS(Player* player) {
    if(!player->IsBeingTeleported()) {
        player->TeleportTo(player->m_homebindMapId, player->m_homebindX,
                           player->m_homebindY, player->m_homebindZ, 0.0f);
    }
}

void BattleRoyale::HandlePlayerEnterZone(Player* player, uint32 zone) {
    if(player->IsGameMaster()) {
        return;
    }
    if(gameStarted) {
        if (IsPlayerInRoyale(player)) {
            setPlayerFFAFlags(player);
        } else {
            // Remove player from Moonglade
            NotifyPlayerGameInProgress(player);
            RemovePlayerFromRoyal(player);
        }
    } else {
        // add player to queue
        if(!IsPlayerInQueue(player)) {
            queue.push_back(player->GetGUID());
        }
        NotifyPlayerEnteredQueue(player);
    }
    OutdoorPvP::HandlePlayerEnterZone(player, zone);
}

void BattleRoyale::HandlePlayerLeaveZone(Player *player, uint32 zone) {
    if(player->IsGameMaster()) {
        return;
    }
    if(gameStarted) {
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
    if(currentRelic != nullptr) {
        currentRelic->DespawnOrUnsummon(Milliseconds(200));
    }
}


void BattleRoyale::setNewRelic() {

   // 5 min -> 5 min -> 3:20 -> 2:20 -> 2:30 -> 1:30 -> 2:00 -> 1:00 -> 2:00 -> 0:40 -> 1:30 -> 1:30 -> 0:30 -> 1:00 -> 0:30
    switch(currentIteration) {
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
            relicSwitchTimer = ONE_MINUTE_IN_MS * 2;
            break;
        case 5:
            relicSwitchTimer = ONE_MINUTE_IN_MS * 2;
            break;
        case 6:
            relicSwitchTimer = ONE_MINUTE_IN_MS;
            break;
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
    std::mt19937 mersenne(static_cast<std::mt19937::result_type>(std::time(nullptr)));
    std::uniform_int_distribution<> die(1, 3);
    int newRelic = (int) die(mersenne);
    while(newRelic == RELIC) {
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
    switch(RELIC) {
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
    if(currentRelic != nullptr) {
        currentRelic->DespawnOrUnsummon(Milliseconds(200));
    }

    switch(RELIC) {
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

    if(gameStarted) {

        if(aggressivePlayers) {
            TurnPlayersAggressive(); // DBC makes moonglade revert FFA flags on Player::UpdateArea().
        }

        if((int) playersInRoyale.size() == 1 && aggressivePlayers) {
            AwardWinner();
            resetRoyale();
            return true;
        }

        relicSwitchTimer -= m_diff;

        if (relicSwitchTimer <= m_diff) { // switch Relic to new location
            if(currentIteration == 1) {
                startInfecting = true;
            }
            clearExistingRelicBuffs();
            setNewRelic();
            spawnNewRelic();
            setGracePeriod();
            NotifyNewRelic();
        }

        if(gracePeriod <= m_diff && startInfecting) { // Damage players without relic buffs
            infectPlayers();
        }

        if(startInfecting) {
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
            game_master->Yell("The battle royal has begun!",
                                LANG_UNIVERSAL);
            return true;
        }

    return true;
}


int BattleRoyale::getRelicBuffSpellID() {
    int spellID;
    switch(RELIC) {
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
    if(!playersInRoyale.empty()) {
        for(GuidSet::iterator itr = playersInRoyale.begin(); itr != playersInRoyale.end();) {
            ObjectGuid playerGuid = *itr;
            if (Player *player = ObjectAccessor::FindPlayer(playerGuid)) {
                char msg[250];
                snprintf(msg, 250,
                         "%s has won the Battle Royale! What a legend!",
                         player->GetName().c_str());
                sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
                player->AddItem(ITEM_WINNER_TOKEN, 1);
                RemovePlayerFromRoyal(player);
            }
            break;
        }
    }
}

void BattleRoyale::TurnPlayersAggressive() {
    for(GuidSet::iterator itr = playersInRoyale.begin(); itr != playersInRoyale.end();) {
        ObjectGuid playerGuid = *itr;
        ++itr;

        if(Player* player = ObjectAccessor::FindPlayer(playerGuid)) {
            if(player->IsAlive()) {
                setPlayerFFAFlags(player);
            }
        }
    }

}

void BattleRoyale::clearExistingRelicBuffs() {
    int spellID = getRelicBuffSpellID();
    for(GuidSet::iterator itr = playersInRoyale.begin(); itr != playersInRoyale.end();) {
        ObjectGuid playerGuid = *itr;
        ++itr;
        if(Player* player = ObjectAccessor::FindPlayer(playerGuid)) {
            if(player->IsAlive() && player->HasAura(spellID)) {
                player->RemoveAura(spellID);
            }
        }
    }
}

void BattleRoyale::infectPlayers() {
    int relicAuraSpellID = getRelicBuffSpellID();
    for(GuidSet::iterator itr = playersInRoyale.begin(); itr != playersInRoyale.end();) {
        ObjectGuid playerGuid = *itr;
        ++itr;

        if(Player* player = ObjectAccessor::FindPlayer(playerGuid)) {
            if(player->IsAlive() && !player->HasAura(relicAuraSpellID)) {
                player->CastSpell(player, SPELL_INFECT_DMG4, true);
                if(!player->IsAlive()) {
                    HandleInfectionKill(player);
                }
            }
        }
    }
}

bool BattleRoyale::checkShouldStart() {
    return !gameStarted && queue.size() >= 3;
}

void BattleRoyale::RemoveExtraPlayers() {
    while(!queue.empty()) {
        ObjectGuid playerGuid = queue.front();

        if(Player* player = ObjectAccessor::FindPlayer(playerGuid)) {
            if(player->IsAlive()) {
                TeleportPlayerToHS(player);
                NotifyPlayerMissedQueue(player);
            }
        }
        queue.pop_front();
    }
}

void BattleRoyale::SelectQueuedPlayers() {

    while(!queue.empty() || playersInRoyale.size() == playerLimit) {
        ObjectGuid playerGuid = queue.front();

        if(Player* player = ObjectAccessor::FindPlayer(playerGuid)) {
            if(player->IsAlive() && !player->IsBeingTeleported()) {
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
    this->game_master = SpawnCreature(133701,
                                      Position(7396.81, -2186.72, 527, 0.6));
    this->startTimer = 60000;
}

bool BattleRoyale::IsPlayerInRoyale(Player* player) {
    return playersInRoyale.find(player->GetGUID()) != playersInRoyale.end();
}

void BattleRoyale::RemovePlayerFromRoyaleTracker(Player* player) {
    if(IsPlayerInRoyale(player)) {
        playersInRoyale.erase(player->GetGUID());
    }
}

void BattleRoyale::RemovePlayerFromRoyal(Player* player) {
    RemovePlayerFromQueue(player);
    RemovePlayerFromRoyaleTracker(player);
    player->ResurrectPlayer(1.0f);
    TeleportPlayerToHS(player);
}

void BattleRoyale::RemovePlayerFromQueue(Player *player) {
    if(IsPlayerInQueue(player)) {
        auto itr = std::find(queue.begin(), queue.end(), player->GetGUID());
        queue.erase(itr);
    }
}

bool BattleRoyale::IsPlayerInQueue(Player *player) {
    return std::find(queue.begin(), queue.end(), player->GetGUID()) != queue.end();
}

Creature *BattleRoyale::SpawnCreature(uint32 entry, Position const &pos) {
    //Get map object
    Map *map = sMapMgr->CreateBaseMap(1);
    if (!map) {
        return nullptr;
    }

    auto creature = new Creature();
    if (!creature->Create(map->GenerateLowGuid<HighGuid::Unit>(), map,
                          PHASEMASK_NORMAL, entry, pos)) {
        delete creature;
        return nullptr;
    }

    creature->SetHomePosition(pos);

    // Set creature in world
    map->AddToMap(creature);
    creature->setActive(true);
    creature->SetFarVisible(true);

    return creature;
}

GameObject* BattleRoyale::SpawnGameObject(uint32 entry, Position const& pos)
{
    // Get map object
    if (!m_map)
        return nullptr;

    // Create gameobject
    GameObject* go = new GameObject;
    QuaternionData rot = QuaternionData::fromEulerAnglesZYX(0.f, 0.f, 0.f);
    if (!go->Create(m_map->GenerateLowGuid<HighGuid::GameObject>(), entry, m_map, PHASEMASK_NORMAL, pos, rot, 255, GO_STATE_READY))
    {
        delete go;
        return nullptr;
    }

    // Add to world
    m_map->AddToMap(go);
    go->SetFarVisible(true);
    go->SetIsNewObject(true);
    return go;
}


void AddSC_battle_royal() {
    new OutdoorPvP_moonglade_battle_royal();
    new BattleRoyalRelic();
}

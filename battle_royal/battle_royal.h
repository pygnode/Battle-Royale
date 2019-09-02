#ifndef TRINITYCORE_BATTLE_ROYAL_H
#define TRINITYCORE_BATTLE_ROYAL_H

#include "ScriptMgr.h"
#include "Player.h"
#include "OutdoorPvP.h"
#include <map>

using namespace std;


class BattleRoyale : public OutdoorPvP {
public:
    BattleRoyale();

    bool SetupOutdoorPvP() override;

    void HandlePlayerEnterZone(Player *player, uint32 zone) override;

    void HandlePlayerLeaveZone(Player *player, uint32 zone) override;

    bool Update(uint32 diff) override;

    void HandleKill(Player* player, Unit* killed) override;

private:
    void startRoyale();

    void resetRoyale();

    bool checkShouldStart();

    void RemovePlayerFromRoyal(Player *player);

    bool IsPlayerInQueue(Player *player);

    void RemovePlayerFromQueue(Player *player);

    void RemovePlayerFromRoyaleTracker(Player *player);

    void TurnPlayersAggressive();

    void setNewRelic();

    void infectPlayers();

    bool IsPlayerInRoyale(Player *player);

    static void setPlayerFFAFlags(Player *player);

    static void removePlayerFFAFlags(Player *player);

    void NotifyPlayerEnteredQueue(Player *player);

    void NotifyPlayerGameInProgress(Player *player);

    static void TeleportPlayerToHS(Player *player);

    void RemoveExtraPlayers();

    void NotifyPlayerMissedQueue(Player *player);

    void SelectQueuedPlayers();

    void AwardWinner();

    void NotifyNewRelic();

    void spawnNewRelic();

    void setGracePeriod();

    void clearExistingRelicBuffs();

    int getRelicBuffSpellID();

    void HandleInfectionKill(Player* player);

    void SelectRandomRelic();

    static Creature *SpawnCreature(uint32 entry, Position const &pos);

    GameObject *SpawnGameObject(uint32 entry, Position const &pos);

    int m_zoneId;
    Creature *game_master;
    GuidDeque queue;
    GuidSet playersInRoyale;
    bool gameStarted = false;
    bool aggressivePlayers = false;
    bool startInfecting = false;
    ulong playerLimit = 40;
    int currentIteration = 1;
    int startTimer = 60000;
    int relicSwitchTimer = 60000;
    int gracePeriod = 2000;
    Position spawnPointNighthaven;
    Position spawnPointShrine;
    Position spawnPointDens;
    GameObject* currentRelic = nullptr;

};

#endif //TRINITYCORE_BATTLE_ROYAL_H

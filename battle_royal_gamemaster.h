#ifndef TRINITYCORE_BATTLE_ROYAL_GAMEMASTER_H
#define TRINITYCORE_BATTLE_ROYAL_GAMEMASTER_H

#include "ScriptMgr.h"
#include "CreatureAI.h""
#include "Creature.h"


class npc_battle_royal_gamemaster : CreatureScript
{
public:
    npc_battle_royal_gamemaster();

    CreatureAI* GetAI(Creature* creature) override;

};

#endif //TRINITYCORE_BATTLE_ROYAL_GAMEMASTER_H

#include "ScriptMgr.h"
#include "ObjectAccessor.h"
#include "MotionMaster.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SpellInfo.h"


class npc_battle_royal_teleporter : public CreatureScript {
public:
    npc_battle_royal_teleporter() : CreatureScript(
        "npc_battle_royal_teleporter") {
    }


    struct TeleporterAI : public ScriptedAI {
    public:
        TeleporterAI(Creature *creature) : ScriptedAI(creature) {}

        bool GossipHello(Player *player) override {
            return onGossipHello(player, me);
        }

        static bool onGossipHello(Player *player, Creature *vendor) {
            ClearGossipMenuFor(player);

            uint32 zone = player->GetZoneId();
            if(zone != 493) {
                AddGossipItemFor(player, GOSSIP_ICON_CHAT,
                                 "|TInterface/ICONS/Ability_dualwield:30:30:-18:0|tTeleport to Battle Royale",
                                 GOSSIP_SENDER_MAIN, 0);
            } else {
                AddGossipItemFor(player, GOSSIP_ICON_CHAT,
                                 "|TInterface/ICONS/Achievement_reputation_kirintor:30:30:-18:0|tTeleport to Dalaran",
                                 GOSSIP_SENDER_MAIN, 0);
            }

            SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE,
                              vendor->GetGUID());
            return true;
        }

        bool GossipSelect(Player *player, uint32 /*menuId*/,
                          uint32 gossipListId) override {
            ClearGossipMenuFor(player);
            CloseGossipMenuFor(player);
            if(player->IsAlive() && !player->IsBeingTeleported()) {
                uint32 zone = player->GetZoneId();
                if(zone != 493) {
                    player->TeleportTo(1, 7963.105f, -2497.637f, 487.604f,
                                       3.268f);
                } else {
                    player->TeleportTo(571, 5804.1499f, 624.7710f, 647.7670f, 1.6400f);
                }
            }
            return true;
        };
    };

    CreatureAI *GetAI(Creature *creature) const override {
        return new TeleporterAI(creature);
    }

};




class npc_battle_royal_gamemaster : public CreatureScript
{
public:
    npc_battle_royal_gamemaster() : CreatureScript("npc_battle_royal_gamemaster") {}

    struct npc_battle_royal_gamemasterAI : public ScriptedAI
    {
        explicit npc_battle_royal_gamemasterAI(Creature* creature) : ScriptedAI(creature)
        {
            Initialize();
        }

        void Restart()
        {
            Initialize();
        }

        void Initialize()
        {
            me->Yell("The battle royal will begin in 1 minute!", LANG_UNIVERSAL);
        }

        void UpdateAI(uint32 diff) override
        {

        }

    private:
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_battle_royal_gamemasterAI(creature);
    }
};

void AddSC_battle_royal_gamemaster()
{
    new npc_battle_royal_gamemaster();
    new npc_battle_royal_teleporter();
}

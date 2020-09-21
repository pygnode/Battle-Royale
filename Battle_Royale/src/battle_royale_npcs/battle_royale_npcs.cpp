#include "ScriptMgr.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SpellInfo.h"


class npc_battle_royal_teleporter : public CreatureScript {
public:
    npc_battle_royal_teleporter() : CreatureScript(
            "npc_battle_royal_teleporter") {
    }

    // Called when the player opens the gossip menu assigned to this creature
    bool OnGossipHello(Player *player, Creature *creature) override {
        ClearGossipMenuFor(player);

        uint32 zone = player->GetZoneId();
        if (zone != 493) {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT,
                             "|TInterface/ICONS/Ability_dualwield:30:30:-18:0|tTeleport to Battle Royale",
                             GOSSIP_SENDER_MAIN, 0);
        } else {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT,
                             "|TInterface/ICONS/Achievement_reputation_kirintor:30:30:-18:0|tTeleport to Dalaran",
                             GOSSIP_SENDER_MAIN, 0);
        }

        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE,
                          creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*uiSender*/, uint32 /*uiAction*/) override {
        ClearGossipMenuFor(player);
        CloseGossipMenuFor(player);
        if (player->IsAlive() && !player->IsBeingTeleported()) {
            uint32 zone = player->GetZoneId();
            if (zone != 493) {
                player->TeleportTo(1, 7963.105f, -2497.637f, 487.604f,
                                   3.268f);
            } else {
                player->TeleportTo(571, 5804.1499f, 624.7710f, 647.7670f,
                                   1.6400f);
            }
        }
        return true;
    };


    struct TeleporterAI : public ScriptedAI {
    public:
        TeleporterAI(Creature *creature) : ScriptedAI(creature) {}
    };

    CreatureAI *GetAI(Creature *creature) const override {
        return new TeleporterAI(creature);
    }

};

void AddBattle_RoyaleNPCsScripts() {
    new npc_battle_royal_teleporter();
}

/*
* Copyright (C) 2005 - 2012 MaNGOS <http://www.getmangos.org/>
*
* Copyright (C) 2008 - 2012 TrinityCore <http://www.trinitycore.org/>
*
* Copyright (C) 2011 - 2012 ArkCORE <http://www.arkania.net/>
*
* Copyright (C) 2012 DeepshjirCataclysm Repack
* By Naios
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/*
* This script is arround 93% complete...
* Hardmode and Double Attack Spellscript must be rewritten.
*/

#include "ScriptPCH.h"
#include "blackwing_descent.h"

enum Events
{
    EVENT_DOUBLE_ATTACK = 1,
    EVENT_CAUSTIC_SLIME,
    EVENT_MASSACRE,
    EVENT_SEC_MASSACRE,
    EVENT_BREAK,
};

enum Actions
{
    ACTION_BILE_O_TRON_EVENT_START      = 1,
    ACTION_BILE_O_TRON_SYSTEM_FAILURE,
    ACTION_BILE_O_TRON_RESET,
};

enum Spells
{
    // Chimaeron
    SPELL_DOUBLE_ATTACK                 = 88826,
    SPELL_CAUSTIC_SLIME                 = 82935,
    SPELL_MASSACRE                      = 82848,
    SPELL_FEUD                          = 88872,
    SPELL_BREAK                         = 82881,

    SPELL_MORTALITY                     = 82934,
    SPELL_MORTALITY_RAID_DEBUFF         = 82890,

    // Bile O Tron
    SPELL_FINKLES_MIXTURE               = 82705,
    SPELL_FINKLES_MIXTURE_VISUAL        = 91106,
    SPELL_SYSTEM_FALURE                 = 88853,
    SPELL_REROUTE_POWER                 = 88861,

    // HC: Nefarian
    SPELL_MOCKING_SHADOWS               = 91307,
};

Position const BilePositions[6] =
{
    {-135.795151f, 15.569847f, 73.165909f, 4.646072f},
    {-129.176636f, -10.488489f, 73.079071f, 5.631739f},
    {-106.186249f, -18.533386f, 72.798332f, 1.555510f},
    {-77.951973f, 0.702321f, 73.093552f, 1.509125f},
    {-77.466125f, 31.038124f, 73.177673f, 4.489712f},
    {-120.426445f, 34.491863f, 72.057610f, 4.116642f},
};

class boss_chimaeron : public CreatureScript
{
public:
    boss_chimaeron() : CreatureScript("boss_chimaeron") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_chimaeronAI (creature);
    }

    struct boss_chimaeronAI: public BossAI
    {
        boss_chimaeronAI(Creature* creature) : BossAI(creature, DATA_CHIMAERON)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;
        uint8 phase;

        void Reset()
        {
            events.Reset();
            me->SetReactState(REACT_PASSIVE);
            phase = 1;

            me->RemoveAura(SPELL_DOUBLE_ATTACK);
            me->RemoveAura(SPELL_MORTALITY);
            me->RemoveAura(SPELL_MORTALITY_RAID_DEBUFF);
            me->RemoveAura(SPELL_MOCKING_SHADOWS);
            
            if(Creature* finkle_einhorn = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_FINKLE_EINHORN)))
                finkle_einhorn->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

            if(Creature* bile_o_tron = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_BILE_O_TRON)))
                bile_o_tron->AI()->DoAction(ACTION_BILE_O_TRON_RESET);

            _Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            if(me->GetMap()->IsHeroic())
                DoCast(me, SPELL_MOCKING_SHADOWS);

            events.ScheduleEvent(EVENT_MASSACRE, urand(30000,35000));
            events.ScheduleEvent(EVENT_DOUBLE_ATTACK, urand(13000,15000));
            events.ScheduleEvent(EVENT_CAUSTIC_SLIME, urand(10000,12000));
            events.ScheduleEvent(EVENT_BREAK, urand(14000,16000));

            _EnterCombat();
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STAT_CASTING))
                return;

            if(me->GetHealthPct() < 20 && phase == 1)
            {
                phase = 2;

                DoCast(me, SPELL_MORTALITY);
                DoCastAOE(SPELL_MORTALITY_RAID_DEBUFF);

                events.CancelEvent(EVENT_MASSACRE);
                events.CancelEvent(EVENT_BREAK);
                events.CancelEvent(EVENT_CAUSTIC_SLIME);
            }

            events.Update(diff);
            _DoAggroPulse(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {

                case EVENT_MASSACRE:
                    DoCastVictim(SPELL_MASSACRE);
                    DoCast(me,SPELL_FEUD);

                    if(Creature* bile_o_tron = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_BILE_O_TRON)))
                        bile_o_tron->AI()->DoAction(ACTION_BILE_O_TRON_SYSTEM_FAILURE);

                    events.ScheduleEvent(EVENT_SEC_MASSACRE, 11000);
                    events.ScheduleEvent(EVENT_MASSACRE, urand(90000,95000));
                    break;

                case EVENT_SEC_MASSACRE:
                    DoCastVictim(SPELL_MASSACRE);
                    DoCast(me,SPELL_FEUD);
                    break;

                case EVENT_DOUBLE_ATTACK:
                    DoCast(me, SPELL_DOUBLE_ATTACK);
                    events.ScheduleEvent(EVENT_DOUBLE_ATTACK, urand(13000,15000));
                    break;

                case EVENT_CAUSTIC_SLIME:
                    DoCastAOE(SPELL_CAUSTIC_SLIME);
                    events.ScheduleEvent(EVENT_CAUSTIC_SLIME, urand(10000,12000));
                    break;

                case EVENT_BREAK:
                    DoCastVictim(SPELL_BREAK);
                    events.ScheduleEvent(EVENT_BREAK, 14000);
                    break;

                default:
                    break;
                }
            }		

            DoMeleeAttackIfReady();
        }

        void DamageTaken(Unit* who, uint32& damage)
        {
            if(me->HasReactState(REACT_PASSIVE))
            {
                me->SetReactState(REACT_AGGRESSIVE);
                DoZoneInCombat(me);
            }
        }

        void JustDied(Unit* /*killer*/)
        {
            if(Creature* bile_o_tron = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_BILE_O_TRON)))
            {
                bile_o_tron->RemoveAllAuras();
                bile_o_tron->GetMotionMaster()->MoveIdle();
            }

            _JustDied();
        }
    };
};

class mob_finkle_einhorn : public CreatureScript
{
public:
    mob_finkle_einhorn() : CreatureScript("mob_finkle_einhorn") { }

    bool OnGossipHello(Player* pPlayer, Creature* creature)
    {

        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Help us with your Bille-O-Tron 800!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(1,creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* creature, uint32 uiSender, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();

        pPlayer->CLOSE_GOSSIP_MENU();

        if(InstanceScript* instance = creature->GetInstanceScript())
        {
            if(Creature* bile_o_tron = ObjectAccessor::GetCreature(*creature,instance->GetData64(NPC_BILE_O_TRON)))
            {
                bile_o_tron->AI()->DoAction(ACTION_BILE_O_TRON_EVENT_START);
                creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }
        }
        return true;
    }
};

class mob_bile_o_tron : public CreatureScript
{
public:
    mob_bile_o_tron() : CreatureScript("mob_bile_o_tron") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_bile_o_tronAI (creature);
    }

    struct mob_bile_o_tronAI : public ScriptedAI
    {
        mob_bile_o_tronAI(Creature* creature) : ScriptedAI(creature), waypoint(7), uiSystemFailureTimer(0), activated(false)
        {
            instance = creature->GetInstanceScript();
            creature->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);

            isFailureActive = false;
        }

        InstanceScript* instance;
        uint8 waypoint;
        uint32 uiSystemFailureTimer;
        bool activated;
        bool isFailureActive;

        void UpdateAI(const uint32 diff)
        {
            if(!activated)
                return;

            if(!me->HasAura(SPELL_SYSTEM_FALURE) && !me->HasAura(SPELL_FINKLES_MIXTURE))
                me->AddAura(SPELL_FINKLES_MIXTURE, me);

            if(uiSystemFailureTimer <= diff && isFailureActive)
            { // Reroute Power
                me->RemoveAura(SPELL_SYSTEM_FALURE);
                me->GetMotionMaster()->MovePoint(1,BilePositions[waypoint]);

                isFailureActive = false;
                DoCast(me,SPELL_FINKLES_MIXTURE_VISUAL,true);
            }
            else uiSystemFailureTimer -= diff;
        }

        void DoAction(const int32 action)
        {

            Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();

            switch(action)
            {

            case ACTION_BILE_O_TRON_EVENT_START:
                DoCast(me,SPELL_FINKLES_MIXTURE_VISUAL,true);
                waypoint = 8;
                me->GetMotionMaster()->MovePoint(1, BilePositions[0]);
                activated = true;
                isFailureActive = false;
                break;

            case ACTION_BILE_O_TRON_SYSTEM_FAILURE:
                if(!activated)
                    break;

                if(Creature* finkle_einhorn = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_FINKLE_EINHORN)))
                    finkle_einhorn->MonsterYell("Poor little fella.",0,0);

                me->RemoveAllAuras();
                DoCast(me,SPELL_REROUTE_POWER, true);
                DoCast(me,SPELL_SYSTEM_FALURE, true);
                isFailureActive = true;
                uiSystemFailureTimer = 26000;
                break;

            case ACTION_BILE_O_TRON_RESET:
                me->RemoveAllAuras();
                me->GetMotionMaster()->MoveTargetedHome();
                waypoint = 7;
                uiSystemFailureTimer = 0;
                activated = false;
                break;
            }
        }

        void JustReachedHome()
        {
            me->RemoveAllAuras();
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE || waypoint == 7)
                return;

            if(waypoint >= 5)
                waypoint = 0;
            else
                waypoint++;

            me->GetMotionMaster()->MovePoint(1,BilePositions[waypoint]);               
        }
    };
};

void AddSC_boss_chimaeron()
{
    new boss_chimaeron();
    new mob_finkle_einhorn();
    new mob_bile_o_tron();
}
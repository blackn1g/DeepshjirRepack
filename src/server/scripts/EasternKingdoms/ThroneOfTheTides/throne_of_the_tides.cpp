/*
* Copyright (C) 2005 - 2011 MaNGOS <http://www.getmangos.org/>
*
* Copyright (C) 2008 - 2011 TrinityCore <http://www.trinitycore.org/>
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

#include "ScriptPCH.h"
#include "throne_of_the_tides.h"

Position const EventPositions[] =
{{-139.756f, 802.663f, 796.641f, 3.136672f}, // Start Point
{-132.084763f, 798.554138f, 796.976257f, 3.155597f}, // Invader Spawnpoints
{-132.03f, 806.99f, 797.f, 3.06f},
{-103.577f, 806.394f, 796.965f, 3.06048f},
{-72.4102f, 798.265f, 796.97f, 3.12723f}, // Spiritmender Spawnpoints
{-72.2161f, 806.563f, 796.966f, 3.09581f},
{-104.031f, 798.42f, 796.957f, 3.14216f},
{-45.626f, 802.385f, 797.117f, 3.1178f}, // Murloc Spawnposition
{23.623f, 802.433f, 806.317f, 6.254f}}; // Naz'jar despawn Point

class mob_lady_nazjar_event : public CreatureScript
{
public:
    mob_lady_nazjar_event() : CreatureScript("mob_lady_nazjar_event") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_lady_nazjar_eventAI (creature);
    }

    struct mob_lady_nazjar_eventAI : public ScriptedAI
    {
        mob_lady_nazjar_eventAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        bool eventProgress;
        uint8 nextStep;
        uint32 timer;

        void Reset()
        {
            if(instance->GetData(DATA_LADY_NAZJAR) == DONE)
                me->ForcedDespawn(0);

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetReactState(REACT_PASSIVE);

            eventProgress = false;
            nextStep = 0;

            timer = 1000;
        }

        void UpdateAI(const uint32 diff)
        {
            if(!instance)
                return;

            if (!eventProgress)
            {
                if (timer <= diff)
                {
                    if (Player* target = me->FindNearestPlayer(143.0f, true))
                    {
                        if (!target->isGameMaster() /*&& target->GetDistance(me) < 143 */&& target->GetDistance(EventPositions[0]) < 15.f)
                        {
                            if(GameObject* door = me->FindNearestGameObject(GO_COMMANDER_ULTHOK_DOOR, 30.0f))
                                instance->HandleGameObject(0, false, door);

                            me->MonsterYell("Armies of the depths, wash over our enemies as a tide of death!",0,0);

                            eventProgress = true;

                            for(uint8 i = 1; i <= 3; i++)
                            {
                                if(Creature* invader = me->SummonCreature(NPC_NAZJAR_INVADER,EventPositions[i],TEMPSUMMON_MANUAL_DESPAWN))
                                    if(i != 3)
                                        invader->AI()->DoZoneInCombat(invader);
                            }

                            for(uint8 i = 4; i <= 6; i++)
                            {
                                if(Creature* spiritmender = me->SummonCreature(NPC_NAZJAR_SPIRITMENDER,EventPositions[i],TEMPSUMMON_MANUAL_DESPAWN))
                                    spiritmender->AI()->DoZoneInCombat(spiritmender);
                            }

                            nextStep = 4;
                        }
                    }

                    timer = 1000;

                } else timer -= diff;

            }else
            {
                if (timer <= diff)
                {
                    if(Player* target = me->FindNearestPlayer(143.0f, true))
                        if (!target->isGameMaster() && target->GetDistance(me) < 143)
                        {
                            for(uint8 i = 0; i <= 6; i++)
                            {
                                if(Creature* murloc = me->SummonCreature(NPC_DEEP_MURLOC_DRUDGE, EventPositions[7], TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000))
                                {
                                    murloc->AI()->DoZoneInCombat(murloc);
                                    murloc->GetMotionMaster()->MovePoint(0, EventPositions[0]);
                                }
                            }
                        }

                        timer = 16000;

                } else timer -= diff;
            }
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
        {
            if(summon->GetEntry() != NPC_DEEP_MURLOC_DRUDGE)
                nextStep--;

            if(nextStep == 0)
            {
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);

                if(GameObject* door = me->FindNearestGameObject(GO_COMMANDER_ULTHOK_DOOR, 30.0f))
                    instance->HandleGameObject(0, true, door);

                me->MonsterYell("Meddlesome gnats! You think us defeated so easily?",0,0);
                me->GetMotionMaster()->MovePoint(0, EventPositions[8]);
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE || id != 0)
                return;

            me->DisappearAndDie();
        }
    };
};

void AddSC_throne_of_the_tides()
{
    new mob_lady_nazjar_event();
}
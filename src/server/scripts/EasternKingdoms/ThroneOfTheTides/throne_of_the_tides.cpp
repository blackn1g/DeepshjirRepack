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

Position const Eventpositions[] =
{{-126.425f, 816.384f, 796.188f, 3.007515f}, // Invader
{-126.516f, 797.748f, 797.104f, 2.57005f},
{-125.660f, 786.393f, 796.736f, 3.113f},
{-126.098f, 806.249f, 797.098f, 3.55966f}};

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

        uint8 eventProgress;
        uint8 nextStep;
        uint32 timer;

        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetReactState(REACT_PASSIVE);

            eventProgress = 0;
            nextStep = 0;

            timer = 1000;
        }

        void UpdateAI(const uint32 diff)
        {
            if(!instance)
                return;

            if(eventProgress != 0 && !me->isInCombat())
                Reset();

            if (eventProgress == 0)
            {
                if (timer <= diff)
                {
                    if (Player* target = me->FindNearestPlayer(143.0f, true))
                    {
                        if (!target->isGameMaster() && target->GetDistance(me) < 143)
                        {
                            me->MonsterYell("Armies of the depths, wash over our enemies as a tide of death!",0,0);
                            DoZoneInCombat(me);

                            eventProgress = 0;
                            ProcessStep();
                        }
                    }

                    if (eventProgress == 0)
                        timer = 1000;

                } else timer -= diff;
            }
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            if(nextStep > 0)
            {
                nextStep--;

                if(nextStep == 0 && eventProgress != 0)
                    ProcessStep();
            }
        }

        void JustSummoned(Creature* summon)
        {
            if(eventProgress != 0)
                nextStep++;
        }

    private:
        void ProcessStep()
        {
            eventProgress++;

            switch(eventProgress)
            {
            case 1:
                for(uint8 i = 0; i <= 2; i+= 2)
                {
                    if(Creature* invader = me->SummonCreature(NPC_NAZJAR_INVADER,Eventpositions[i],TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT))
                    {
                        invader->GetMotionMaster()->MoveJump(Eventpositions[i+1].GetPositionX(),Eventpositions[i+1].GetPositionY(),Eventpositions[i+1].GetPositionZ(),.3f,.3f);
                    }
                }

                break;

            case 2:

                break;

            }
        }
    };
};

void AddSC_throne_of_the_tides()
{
    new mob_lady_nazjar_event();
}
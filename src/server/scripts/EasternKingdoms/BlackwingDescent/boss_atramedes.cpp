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

#include "ScriptPCH.h"
#include "blackwing_descent.h"

enum Events
{
    // Groundphase
    EVENT_SONAR_PULSE = 1,
    EVENT_MODULATION,
    EVENT_SONIC_BREATH,
    EVENT_SEARING_FLAMES,
};

enum Spells
{
    // Pre Event
    SPELL_GLOW_ORANGE_GENERETIC     = 80857,

    // Bossfight
    // Groundphase   

};

/*

- Sonar Pulse
77672, 92411, 92412, 92413 dbm warining and sound & emote (maintrigger?)
77673 nothing
77674, 92414, 92415, 92416 visual effect
77675 nothing
92417 nothing
92418 nothing
92419 nothing
92519 nothing
92526 nothing
92530 summons for 3 seconds
92531 "
92532 "
92533 "
*/

enum ScriptTexts
{
    SAY_EVENT                     = -1851029,
};

class boss_atramedes : public CreatureScript
{
public:
    boss_atramedes() : CreatureScript("boss_atramedes") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_atramedesAI(creature);
    }

    struct boss_atramedesAI : public BossAI
    {
        boss_atramedesAI(Creature* creature) : BossAI(creature, DATA_ATRAMEDES)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        bool isOnGround;

        void Reset()
        {
            isOnGround = true;
            DespawnMinions();

            _Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            initEvents();

            _EnterCombat();
        }

        void initEvents(bool onGround = true)
        {
            events.Reset();

            if(onGround)
            {
                events.ScheduleEvent(EVENT_SONAR_PULSE, 20000);
                //events.ScheduleEvent(EVENT_MODULATION, 20000);
                //events.ScheduleEvent(EVENT_SONAR_PULSE, 20000);
                //events.ScheduleEvent(EVENT_SONAR_PULSE, 20000);

            }else
            {

            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STAT_CASTING))
                return;

            events.Update(diff);
            _DoAggroPulse(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {

                    // Ground Phase
                case EVENT_SONAR_PULSE:
                    for(uint8 i=0; i<=7; i++)
                        me->SummonCreature(NPC_SONAR_PULSE,me->GetPosition(),TEMPSUMMON_TIMED_DESPAWN,30000);

                    events.ScheduleEvent(EVENT_SONAR_PULSE, 50000);
                    break;
                case EVENT_MODULATION:

                    events.ScheduleEvent(EVENT_MODULATION, 20000);
                    break;
                case EVENT_SONIC_BREATH:

                    events.ScheduleEvent(EVENT_SONIC_BREATH, 20000);
                    break;
                case EVENT_SEARING_FLAMES:

                    events.ScheduleEvent(EVENT_SONAR_PULSE, 20000);
                    break;

                default:
                    break;
                }
            }		

            if(isOnGround)
                DoMeleeAttackIfReady();
        }

        void JustSummoned(Creature* summon)
        {
            summon->AI()->SetMinionInCombat();
        }

        void JustDied(Unit* /*killer*/)
        {
            //DoScriptText(SAY_DEATH, me);
            DespawnMinions();

            _JustDied();
        }

    private:
        inline void DespawnMinions()
        {
            me->DespawnCreaturesInArea(NPC_SONAR_PULSE);
        }
    };
};

class mob_sonar_pulse : public CreatureScript
{
public:
    mob_sonar_pulse() : CreatureScript("mob_sonar_pulse") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_sonar_pulseAI (creature);
    }

    struct mob_sonar_pulseAI : public ScriptedAI
    {
        mob_sonar_pulseAI(Creature* creature) : ScriptedAI(creature)
        {
            timerChangeTarget = 1000;
            creature->SetReactState(REACT_PASSIVE);
        }

        uint32 timerChangeTarget;

        void UpdateAI(const uint32 diff)
        {
            if (timerChangeTarget <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                    me->GetMotionMaster()->MoveFollow(target, 0, 0);

                timerChangeTarget = 10000;

            } else timerChangeTarget -= diff;
        }
    };
};

uint16 const times[16] =
{
    0, 10000, 10000, 5000, 6000, 7500, 5000, 7500,
    5000, 6000, 4000, 5000, 7000, 700, 6000, 4000
};

uint16 const adds[8] =
{
    43119, 43128, 43130, 43122, 43127, 43125, 43129, 43126
};


class mob_maloriak_atramedes_event : public CreatureScript
{
public:
    mob_maloriak_atramedes_event() : CreatureScript("mob_maloriak_atramedes_event") { }

    struct mob_maloriak_atramedes_eventAI : public ScriptedAI
    {
        mob_maloriak_atramedes_eventAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();

            maloriak = creature;
        }

        InstanceScript* instance;
        uint32 timer;
        uint8 eventStep;

        Creature* nefarian;
        Creature* atramedes;
        Creature* maloriak;

        void Reset()
        {
            timer = 1000;
            eventStep = 0;
            me->SetVisible(false);
        };

        void UpdateAI(uint32 const diff) 
        {
            if (timer <= diff)
            {
                if(eventStep == 0)
                {
                    timer = times[eventStep];

                    // Check weather all Adds are death
                    for(uint8 i=0; i<=7; i++)
                        if(me->FindNearestCreature(adds[i], 100.f,true))
                            return;

                    if(atramedes = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_PRE_ATRAMEDES)))
                    {   
                        atramedes->SetPhaseMask(1, true);
                        eventStep = 1;
                        me->SetVisible(true);
                        nefarian = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_PRE_NEFARIAN));
                        nefarian->SetPhaseMask(1, true);

                        // If something went wrong...
                        if(Creature* atramedesnew = ObjectAccessor::GetCreature(*me,instance->GetData64(BOSS_ATRAMEDES)))
                            atramedesnew->SetPhaseMask(2, true);
                    }
                }else
                {

                    switch(eventStep)
                    {
                        // Nefarian
                    case 1:
                        Say(nefarian);
                        break;
                    case 3:
                        Say(nefarian);
                        break;
                    case 9:
                    case 10:
                    case 12:
                    case 13:
                    case 14:
                        Say(nefarian);
                        break;

                        // Maloriak
                    case 2:
                    case 4:
                    case 6:
                    case 11:
                    case 15:
                        Say(maloriak);
                        break;
                    case 5:
                        atramedes->GetMotionMaster()->MovePoint(0, 188.951569f, -224.604065f, 75.4534f);
                        Say(maloriak);
                        break;
                    case 7:
                        me->AddAura(SPELL_GLOW_ORANGE_GENERETIC, atramedes);
                        Say(maloriak);
                        break;
                    case 8:
                        atramedes->ForcedDespawn();

                        if(Creature* atramedesnew = ObjectAccessor::GetCreature(*me,instance->GetData64(BOSS_ATRAMEDES)))
                            atramedesnew->SetPhaseMask(1, true);

                        atramedes->RemoveAura(SPELL_GLOW_ORANGE_GENERETIC);
                        Say(maloriak);
                        break;

                        // Both
                    case 16:
                        nefarian->ForcedDespawn();
                        maloriak->ForcedDespawn();
                        break;
                    }

                    timer = times[eventStep];

                    if(eventStep<16)
                        eventStep++;
                }

            } else timer -= diff;
        }

        void Say(Creature* cr)
        {
            DoScriptText(SAY_EVENT - eventStep, cr);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_maloriak_atramedes_eventAI(creature);
    }
};

void AddSC_boss_atramedes()
{
    new boss_atramedes();
    new mob_sonar_pulse();
    new mob_maloriak_atramedes_event();
}
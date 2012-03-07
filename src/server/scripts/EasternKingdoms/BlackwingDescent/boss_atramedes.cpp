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
                events.ScheduleEvent(EVENT_SONAR_PULSE, 5000);
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

            if(summon->GetEntry() == NPC_SONAR_PULSE)
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                    summon->GetMotionMaster()->MoveChase(target, 0, 0);
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
            timerChangeTarget = 13000;
            creature->SetReactState(REACT_PASSIVE);
        }

        uint32 timerChangeTarget;

        void UpdateAI(const uint32 diff)
        {
            if (timerChangeTarget <= diff)
            {
                if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                    me->GetMotionMaster()->MoveFollow(target, 3, 2);

                timerChangeTarget = 13000;

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

        bool eventProcessed;

        void Reset()
        {
            timer = 1000;
            eventStep = 0;

            me->DespawnCreaturesInArea(NPC_PRE_LIGHT_EFFECT);
            me->SummonCreature(NPC_PRE_LIGHT_EFFECT, 126.22007f, -231.013306f, 75.453453f, 3.117948f);
            me->SummonCreature(NPC_PRE_LIGHT_EFFECT, 125.573227f, -221.80191f, 75.453453f, 2.945161f);
        };

        void UpdateAI(uint32 const diff) 
        {
            if (timer <= diff)
            {
                if(eventStep == 0)
                {
                    timer = times[eventStep];

                    if (Player* target = me->FindNearestPlayer(40.f, true))
                        if (target->GetDistance(me) < 35.f)
                        {
                            if(atramedes = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_PRE_ATRAMEDES)))
                            {
                                eventStep = 1;
                                nefarian = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_PRE_NEFARIAN));
                            }
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
                        atramedes->GetMotionMaster()->MovePoint(0,125.996674f, -227.132782f, 75.453979f);
                        Say(maloriak);
                        break;
                    case 7:
                        me->AddAura(SPELL_GLOW_ORANGE_GENERETIC, atramedes);
                        Say(maloriak);
                        break;
                    case 8:
                        atramedes->RemoveAura(SPELL_GLOW_ORANGE_GENERETIC);
                        Say(maloriak);
                        break;

                        // Both
                    case 16:
                        atramedes->DespawnOrUnsummon();
                        nefarian->DespawnOrUnsummon();
                        me->DespawnCreaturesInArea(NPC_PRE_LIGHT_EFFECT);

                        if(GameObject* throne = me->FindNearestGameObject(GOB_NEFARIANS_THRONE,20))
                            throne->Delete();

                        maloriak->DespawnOrUnsummon();
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
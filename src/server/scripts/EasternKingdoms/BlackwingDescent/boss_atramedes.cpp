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

enum Spells
{
    // Pre Event
    SPELL_GLOW_ORANGE_GENERETIC     = 80857,

    // Bossfight
};

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

            _Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            //events.ScheduleEvent(EVENT_TEST, urand(10000,12000));

            _EnterCombat();
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STAT_CASTING))
                return;

            events.Update(diff);
            _DoAggroPulse(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                /*switch (eventId)
                {

                case EVENT_TEST:
                DoCastVictim(SPELL_ENFEEBLING_BLOW);
                events.ScheduleEvent(EVENT_ENFEEBLING_BLOW, urand(19000,24000));
                break;

                default:
                break;
                }*/
            }		

            if(isOnGround)
                DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/)
        {
            //DoScriptText(SAY_DEATH, me);
            DespawnMinions();

            _JustDied();
        }

        void initEvents(bool onGround = true)
        {
            events.Reset();

            if(onGround)
            {

            }else
            {

            }
        }

    private:
        inline void DespawnMinions()
        {

        }

        void DespawnCreatures(uint32 entry)
        {
            std::list<Creature*> creatures;
            GetCreatureListWithEntryInGrid(creatures, me, entry, 100.0f);

            if (creatures.empty())
                return;

            for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                (*iter)->DespawnOrUnsummon();
        }
    };
};

uint16 const times[16] =
{
    0, 9000, 8000, 5000, 6000, 7500, 5000, 7500,
    5000, 6000, 4000, 5000, 6000, 5500, 6000, 4000
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
    new mob_maloriak_atramedes_event();
}
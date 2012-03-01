/*
* Copyright (C) 2010-2012 Project SkyFire <http://www.projectskyfire.org/>
* Copyright (C) 2011-2012 ArkCORE <http://www.arkania.net/>
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
#include "halls_of_origination.h"

enum Texts
{
    SAY_AGGRO       = 0,    //More carrion for the swarm...
    SAY_DEATH       = 1,    //Ptah... is... no more...
    SAY_CAST        = 2,    //Dust to dust.
};

enum Npcs
{
    BOSS_PTAH       = 39428,
    NPC_HORROR      = 40810,
    NPC_SCARAB      = 40458,
};

enum Spells
{
    //Ptah Spells
    SPELL_FLAME_BOLT        = 77370,
    SPELL_RAGING_SMASH      = 83650,
    SPELL_EARTH_PEAK        = 75339,
    SPELL_DUST_MOVE         = 75547,
    SPELL_DUST_VORTEX       = 93570,
    SPELL_SINKING_SAND      = 75547,    //or 75546
    //Horror spells
    SPELL_CRUNCH            = 75453,
};

enum Events
{
    //Ptah events
    EVENT_FLAME_BOLT,
    EVENT_RAGING_SMASH,
    EVENT_EARTH_POINT,
    EVENT_SUMMON,
    EVENT_DUST_MOVE,
    EVENT_VORTEX_DUST,
    //Horror events
    EVENT_CRUNCH,
};

const Position aSpawnLocations[3] =
{
    {-530.561584f, -370.613525f, 156.935913f, 5.081081f},
    {-484.478302f, -371.117584f, 155.954208f, 4.429200f},
    {-507.319977f, -381.939392f, 154.764664f, 4.700163f},
};

class boss_ptah : public CreatureScript
{
public:
    boss_ptah() : CreatureScript("boss_ptah") {}

    struct boss_ptahAI : public BossAI
    {
        boss_ptahAI(Creature* creature) : BossAI(creature, DATA_EARTHRAGER_PTAH_EVENT), Summons(me)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        SummonList Summons;

        void Reset()
        {
            _Reset();
        }

        void EnterCombat(Unit * /*who*/)
        {
            _EnterCombat();

            events.ScheduleEvent(EVENT_FLAME_BOLT, 7500);
            events.ScheduleEvent(EVENT_RAGING_SMASH, urand(4000, 10000));
            events.ScheduleEvent(EVENT_EARTH_POINT, 8000);
            events.ScheduleEvent(EVENT_SUMMON, 50000);
            events.ScheduleEvent(EVENT_DUST_MOVE, 15000);
            events.ScheduleEvent(EVENT_VORTEX_DUST, urand(14000, 20000));

            Talk(SAY_AGGRO);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ADD, me);
        }

        void JustDied(Unit* /*killer*/)
        {
            _JustDied();

            Talk(SAY_DEATH);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_REMOVE, me);
        }

        void JustSummoned(Creature *summoned)
        {
            summoned->SetInCombatWithZone();
            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0))
                summoned->AI()->AttackStart(target);
            Summons.Summon(summoned);
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            Summons.Despawn(summon);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_FLAME_BOLT:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, true))
                        DoCast(target, SPELL_FLAME_BOLT);
                    events.ScheduleEvent(EVENT_FLAME_BOLT, 7500);
                    break;;
                case EVENT_RAGING_SMASH:
                    DoCast(me->getVictim(), SPELL_RAGING_SMASH);
                    events.ScheduleEvent(EVENT_RAGING_SMASH, urand(4000, 10000));
                    break;;
                case EVENT_EARTH_POINT:
                    Talk(SAY_CAST);
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM,1,100,true))
                        DoCast(target, SPELL_EARTH_PEAK);
                    events.ScheduleEvent(EVENT_EARTH_POINT, 8000);
                    break;;
                case EVENT_SUMMON:
                    if (IsHeroic())
                    {
                        me->SummonCreature(NPC_HORROR, aSpawnLocations[0].GetPositionX(), aSpawnLocations[0].GetPositionY(), aSpawnLocations[0].GetPositionZ(), 0.0f, TEMPSUMMON_CORPSE_DESPAWN);
                        me->SummonCreature(NPC_SCARAB, aSpawnLocations[1].GetPositionX(), aSpawnLocations[1].GetPositionY(), aSpawnLocations[1].GetPositionZ(), 0.0f, TEMPSUMMON_CORPSE_DESPAWN);
                        events.ScheduleEvent(EVENT_SUMMON, 50000);
                    }
                    break;
                case EVENT_DUST_MOVE:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, true))
                        DoCast(target, SPELL_DUST_MOVE);
                    events.ScheduleEvent(EVENT_DUST_MOVE, 15000);
                    break;
                case EVENT_VORTEX_DUST:
                    if (IsHeroic())
                    {
                        DoCastAOE(SPELL_DUST_VORTEX);
                    }
                    events.ScheduleEvent(EVENT_VORTEX_DUST, urand(14000, 20000));
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_ptahAI(creature);
    }
};

class npc_horror : public CreatureScript
{
public:
    npc_horror() : CreatureScript("npc_horror") {}

    struct npc_horrorAI : public ScriptedAI
    {
        npc_horrorAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        EventMap events;
        InstanceScript* instance;

        void EnterCombat(Unit * /*who*/)
        {
            events.ScheduleEvent(EVENT_CRUNCH, urand(6000, 7000));
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CRUNCH:
                    DoCast(me->getVictim(), SPELL_CRUNCH);
                    events.ScheduleEvent(EVENT_CRUNCH, urand(6000, 7000));
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_horrorAI(creature);
    }
};

void AddSC_boss_ptah()
{
    new boss_ptah();
    new npc_horror();
}
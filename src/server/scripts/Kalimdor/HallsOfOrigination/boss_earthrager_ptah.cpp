#include "ScriptPCH.h"
#include "halls_of_origination.h"

/*
SData: Boss Earthrager Ptah
Author: NorthStrider
SDComplete: 70%

TODO:
-implement achievement
-correct timers
-cosmetic stuff :P
-core support for spells
*/



enum Texts
{
    SAY_AGGRO       = 0,    //More carrion for the swarm...
    SAY_DEATH       = 1,    //Ptah... is... no more...
    SAY_CAST        = 2,    //Dust to dust.
    SAY_SLAY        = 3,    //HYAAAH´!
};

enum Creatures
{
    BOSS_PTAH       = 39428,
    NPC_HORROR      = 40810,
    NPC_SCARAB      = 40458,
};

enum Spells
{
    //Ptah Spells
    SPELL_FLAME_BOLT        = 77370,    //75540 on wow head its linked to this id, so maybe spell needs extra script
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

enum Achievement
{
    CAMEL       = 5294,
};

const Position aSpawnLocations[3] =
{
    {-530.561584f, -370.613525f, 156.935913f, 5.081081f},
    {-484.478302f, -371.117584f, 155.954208f, 4.429200f},
    {-507.319977f, -381.939392f, 154.764664f, 4.700163f},
};

class boss_earthrager_ptah : public CreatureScript
{
    public:
        boss_earthrager_ptah() : CreatureScript("boss_earthrager_ptah") { }

        struct boss_earthrager_ptahAI : public BossAI
        {
            boss_earthrager_ptahAI(Creature* creature) : BossAI(creature, DATA_EARTHRAGER_PTAH), Summons(me)
            {
                instance = creature->GetInstanceScript();
            }

            InstanceScript* instance;
            EventMap events;
            SummonList Summons;

            void Reset()
            {
                _Reset();
                instance->SetData(DATA_EARTHRAGER_PTAH, NOT_STARTED);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_REMOVE, me);
            }

            void EnterCombat(Unit * /*who*/)
            {
                Talk(SAY_AGGRO);
                instance->SetData(DATA_EARTHRAGER_PTAH, IN_PROGRESS);
                instance->SendEncounterUnit(ENCOUNTER_FRAME_ADD, me);
                events.ScheduleEvent(EVENT_FLAME_BOLT, 7500);
                events.ScheduleEvent(EVENT_RAGING_SMASH, urand(4000, 10000));
                events.ScheduleEvent(EVENT_EARTH_POINT, 8000);
                events.ScheduleEvent(EVENT_SUMMON, 50000);
                events.ScheduleEvent(EVENT_DUST_MOVE, 15000);
            }

            void KilledUnit(Unit* victim)
            {
                Talk(SAY_SLAY);
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
                Talk(SAY_DEATH);
                instance->SetData(DATA_EARTHRAGER_PTAH, DONE);
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

                if (me->HealthBelowPct(50))
                {
                    me->SummonCreature(NPC_HORROR, aSpawnLocations[0].GetPositionX(), aSpawnLocations[0].GetPositionY(), aSpawnLocations[0].GetPositionZ(), 0.0f, TEMPSUMMON_CORPSE_DESPAWN);
                    me->SummonCreature(NPC_SCARAB, aSpawnLocations[1].GetPositionX(), aSpawnLocations[1].GetPositionY(), aSpawnLocations[1].GetPositionZ(), 0.0f, TEMPSUMMON_CORPSE_DESPAWN);
                    if (IsHeroic())
                        DoCastAOE(SPELL_DUST_VORTEX);
                }

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FLAME_BOLT:
                            DoCastAOE(SPELL_FLAME_BOLT);
                            events.ScheduleEvent(EVENT_FLAME_BOLT, 7500);
                            break;
                        case EVENT_RAGING_SMASH:
                            DoCast(me->getVictim(), SPELL_RAGING_SMASH);
                            events.ScheduleEvent(EVENT_RAGING_SMASH, urand(4000, 10000));
                            break;
                        case EVENT_EARTH_POINT:
                            Talk(SAY_CAST);
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM,1,100,true))
                                DoCast(target, SPELL_EARTH_PEAK);
                            events.ScheduleEvent(EVENT_EARTH_POINT, 8000);
                            break;
                        case EVENT_DUST_MOVE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, true))
                                DoCast(target, SPELL_DUST_MOVE);
                            events.ScheduleEvent(EVENT_DUST_MOVE, 15000);
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_earthrager_ptahAI(creature);
    }
};

void AddSC_boss_earthrager_ptah()
{
     new boss_earthrager_ptah();
}

#include "ScriptPCH.h"

class boss_chimaeron : public CreatureScript
{
public:
    boss_chimaeron() : CreatureScript("boss_chimaeron") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_chimaeronAI (creature);
    }

    struct boss_chimaeronAI : public ScriptedAI
    {
        boss_chimaeronAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
		EventMap events;
		
        void Reset()
		{
			events.Reset();
		}

        void EnterCombat(Unit* /*who*/)
		{
			me->GetMotionMaster()->MoveTargetedHome();
			
			//events.ScheduleEvent(EVENT_TEST, urand(10000,12000));
		}

        void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim() || me->HasUnitState(UNIT_STAT_CASTING))
				return;
			
			events.Update(diff);

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

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_chimaeron()
{
    new boss_chimaeron();
}
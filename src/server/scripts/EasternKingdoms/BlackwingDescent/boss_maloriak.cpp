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
#include "blackwing_descent.h"

Position const CauldronPositions[1] =
{
    {-111.704559f, -477.060272f, 73.456284f, 5.158795f},
};

class boss_maloriak : public CreatureScript
{
public:
    boss_maloriak() : CreatureScript("boss_maloriak") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_maloriakAI (creature);
    }

    struct boss_maloriakAI : public BossAI
    {
        boss_maloriakAI(Creature* creature) : BossAI(creature, DATA_MALORIAK)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
		EventMap events;
		
        void Reset()
		{
			events.Reset();

            _Reset();
		}

        void EnterCombat(Unit* /*who*/)
		{
            _EnterCombat();
			
			//events.ScheduleEvent(EVENT_TEST, urand(10000,12000));

            if(GameObject* cauldron = me->FindNearestGameObject(GOB_MALORIAKS_CAULDRON,100.f))
            {
                cauldron->SendCustomAnim(urand(0,4));
                me->MonsterSay("cauldron",0,0);
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

        void JustDied(Unit* /*killer*/)
        {
            _JustDied();
        }
    };
};

void AddSC_boss_maloriak()
{
    new boss_maloriak();
}
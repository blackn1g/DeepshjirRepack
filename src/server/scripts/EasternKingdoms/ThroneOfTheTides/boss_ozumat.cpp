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

#define GOSSIP_READY "We are ready!"

#define SAY_1 "As I purify these waters, the servants of filth will surely be stirred. Beware!"
#define SAY_2 "Patience guests. The waters are nearly cleansed."
#define SAY_3 "The beast has returned! It must not pollute my waters!"
#define SAY_DEATH "Your kind... cannot be... trusted..."
#define SAY_CLEANSED "My waters are cleansed! Drink in their power!"

Position const SpawnPositions[] =
{
    {-155.955f, 952.67f, 230.456f, 1.1f},
    {-105.204f, 979.93f, 229.862f, 2.9f},
    {-172.205f, 1012.91f, 231.33f, 5.5f},
    {-137.0736f, 973.488f, 230.355f, 2.0832f},
    {-143.2496f, 1005.569f, 229.8465f, 4.4472f},
    {-163.4171f, 978.427f, 229.31f, 0.4833f},
};

enum Spells
{
    SPELL_ENTANGLING_GRASP = 83463,
    SPELL_NEPTULON_BEAM_VISUAL = 79511,
};

class mob_neptulon : public CreatureScript
{
public:
    mob_neptulon() : CreatureScript("mob_neptulon") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_neptulonAI (creature);
    }

    struct mob_neptulonAI : public ScriptedAI
    {
        mob_neptulonAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint8 waveCount;
        uint32 generalTimer;
        uint32 playerAliveChecker;
        uint8 nextStep;

        void Reset()
        {
            if (instance)
                instance->SetData(DATA_OZUMAT, NOT_STARTED);

            me->RemoveAllAuras();
            me->CastStop();

            waveCount = 0;
            nextStep = 3;
            generalTimer = 10000;
            playerAliveChecker = 1500;
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        }

        void UpdateAI(const uint32 diff)
        {
            if(waveCount == 0)
                return;

            if (playerAliveChecker <= diff)
            {	
                if(!isPlayerAlive())
                    Reset();

                playerAliveChecker = 1500;
            } else playerAliveChecker -= diff;

            if (generalTimer <= diff)
            {	
                HandleWave();

                generalTimer = 10000;
            } else generalTimer -= diff;
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
        {
            if(summon->GetEntry() != NPC_DEEP_MURLOC_DRUDGE && nextStep != 0)
                nextStep--;

            if(nextStep == 0)
            {
                waveCount++;
                HandleWave();
            }
        }

        void HandleWave()
        {          
            if(waveCount < 7)
            {
                waveCount++;

                uint32 summonEntry;
                uint8 p = urand(0,2);

                if(waveCount == 1)
                {
                    me->MonsterSay(SAY_1,0,0);
                     DoCastAOE(SPELL_NEPTULON_BEAM_VISUAL,true);

                }else if (waveCount == 6)
                {
                    for(uint8 i = 3; i <= 5; i++)
                        if(Creature* summon = me->SummonCreature(NPC_FACELESS_SAPPER, SpawnPositions[i],TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000))
                        {                  
                            summon->AI()->DoZoneInCombat(summon);
                            summon->SetReactState(REACT_PASSIVE);
                            summon->CastSpell(me, SPELL_ENTANGLING_GRASP, true);
                        }

                    return;
                }

                switch(urand(0,2))
                {

                case 0:
                    summonEntry = NPC_DEEP_MURLOC_INVADER;
                    break;
                case 1:
                    summonEntry = NPC_UNYIELDING_BEHEMOTH;
                    break;
                case 2:
                    summonEntry = NPC_VICIOUS_MINDLASHER;
                    break;
                }

                for(uint8 i = 0; i <= (summonEntry == NPC_DEEP_MURLOC_INVADER ? 2 : 0); i++)
                    if(Creature* summon = me->SummonCreature(summonEntry, SpawnPositions[p],TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000))
                    {                  
                        summon->AI()->DoZoneInCombat(summon);
                    }

            }else if(waveCount == 7)
            {
                if(Creature* summon = me->SummonCreature(NPC_BLIGHT_BEAST, SpawnPositions[urand(0,2)],TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000))
                    summon->AI()->DoZoneInCombat(summon);

            }else if(waveCount == 8)
            {
                me->MonsterSay("final phase",0,0);
            }
        }

        bool isPlayerAlive()
        {
            Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();

            if (!PlayerList.isEmpty())
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    if(i->getSource()->isAlive() && !i->getSource()->isGameMaster())
                        return true;

            return false;
        };
    };

    bool OnGossipHello(Player* pPlayer, Creature* creature)
    {

        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_READY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(1,creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* creature, uint32 uiSender, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();

        pPlayer->CLOSE_GOSSIP_MENU();

        creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

        if (InstanceScript* instance = creature->GetInstanceScript())
            instance->SetData(DATA_OZUMAT, IN_PROGRESS);

        CAST_AI(mob_neptulon::mob_neptulonAI, creature->AI())->HandleWave();

        return true;
    }
};


class boss_ozumat : public CreatureScript
{
public:
    boss_ozumat() : CreatureScript("boss_ozumat") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_ozumatAI (creature);
    }

    struct boss_ozumatAI : public ScriptedAI
    {
        boss_ozumatAI(Creature* creature) : ScriptedAI(creature)
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

void AddSC_ozumat()
{
    new mob_neptulon();
    new boss_ozumat();
}
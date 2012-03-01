/*
* Copyright (C) 2005 - 2012 MaNGOS <http://www.getmangos.org/>
*
* Copyright (C) 2008 - 2012 TrinityCore <http://www.trinitycore.org/>
*
* Copyright (C) 2011 - 2012 ArkCORE <http://www.arkania.net/>
*
* Copyright (C) 2011 - 2012 True Blood <http://www.trueblood-servers.com/>
* By Asardial
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
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

#include "blackwing_descent.h"

#define SAY_AGGRO "Hmm... the Omnotron Defense System. Centuries ago these constructs were considered the Dwarves greatest technical achievements. With so many counters to each construct's attacks. I''ll have to rectify these designs for them to provide me any entertainement."
#define SAY_STORY "Stupid Dwarnes and you fascination with runes! Why would you create something that would help your enemy?"
#define SAY_LIGHTNING "Lightning conductor works much better in a crowd,yes?"
#define SAY_TOXITRON_CHEMICAL "Were you planning on using Toxitron's chemical to damage the other construcs?Clever plan, let me ruin that for you."
#define SAY_TOO_BAD "You would like to move away from that, wouldn't you?Too bad."
#define SAY_DEATH "Defense systems obliterated. Powering down...."

#define SAY_SHIELD_POISON "Toxitron unit shield system onlines."
#define SAY_SHIELD_FLAMME "Magmatron unit shield system onlines."
#define SAY_SHIELD_ELECTRON "Electron unit shield system onlines."
#define SAY_SHIELD_ARCANOTRON "Arcanotron unit shield system onlines."
#define SAY_ACTIVATION_TOXITRON "Toxitron unit activated."
#define SAY_ACTIVATION_MAGMATRON "Magmatron unit activated."
#define SAY_ACTIVATION_ELECTRON "Electron unit activated."
#define SAY_ACTIVATION_ARCANOTRON "Arcanotron unit activated."
#define SAY_REROUTING_ENERGY "Defense systems obliterated. Powering down...."

#define TRON_PATH 10821150

enum Spells
{
    // Omnitron
    SPELL_INACTIVE=78726,
    SPELL_SHUTTING_DOWN = 78746,
    SPELL_ACTIVATED = 78740,

    // Electron
    SPELL_LIGHTNING_CONDUCTOR = 79888,
    SPELL_ELECTRICAL_DISCHARGE = 91465,
    SPELL_UNSTABLE_SHIELD = 91447,
    // Toxitron
    SPELL_CHEMICAL_BOMB = 80157,
    SPELL_POISON_SOAKED_SHELL = 91501,
    SPELL_POISON_PROTOCOL = 91513,
    // Magmatron
    SPELL_BARRIER = 79582,
    SPELL_ACQUIRING_TARGET = 79501,
    SPELL_FLAMETHROWER = 79504,
    SPELL_INCINERATION_SECURITY_MEASURE = 79023,
    // Arcanotron
    SPELL_POWER_GENERATOR = 79624,
    SPELL_ARCANE_ANNIHILATOR = 79710,
    SPELL_POWER_CONVERSION = 79729,

    // Bombe de Poison
    SPELL_FIXER = 80094,
    SPELL_BOMBE = 80092,
};

enum Summons
{
    MOB_POISON_BOMB = 42897,
};

enum Events
{
    // General
    EVENT_SHUT_DOWN = 1,

    // Omnotron
    EVENT_ACTIVATE_NEXT_CONSTRUCT,
    EVENT_UPDATE_HEALTH,

    // Magmatron
    EVENT_ACQUIRING_TARGET,
    EVENT_INCINERATION_SECURITY_MEASURE,
    EVENT_BARRIER,
    // Toxitron
    EVENT_CHEMICAL_BOMB,
    EVENT_POISON_PROTOCOL,
    EVENT_POISON_SOAKED_SHELL,
    // Electron
    EVENT_LIGHTNING,
    EVENT_DISCHARGE,
    EVENT_UNSTABLE_SHIELD,
    // Arcanotron
    EVENT_POWER_CONVERSION,
    EVENT_POWER_GENERATOR,
    EVENT_ARCANE_ANNIHILATOR,

};

enum Actions
{
    ACTION_OMNOTRON_START_EVENT,
    ACTION_OMNOTRON_RESET,
    ACTION_OMNNOTRON_EVENT_FINISHED,

    ACTION_TRON_ACTIVATE,
    ACTION_EVENT_FAILED,
    ACTION_DEACTIVATE,
};

enum Misc
{
    DATA_IS_FIRST_TRON,
};

Position const wayPos[2] =
{
    {-315.853f, -400.56f, 213.974f,0},
    {-334.152f, -400.321f, 214.005f,0},
};

class boss_omnotron : public CreatureScript
{
public:
    boss_omnotron() : CreatureScript("boss_omnotron") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_omnotronAI (creature);
    }

    struct boss_omnotronAI : public ScriptedAI
    {
        boss_omnotronAI(Creature* creature) : ScriptedAI(creature), eventActive(false), intialized(false)
        {
            instance = creature->GetInstanceScript();
            //creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            creature->SetReactState(REACT_PASSIVE);
            creature->setFaction(16);
        }

        InstanceScript* instance;
        EventMap events;
        bool intialized;
        bool isEncounterDone;

        Creature* trons[4];

        uint8 activateTron;
        bool eventActive;

        void UpdateAI(const uint32 diff)
        {
            if(!intialized)
            {
                trons[0] = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_MAGMATRON));
                trons[1] = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_ELECTRON));
                trons[2] = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_ARCANOTRON));
                trons[3] = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_TOXITRON));

                for(uint8 i = 0; i<=3; i++)
                    if(trons[i] == NULL)
                        return;

                me->SetMaxHealth(trons[0]->GetMaxHealth());

                eventActive = true;
                intialized = true;
                DoAction(ACTION_OMNOTRON_RESET);
            }else
            {
            
            //me->MonsterSay("AI",0,0);
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {

                case EVENT_ACTIVATE_NEXT_CONSTRUCT:
                    ActivateNextTron();

                    events.ScheduleEvent(EVENT_ACTIVATE_NEXT_CONSTRUCT, 45000);
                    break;

                case EVENT_UPDATE_HEALTH:
                    for(uint8 i = 0; i<=3; i++)
                        trons[i]->SetHealth(me->GetHealth());

                    me->MonsterSay("Updated",0,0);

                    events.ScheduleEvent(EVENT_UPDATE_HEALTH, 1000);
                    break;

                default:
                    break;
                }
            }		
            }
        }

        void DoAction(const int32 action)
        {
            switch(action)
            {
            case ACTION_OMNOTRON_START_EVENT:
                // Start Encounter

                /*if(isEncounterDone)
                    return;*/

                if (instance)
                    instance->SetData(DATA_OMNOTRON_DEFENSE_SYSTEM, IN_PROGRESS);

                eventActive = true;

                if(me->GetMap()->IsHeroic())
                    me->SummonCreature(NPC_NEFARIAN_HELPER_HEROIC,-302.121f, -349.35f, 220.48f, 4.682203f,TEMPSUMMON_MANUAL_DESPAWN);

                me->MonsterSay("Event Scheduled",0,0);
                events.ScheduleEvent(EVENT_ACTIVATE_NEXT_CONSTRUCT, 15000);
                events.ScheduleEvent(EVENT_UPDATE_HEALTH, 1000);
                break;

            case ACTION_OMNOTRON_RESET:
                // Resets Encounter

                if(eventActive)
                {
                    me->SetFullHealth();
                    eventActive = false;

                    DespawnMinions();
                    events.Reset();

                    ResetTrons();

                    if (instance)
                        instance->SetData(DATA_OMNOTRON_DEFENSE_SYSTEM, FAIL);
                }
                break;

            case ACTION_OMNNOTRON_EVENT_FINISHED:

                if(isEncounterDone)
                    return;
                else
                    isEncounterDone = true;

                DespawnMinions();

                for(uint8 i = 0; i<=3; i++)
                    trons[i]->setDeathState(DEAD);

                if (instance)
                    instance->SetData(DATA_OMNOTRON_DEFENSE_SYSTEM, DONE);

                eventActive = false;
                break;

            default:
                break;
            }
        }

        void ResetTrons()
        {
            for(uint8 i = 0; i<=3; i++)
                if(trons[i] == NULL)
                    return;

            for(uint8 i = 0; i<=3; i++)
                trons[i]->AI()->SetData(DATA_IS_FIRST_TRON, 0);

            uint8 a = urand(15,25);
            Creature* tronCache;
            for(uint8 i = 0; i<=a; i++)
            {
                uint8 moveDest = urand(0,3);
                uint8 moveTarget = urand(0,3);

                tronCache = trons[moveTarget];
                trons[moveTarget] = trons[moveDest];
                trons[moveDest] = tronCache;
            }

            /*
            // ## DEBUG ##

            Creature* tronCache;
            uint8 moveDest = 3;
            uint8 moveTarget = 0;

            tronCache = trons[moveTarget];
            trons[moveTarget] = trons[moveDest];
            trons[moveDest] = tronCache;

            // ## END DEBUG ##
            */

            trons[0]->AI()->SetData(DATA_IS_FIRST_TRON, 1);

            for(uint8 i = 0; i<=3; i++)
                trons[i]->AI()->DoAction(ACTION_EVENT_FAILED);
        };

        void ActivateNextTron()
        {
            for(uint8 i = 0; i<=3; i++)
                if(trons[i] == NULL)
                    return;

            // Select next Tron
            Creature* tronCache;

            tronCache = trons[3];
            trons[3] = trons[0]; 
            trons[0] = tronCache;

            // Activate Tron
            trons[0]->AI()->DoAction(ACTION_TRON_ACTIVATE);
            return;
        }

    private:
        inline void DespawnMinions()
        {
            DespawnCreatures(NPC_POISON_BOMB);
            DespawnCreatures(NPC_POISON_CLOUD);
            DespawnCreatures(NPC_NEFARIAN_HELPER_HEROIC);
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

class boss_trons : public CreatureScript
{
public:
    boss_trons() : CreatureScript("boss_trons") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_tronsAI (creature);
    }

    struct boss_tronsAI : public ScriptedAI
    {
        boss_tronsAI(Creature* creature) : ScriptedAI(creature), activated(false), isFirstTron(false)
        {
            instance = creature->GetInstanceScript();

            creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            creature->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);

            homePosition = creature->GetHomePosition();
        }

        InstanceScript* instance;
        EventMap events;
        bool activated;
        bool isFirstTron;
        bool isMovingHome;
        Position homePosition;
        Creature* omnotron;

        void EnterCombat(Unit * who)
        {
            if(isFirstTron)
            {
                if (Creature* omnotron = ObjectAccessor::GetCreature(*me,instance->GetData64(BOSS_OMNOTRON)))
                    omnotron->AI()->DoAction(ACTION_OMNOTRON_START_EVENT);

                DoAction(ACTION_TRON_ACTIVATE);
            }
        }

        void JustDied(Unit* /*Killer*/)
        {
            me->MonsterYell(SAY_DEATH, 0, 0);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE || id > 2)
                return;

            me->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);

            switch(id)
            {
            case 0:
                if(!isFirstTron && isMovingHome)
                {
                    isMovingHome = false;
                    // me->GetMotionMaster()->MovementExpired(true);
                    // me->GetMotionMaster()->Clear(true);
                    me->GetMotionMaster()->MoveTargetedHome();
                    me->SetOrientation(homePosition.GetOrientation());
                    WorldPacket data;
                    me->BuildHeartBeatMsg(&data);
                    me->SendMessageToSet(&data, false);
                    me->AddAura(SPELL_INACTIVE, me);
                    //DoCast(me,SPELL_SHUTTING_DOWN);
                }
                break;
            case 1: 
                me->GetMotionMaster()->MovePoint(2, wayPos[1]);
                break;
            case 2:
                me->GetMotionMaster()->MovePoint(1, wayPos[0]);
                break;
            }

        }

        void Reset()
        {
            if (Creature* omnotron = ObjectAccessor::GetCreature(*me,instance->GetData64(BOSS_OMNOTRON)))
                omnotron->AI()->DoAction(ACTION_OMNOTRON_RESET);
        };

        void DoAction(const int32 action)
        {
            events.Reset();

            switch(action)
            {
            case ACTION_TRON_ACTIVATE:
                me->RemoveAllAuras();
                me->SetReactState(REACT_AGGRESSIVE);
                DoZoneInCombat(me);
                me->AddAura(SPELL_ACTIVATED, me);
                activated = true;
                isMovingHome = false;

                omnotron = ObjectAccessor::GetCreature(*me,instance->GetData64(BOSS_OMNOTRON));

                // Intialize Events
                switch(me->GetEntry())
                {

                case NPC_MAGMATRON:
                    events.ScheduleEvent(EVENT_ACQUIRING_TARGET, 15000);
                    events.ScheduleEvent(EVENT_INCINERATION_SECURITY_MEASURE, 27000);
                    events.ScheduleEvent(EVENT_BARRIER, 30000);
                    break;

                case NPC_TOXITRON:
                    events.ScheduleEvent(EVENT_CHEMICAL_BOMB, 30000);
                    events.ScheduleEvent(EVENT_POISON_PROTOCOL, 41500);
                    events.ScheduleEvent(EVENT_POISON_SOAKED_SHELL, 65000);
                    break;

                case NPC_ELECTRON:
                    events.ScheduleEvent(EVENT_LIGHTNING, 10000);
                    events.ScheduleEvent(EVENT_DISCHARGE, 25000);
                    events.ScheduleEvent(EVENT_UNSTABLE_SHIELD, 11500);
                    break;

                case NPC_ARCANOTRON:
                    events.ScheduleEvent(EVENT_POWER_CONVERSION, 10000);
                    events.ScheduleEvent(EVENT_POWER_GENERATOR, 30000);
                    events.ScheduleEvent(EVENT_ARCANE_ANNIHILATOR, 8000);
                    break;
                }

                return;
                break;

            case ACTION_EVENT_FAILED:
                if(!isFirstTron)
                { // is not First Tron
                    DoAction(ACTION_DEACTIVATE);
                }else
                { // is First Tron
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->RemoveAllAuras();
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    me->GetMotionMaster()->MovePoint(1, wayPos[0]);
                    activated = true;
                    isMovingHome = false;
                }
                break;

            case ACTION_DEACTIVATE:
                me->SetReactState(REACT_PASSIVE);
                me->AttackStop();
                me->RemoveAllAuras();
                me->GetMotionMaster()->MovePoint(0, homePosition);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                activated = false;
                isMovingHome = true;
                break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || !activated)
                return;

            if(!me->HasAura(SPELL_ACTIVATED) && !isMovingHome)
            {
                DoAction(ACTION_DEACTIVATE);
                return;
            }

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
                    // Magmatron
                case EVENT_ACQUIRING_TARGET:
                    if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                        DoCast(pTarget, SPELL_ACQUIRING_TARGET);
                    me->MonsterYell(SAY_REROUTING_ENERGY, 0, 0);
                    events.ScheduleEvent(EVENT_ACQUIRING_TARGET, 15000);
                    return;
                case EVENT_INCINERATION_SECURITY_MEASURE:
                    DoCastAOE(SPELL_INCINERATION_SECURITY_MEASURE);
                    events.ScheduleEvent(EVENT_INCINERATION_SECURITY_MEASURE, 27000);
                    return;
                case EVENT_BARRIER:
                    DoCast(me, SPELL_BARRIER);
                    me->MonsterYell(SAY_SHIELD_FLAMME, 0, 0);
                    events.ScheduleEvent(EVENT_BARRIER, 60000);
                    return;

                    // Toxitron
                case EVENT_CHEMICAL_BOMB:
                    if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                        DoCast(pTarget, SPELL_CHEMICAL_BOMB);
                    events.ScheduleEvent(EVENT_CHEMICAL_BOMB, 30000);
                    return;

                case EVENT_POISON_PROTOCOL:
                    for (uint32 i10 = 0; i10 < 8; ++i10)
                        DoCast(me, SPELL_POISON_PROTOCOL+i10);
                    if (Is25ManRaid())
                        for (uint32 i25 = 0; i25 < 15; ++i25)
                            DoCast(me, SPELL_POISON_PROTOCOL+i25);
                    events.ScheduleEvent(EVENT_POISON_PROTOCOL, 41500);
                    return;

                case EVENT_POISON_SOAKED_SHELL:
                    DoCast(me, SPELL_POISON_SOAKED_SHELL);
                    me->MonsterYell(SAY_SHIELD_POISON, 0, 0);
                    events.ScheduleEvent(EVENT_POISON_SOAKED_SHELL, 65000);
                    return;

                    // Electron
                case EVENT_LIGHTNING:
                    if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                        DoCast(pTarget, SPELL_LIGHTNING_CONDUCTOR);
                    me->MonsterYell(SAY_LIGHTNING, 0, 0);
                    events.ScheduleEvent(EVENT_LIGHTNING,10000);
                    return;
                case EVENT_DISCHARGE:
                    if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                        DoCast(pTarget, SPELL_ELECTRICAL_DISCHARGE);
                    events.ScheduleEvent(EVENT_DISCHARGE, 25000);
                    return;
                case EVENT_UNSTABLE_SHIELD:
                    DoCast(me, SPELL_UNSTABLE_SHIELD);
                    me->MonsterYell(SAY_SHIELD_ELECTRON, 0, 0);
                    events.ScheduleEvent(EVENT_UNSTABLE_SHIELD, 60000);
                    return;

                    // Arcanotron
                case EVENT_POWER_CONVERSION:
                    DoCast(me, SPELL_POWER_CONVERSION);
                    me->MonsterYell(SAY_SHIELD_ARCANOTRON, 0, 0);
                    events.ScheduleEvent(EVENT_POWER_CONVERSION, 60000);
                    return;
                case EVENT_POWER_GENERATOR:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM,1,100,true))
                        DoCast(target, SPELL_POWER_GENERATOR);
                    events.ScheduleEvent(EVENT_POWER_GENERATOR, 30000);
                    return;
                case EVENT_ARCANE_ANNIHILATOR:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, true))
                        DoCast(target, SPELL_ARCANE_ANNIHILATOR);
                    events.ScheduleEvent(EVENT_ARCANE_ANNIHILATOR, 8000);
                    return;
                }
            }

            DoMeleeAttackIfReady();
        }

        void SetData(uint32 Type, uint32 Data)
        {
            if(Type != DATA_IS_FIRST_TRON)
                return;

            isFirstTron = (Data == 0) ? false : true;
        }

        void DamageTaken(Unit* /*who*/, uint32& damage)
        {
            if(omnotron)
                if(omnotron->GetHealth() - damage > 1)
                    omnotron->SetHealth(omnotron->GetHealth()-damage);
                else
                {
                    damage = 0;
                    omnotron->AI()->DoAction(ACTION_OMNNOTRON_EVENT_FINISHED);
                }
        }
    };
};

class npc_poison_bomb : public CreatureScript
{
public:
    npc_poison_bomb() : CreatureScript("npc_poison_bomb") { }

    struct npc_poison_bombAI : public ScriptedAI
    {
        npc_poison_bombAI(Creature * pCreature) : ScriptedAI(pCreature) {}

        uint32 uiFixe;
        uint32 uiBombeFixe;

        void Reset()
        {
            uiFixe = 2000;
            uiBombeFixe = 20000;
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            if(uiFixe <= uiDiff)
            {
                if (Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 1, 100, true))
                    DoCast(target, SPELL_FIXER);
                uiFixe = 2000;
            } else uiFixe -= uiDiff;

            if(uiBombeFixe <= uiDiff)
            {
                DoCastAOE(SPELL_BOMBE);
                uiBombeFixe = 20000;
            } else uiBombeFixe -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_poison_bombAI(creature);
    }
};

void AddSC_boss_omnotron_defense_system()
{
    new boss_omnotron();
    new boss_trons();
    new npc_poison_bomb();
}
/*
* Copyright (C) 2011 True Blood <http://www.trueblood-servers.com/>
* By Asardial
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

enum Spells
{
    // Omnitron
    SPELL_SHUTING_DOWN = 78746,
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
    EVENT_SHUTING_DOWN_ELECTRON,
    EVENT_SHUTING_DOWN_MAGMATRON,
    EVENT_SHUTING_DOWN_TOXITRON,
    EVENT_SHUTING_DOWN_ARCANOTRON,
    EVENT_LIGHTNING,
    EVENT_DISCHARGE,
    EVENT_UNSTABLE_SHIELD,
    EVENT_CHEMICAL_BOMB,
    EVENT_POISON_SOAKED_SHELL,
    EVENT_POISON_PROTOCOL,
    EVENT_GRIP_OF_DEATH,
    EVENT_BARRIER,
    EVENT_ACQUIRING_TARGET,
    EVENT_FLAMETHROWER,
    EVENT_INCINERATION_SECURITY_MEASURE,
    EVENT_POWER_GENERATOR,
    EVENT_ARCANE_ANNIHILATOR,
    EVENT_POWER_CONVERSION,
    EVENT_OVERCHARGED_POWER_GENERATOR,
    EVENT_GRIP_ZONE,
    EVENT_ENCASING_SHADOWS,
    EVENT_OVERCHARGED,
};

enum Actions
{
    ACTION_OMNOTRON_START_EVENT,
    ACTION_OMNOTRON_RESET,
};

enum DirectAccess
{
    MAGMATRON,
    TOXITRON,
    ELECTRON,
    ARCANOTRON,
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
        boss_omnotronAI(Creature* creature) : ScriptedAI(creature), eventActive(false)
        {
            instance = creature->GetInstanceScript();

            //for(uint8 i = 0; i<=3; i++)
            //    trons[i] = NULL;
        }

        InstanceScript* instance;
        EventMap events;

        Unit* trons[4];
        bool eventActive;


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
            if(!eventActive)
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

        void DoAction(uint32 action)
        {
            switch(action)
            {
            case ACTION_OMNOTRON_START_EVENT:
                // Start Encounter

                eventActive = true;
                me->MonsterSay("Activated",0,0);
                

                break;

            case ACTION_OMNOTRON_RESET:
                // Resets Encounter

                if(eventActive)
                {
                    eventActive = false;

                    me->MonsterSay("Reset",0,0);

                    trons[MAGMATRON] = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_MAGMATRON));
                    trons[TOXITRON] = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_TOXITRON));
                    trons[ELECTRON] = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_ELECTRON));
                    trons[ARCANOTRON] = ObjectAccessor::GetCreature(*me,instance->GetData64(NPC_ARCANOTRON));
                }
                break;
            }
        }
    };
};

struct mob_tron_dummyAI : public ScriptedAI
{
    mob_tron_dummyAI(Creature* creature) : ScriptedAI(creature)
    {
        instance = creature->GetInstanceScript();
    }

    InstanceScript* instance;

    void _Reset()
    {
        ResetTron();

        if (Creature* omnotron = ObjectAccessor::GetCreature(*me,instance->GetData64(BOSS_OMNOTRON)))
            omnotron->GetAI()->DoAction(ACTION_OMNOTRON_START_EVENT);

    }

    void ResetTron()
    {
        me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
        me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);

        if(me->GetEntry() != NPC_TOXITRON) // Toxitron is using waypoints
        {
            me->AddUnitState(UNIT_STAT_STUNNED);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE || UNIT_FLAG_NON_ATTACKABLE);
        }
    }

    void ActivateTron()
    {
        me->ClearUnitState(UNIT_STAT_STUNNED);
        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE || UNIT_FLAG_NON_ATTACKABLE);
    }

    void DeactivateTron()
    {
        me->GetMotionMaster()->MoveTargetedHome();

        me->AddUnitState(UNIT_STAT_STUNNED);
        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE || UNIT_FLAG_NON_ATTACKABLE);
    }

    void DamageTaken(Unit* attacker, uint32 &damage)
    {
        switch(me->GetEntry())
        {
        case NPC_ARCANOTRON:
            break;
        case NPC_ELECTRON:
            break;
        case NPC_TOXITRON:
            break;
        case NPC_MAGMATRON:
            break;
        }
    }
};

/************
** Electron
************/
class boss_electron: public CreatureScript
{
public :
    boss_electron() : CreatureScript("boss_electron") { }

    struct boss_electronAI : public mob_tron_dummyAI 
    {
        boss_electronAI(Creature * creature) : mob_tron_dummyAI(creature)
        {
            pInstance = creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        EventMap events;

        void Reset()
        {
            _Reset();

            events.Reset();
        }

        void EnterCombat(Unit * /*who*/)
        {
            me->MonsterYell(SAY_AGGRO, 0, 0);
            DoCast(me, SPELL_ACTIVATED);

            events.ScheduleEvent(EVENT_LIGHTNING, 10000);
            events.ScheduleEvent(EVENT_DISCHARGE, 25000);
            events.ScheduleEvent(EVENT_UNSTABLE_SHIELD, 11500);
            events.ScheduleEvent(EVENT_SHUTING_DOWN_ELECTRON, 65000);
        }

        void JustDied(Unit* /*Killer*/)
        {
            me->MonsterYell(SAY_DEATH, 0, 0);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
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
                case EVENT_SHUTING_DOWN_ELECTRON:
                    DoCast(me, SPELL_SHUTING_DOWN);
                    me->AttackStop();
                    me->RemoveAllAuras();
                    events.ScheduleEvent(EVENT_SHUTING_DOWN_ELECTRON, 65000);
                    return;
                }

            }
            DoMeleeAttackIfReady();

        }
    };
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_electronAI (pCreature);
    }

};

/************
** Magmatron
************/
class boss_magmatron : public CreatureScript
{
public :
    boss_magmatron() : CreatureScript("boss_magmatron") { }

    struct boss_magmatronAI : public mob_tron_dummyAI 
    {
        boss_magmatronAI(Creature * pCreature) : mob_tron_dummyAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        EventMap events;

        void Reset()
        {
            _Reset();

            events.Reset();
        }

        void EnterCombat(Unit * /*who*/)
        {
            DoCast(me, SPELL_ACTIVATED);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_AGGRESSIVE);
            //NPC_ELECTRON->SetHealth(me->GetHealth());
            EnterPhaseGround();
        }

        void EnterPhaseGround()
        {
            events.ScheduleEvent(EVENT_ACQUIRING_TARGET, 15000);
            events.ScheduleEvent(EVENT_INCINERATION_SECURITY_MEASURE, 27000);
            events.ScheduleEvent(EVENT_BARRIER, 30000);
            events.ScheduleEvent(EVENT_SHUTING_DOWN_MAGMATRON, 65000);
        }

        void JustDied(Unit* /*Killer*/)
        {
            me->MonsterYell(SAY_DEATH, 0, 0);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
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
                case EVENT_SHUTING_DOWN_MAGMATRON:
                    DoCast(me, SPELL_SHUTING_DOWN);
                    me->AttackStop();
                    me->RemoveAllAuras();
                    events.ScheduleEvent(EVENT_SHUTING_DOWN_MAGMATRON, 65000);
                    return;
                }

            }
            DoMeleeAttackIfReady();

        }
    };
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_magmatronAI (pCreature);
    }

};

/************
** Toxitron
************/
class boss_toxitron : public CreatureScript
{
public :
    boss_toxitron() : CreatureScript("boss_toxitron") { }

    struct boss_toxitronAI : public mob_tron_dummyAI 
    {
        boss_toxitronAI(Creature * pCreature) : mob_tron_dummyAI(pCreature)
        {
            instance = pCreature->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;

        void Reset()
        {
            _Reset();

            events.Reset();
        }

        void EnterCombat(Unit * /*who*/)
        {
            events.ScheduleEvent(EVENT_CHEMICAL_BOMB, 30000);
            events.ScheduleEvent(EVENT_POISON_PROTOCOL, 41500);
            events.ScheduleEvent(EVENT_POISON_SOAKED_SHELL, 65000);
            events.ScheduleEvent(EVENT_SHUTING_DOWN_TOXITRON, 65000);

            if (Creature* omnotron = ObjectAccessor::GetCreature(*me,instance->GetData64(BOSS_OMNOTRON)))
                omnotron->GetAI()->DoAction(ACTION_OMNOTRON_START_EVENT);
        }

        void JustDied(Unit* /*Killer*/)
        {
            me->MonsterYell(SAY_DEATH, 0, 0);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
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
                case EVENT_SHUTING_DOWN_TOXITRON:
                    DoCast(me, SPELL_SHUTING_DOWN);
                    me->AttackStop();
                    me->RemoveAllAuras();
                    events.ScheduleEvent(EVENT_SHUTING_DOWN_TOXITRON, 65000);
                    return;
                }

            }
            DoMeleeAttackIfReady();

        }
    };
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_toxitronAI (pCreature);
    }

};

/*************
** Arcanotron
*************/
class boss_arcanotron : public CreatureScript
{
public :
    boss_arcanotron() : CreatureScript("boss_arcanotron") { }

    struct boss_arcanotronAI : public mob_tron_dummyAI 
    {
        boss_arcanotronAI(Creature * pCreature) : mob_tron_dummyAI(pCreature)
        {
            pInstance = pCreature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        EventMap events;

        void Reset()
        {
            _Reset();

            events.Reset();
        }

        void EnterCombat(Unit * /*who*/)
        {
            DoCast(me, SPELL_ACTIVATED);
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetReactState(REACT_AGGRESSIVE);
            //NPC_ELECTRON->SetHealth(me->GetHealth());
            EnterPhaseGround();
        }

        void EnterPhaseGround()
        {
            events.ScheduleEvent(EVENT_POWER_CONVERSION, 10000);
            events.ScheduleEvent(EVENT_POWER_GENERATOR, 30000);
            events.ScheduleEvent(EVENT_ARCANE_ANNIHILATOR, 8000);
            events.ScheduleEvent(EVENT_SHUTING_DOWN_ARCANOTRON, 65000);
        }

        void JustDied(Unit* /*Killer*/)
        {
            me->MonsterYell(SAY_DEATH, 0, 0);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch(eventId)
                {
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
                case EVENT_SHUTING_DOWN_ARCANOTRON:
                    DoCast(me, SPELL_SHUTING_DOWN);
                    me->AttackStop();
                    me->RemoveAllAuras();
                    events.ScheduleEvent(EVENT_SHUTING_DOWN_ARCANOTRON, 65000);
                    return;
                }

            }
            DoMeleeAttackIfReady();

        }
    };
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_arcanotronAI (pCreature);
    }

};

/**************
** Poison Bomb
**************/
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
    new boss_magmatron();
    new boss_toxitron();
    new boss_electron();
    new boss_arcanotron();
    new npc_poison_bomb();
}
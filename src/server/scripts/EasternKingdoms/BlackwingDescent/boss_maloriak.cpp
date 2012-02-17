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

enum Spells
{
    // General
    SPELL_BERSERK                   = 64238,
    SPELL_RELEASE_ABBERATIONS       = 77569,
    SPELL_RELEASE_ALL_MINIONS       = 77991,
    SPELL_REMEDY                    = 92967,
    SPELL_ARCANE_STORM              = 77896,

    // Red Phase
    SPELL_DRINK_RED_BOTTLE          = 88699,

    SPELL_SCORCHING_BLAST           = 92970,
    SPELL_CONSUMING_FLAMES          = 92973,

    // Blue Phase
    SPELL_DRINK_BLUE_BOTTLE         = 88700,

    SPELL_BITING_CHILL              = 77760,

    SPELL_FLASH_FREEZE              = 92980,
    SPELL_FLASH_FREEZE_SUMMON       = 77711,

    // Green Phase
    SPELL_THROW_GREEN_BOTTLE        = 77937,
    SPELL_DEBILITATING_SLIME        = 92910,
    SPELL_DEBILITATING_SLIME_VISUAL = 77602, //(Dummy Effect)

    // Black Phase
    SPELL_DRINK_BLACK_BOTTLE        = 92828,

    // Final Phase
    SPELL_ACID_NOVA                 = 93013,
    SPELL_MAGMA_JET                 = 78194,
    SPELL_MAGMA_JET_AURA            = 78095,

};

enum Events
{
    // General
    EVENT_NEW_PHASE = 1,
    EVENT_DRINK_BOTTLE,
    EVENT_UNLOCK_SPELLS,
    EVENT_WAIT_SWITCH_PHASE,
    EVENT_BERSERK,
    EVENT_REMEDY,
    EVENT_ARCANE_STORM,

    // Red Phase
    EVENT_SCORCHING_BLAST,
    EVENT_CONSUMING_FLAMES,

    // Blue Phase
    EVENT_BITING_CHILL,
    EVENT_FLASH_FREEZE,
};

Position const CauldronPositions[1] =
{
    {-111.704559f, -477.060272f, 73.456284f, 6.216876f},
};

enum Phases
{
    PHASE_RED,
    PHASE_BLUE,
    PHASE_GREEN,
    PHASE_BLACK,
    PHASE_NON,
    PHASE_FINAL,
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
        uint8 phase;
        uint8 abberationsLeft;
        uint8 lastPhase;
        bool spellsLocked;

        bool wasInBlackPhase;
        uint8 withoutGreenPhase;

        void Reset()
        {
            events.Reset();
            me->SetReactState(REACT_AGGRESSIVE);
            abberationsLeft = 15;
            withoutGreenPhase = 0;
            wasInBlackPhase = true;
            spellsLocked = false;
            UpdatePhase(PHASE_NON);
            DespawnMinions();

            _Reset();
        }

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();

            events.ScheduleEvent(EVENT_NEW_PHASE, urand(10000,12000));
            events.ScheduleEvent(EVENT_REMEDY, urand(15000,18000));
            events.ScheduleEvent(EVENT_ARCANE_STORM, urand(7000,8000));

            events.ScheduleEvent(EVENT_BERSERK, me->GetMap()->IsHeroic() ? 720000 : 420000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || me->HasUnitState(UNIT_STAT_CASTING))
                return;

            events.Update(diff);
            _DoAggroPulse(diff);

            if(me->GetHealthPct() < 25 && phase != PHASE_FINAL)
            {   // Enter Final Phase

                uint32 uiBerserker = events.GetNextEventTime(EVENT_BERSERK);
                events.Reset();
                events.ScheduleEvent(EVENT_BERSERK, uiBerserker);

                phase = PHASE_FINAL;

                DoCast(SPELL_RELEASE_ABBERATIONS);
            };

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    // General and Phase Switching
                case EVENT_NEW_PHASE:
                    UpdatePhase(urand(PHASE_RED, PHASE_BLUE));
                    spellsLocked = true;
                    break;

                case EVENT_DRINK_BOTTLE:

                    switch(phase)
                    {
                    case PHASE_RED:
                        DoCastAOE(SPELL_DRINK_RED_BOTTLE);
                        break;

                    case PHASE_BLUE:
                        DoCastAOE(SPELL_DRINK_BLUE_BOTTLE);
                        break;

                    case PHASE_GREEN:
                        DoCastAOE(SPELL_THROW_GREEN_BOTTLE);
                        break;

                    case PHASE_BLACK:
                        DoCastAOE(SPELL_DRINK_BLACK_BOTTLE);
                        break;
                    }
                    events.ScheduleEvent(EVENT_WAIT_SWITCH_PHASE, 1000);
                    break;

                case EVENT_WAIT_SWITCH_PHASE:
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->GetMotionMaster()->MoveChase(me->getVictim());

                    // Intialize Phase Events
                    switch(phase)
                    {
                    case PHASE_RED:
                        events.ScheduleEvent(EVENT_SCORCHING_BLAST, 7000);
                        events.ScheduleEvent(EVENT_CONSUMING_FLAMES, 3000);
                        break;

                    case PHASE_BLUE:
                        events.ScheduleEvent(EVENT_BITING_CHILL, 7000);
                        events.ScheduleEvent(EVENT_FLASH_FREEZE, 9000);
                        break;

                    case PHASE_GREEN:
                        break;

                    case PHASE_BLACK:
                        break;
                    }

                    events.ScheduleEvent(EVENT_UNLOCK_SPELLS, 1500);
                    events.ScheduleEvent(EVENT_NEW_PHASE, 25000);
                    break;

                case EVENT_UNLOCK_SPELLS:
                    spellsLocked = false;
                    break;

                case EVENT_BERSERK:
                    DoCast(me,SPELL_BERSERK);
                    break;

                case EVENT_REMEDY:
                    if(spellsLocked)
                        events.ScheduleEvent(EVENT_REMEDY, 1500);
                    else
                    {
                        DoCast(me,SPELL_REMEDY);
                        events.ScheduleEvent(EVENT_REMEDY, urand(15000,18000));
                    }
                    break;

                case EVENT_ARCANE_STORM:
                    if(spellsLocked)
                        events.ScheduleEvent(EVENT_ARCANE_STORM, 1500);
                    else
                    {
                        me->AttackStop();
                        DoCastAOE(SPELL_ARCANE_STORM);
                        events.ScheduleEvent(EVENT_ARCANE_STORM, urand(27000,29000));
                    }
                    break;

                    // Red Phase
                case EVENT_SCORCHING_BLAST:
                    DoCastAOE(SPELL_SCORCHING_BLAST);
                    events.ScheduleEvent(EVENT_SCORCHING_BLAST, urand(15000, 17000));
                    break;

                case EVENT_CONSUMING_FLAMES:
                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                        DoCast(target, SPELL_CONSUMING_FLAMES);

                    events.ScheduleEvent(EVENT_CONSUMING_FLAMES, urand(7000, 8500));
                    break;

                    // Blue Phase
                case EVENT_BITING_CHILL:
                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                        DoCast(target, SPELL_BITING_CHILL);

                    events.ScheduleEvent(EVENT_BITING_CHILL, urand(11000, 13000));
                    break;

                case EVENT_FLASH_FREEZE:
                    if(Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
                        target->CastSpell(target, SPELL_FLASH_FREEZE_SUMMON, true);

                    events.ScheduleEvent(EVENT_FLASH_FREEZE, urand(14000, 16000));
                    break;

                default:
                    break;
                }
            }		

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/)
        {
            DespawnMinions();

            _JustDied();
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE || id != 1)
                return;

            if(GameObject* cauldron = me->FindNearestGameObject(GOB_MALORIAKS_CAULDRON,100.f))
            {
                cauldron->SendCustomAnim(phase);
                me->SetFacingToObject(cauldron);
            }

            events.ScheduleEvent(EVENT_DRINK_BOTTLE, 1500);
        }

    private:
        void UpdatePhase(uint8 newPhase)
        {
            // Cancel current Phase Events

            switch(phase)
            {
            case PHASE_RED:
                events.CancelEvent(EVENT_SCORCHING_BLAST);
                events.CancelEvent(EVENT_CONSUMING_FLAMES);
                break;

            case PHASE_BLUE:
                events.CancelEvent(EVENT_BITING_CHILL);
                events.CancelEvent(EVENT_FLASH_FREEZE);
                break;

            case PHASE_GREEN:
                break;

            case PHASE_BLACK:
                break;
            }

            phase = newPhase;

            if(phase == PHASE_NON)
                return;

            // In Heroic Mode every 2. Phase is a Black Phase
            if((me->GetMap()->IsHeroic()) && (!wasInBlackPhase))
            {
                phase = PHASE_BLACK;
                wasInBlackPhase = true;
            } else
            {
                withoutGreenPhase++;
                wasInBlackPhase = false;

                if(lastPhase == phase )
                {
                    if(phase == PHASE_RED)
                        phase = PHASE_BLUE;
                    else if(phase == PHASE_BLUE)
                        phase = PHASE_RED;
                }

                lastPhase = phase;
            }

            // Every 3. Phase is a Green Phase
            if(withoutGreenPhase >= 3)
            {
                phase = PHASE_GREEN;
                withoutGreenPhase = 0;
            }              

            // Debug
            phase = PHASE_BLUE;

            me->SetReactState(REACT_PASSIVE);
            me->AttackStop();
            me->GetMotionMaster()->MovePoint(1, CauldronPositions[0]);
        }

        inline void DespawnMinions()
        {
            DespawnCreatures(NPC_ABBERATON);
            DespawnCreatures(NPC_PRIME_SUBJECT);
            DespawnCreatures(NPC_FLASH_FREEZE);
            DespawnCreatures(NPC_MAGMA_JET_CONTROLLER);
            DespawnCreatures(NPC_ABSOLUTE_ZERO);
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

class mob_flash_freeze_maloriak : public CreatureScript
{
public:
    mob_flash_freeze_maloriak() : CreatureScript("mob_flash_freeze_maloriak") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_flash_freeze_maloriakAI (creature);
    }

    struct mob_flash_freeze_maloriakAI : public ScriptedAI
    {
        mob_flash_freeze_maloriakAI(Creature* creature) : ScriptedAI(creature) { }

        Unit* target;

        void IsSummonedBy(Unit* summoner)
        {
            target = summoner;
            me->AddAura(SPELL_FLASH_FREEZE, me);
            target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_STUNNED | UNIT_FLAG_PACIFIED);
        }

        void JustDied(Unit* /*killer*/)
        {
            target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_STUNNED | UNIT_FLAG_PACIFIED);
        }
    };
};

class spell_arcane_storm_maloriak : public SpellScriptLoader
{
public:
    spell_arcane_storm_maloriak() : SpellScriptLoader("spell_arcane_storm_maloriak") { }

    class spell_arcane_storm_maloriak_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_arcane_storm_maloriak_SpellScript);

        bool Validate(SpellEntry const* /*spell*/)
        {
            return true;
        }

        void HandleDamage(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();
            
            if (caster && target)
            {
                caster->CastSpell(target, 77908, true);
            }
        }

        void Register()
        {
            OnEffect += SpellEffectFn(spell_arcane_storm_maloriak_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_arcane_storm_maloriak_SpellScript();
    }
};

class spell_release_abberations : public SpellScriptLoader
{
public:
    spell_release_abberations() : SpellScriptLoader("spell_release_abberations") { }

    class spell_release_abberationsSpellScript : public SpellScript
    {
        PrepareSpellScript(spell_release_abberationsSpellScript);

        bool Validate(SpellEntry const * spellEntry)
        {
            return true;
        }

        bool Load()
        {
            return true;
        }

        void HandleDummy(SpellEffIndex effIndex)
        {
            GetCaster()->MonsterSay("test",0,0);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_release_abberationsSpellScript::HandleDummy,EFFECT_0,SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript *GetSpellScript() const
    {
        return new spell_release_abberationsSpellScript();
    }
};

class spell_release_all_abberations : public SpellScriptLoader
{
public:
    spell_release_all_abberations() : SpellScriptLoader("spell_release_all_abberations") { }

    class spell_release_all_abberationsSpellScript : public SpellScript
    {
        PrepareSpellScript(spell_release_all_abberationsSpellScript);

        bool Validate(SpellEntry const * spellEntry)
        {
            return true;
        }

        bool Load()
        {
            return true;
        }

        void HandleDummy(SpellEffIndex effIndex)
        {
            GetCaster()->MonsterSay("test",0,0);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_release_all_abberationsSpellScript::HandleDummy,EFFECT_0,SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript *GetSpellScript() const
    {
        return new spell_release_all_abberationsSpellScript();
    }
};

void AddSC_boss_maloriak()
{
    new boss_maloriak();
    new mob_flash_freeze_maloriak();
    //new spell_arcane_storm_maloriak();
    new spell_release_abberations();
    new spell_release_all_abberations();
}
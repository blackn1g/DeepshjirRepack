/********************
* (C) DeepshjirCataclysm Repack
*  by Naios (2012)
********************/
-- Maloriak (Blackwing Descent) Updates

UPDATE `creature_template` SET `AIName`='', `ScriptName`='boss_maloriak' WHERE `entry`=41378 LIMIT 1;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 41378;

UPDATE `creature_model_info` SET `combat_reach`=6 WHERE `modelid`=33186 LIMIT 1;

DELETE FROM `creature` WHERE `id` = 41440;

DELETE FROM `creature_template` WHERE `entry` = 41377;
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (41377, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Release Abberations Trigger', '', '', 0, 83, 83, 0, 16, 16, 0, 1, 1.14286, 1, 0, 3.8, 3.8, 0, 46, 1, 0, 0, 1, 33554948, 0, 0, 0, 0, 0, 0, 1.9, 1.9, 5000, 10, 1024, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 1);

-- Prime Subjects
UPDATE `creature_template` SET `faction_A`=16, `faction_H`=16, `rank`=2 WHERE `entry` = 41841;

-- Flash Freeze
UPDATE `creature_template` SET `ScriptName`='mob_flash_freeze_maloriak', `unit_flags`=518, `type_flags`=0, `minlevel`=88, `maxlevel`=88, `faction_A`=16, `faction_H`=16 , `modelid1`=11686, `modelid2`=0, `modelid3`=0, `modelid4`=0 WHERE `entry`=41576;

REPLACE INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`)
VALUES (41576, 0, 0, 0, 0, 0, '92980 0');


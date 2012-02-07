/********************
* (C) DeepshjirCataclysm Repack
*  by Naios (2012)
********************/
-- Conalcave of Wind (Throne of the four Winds)

UPDATE `creature_template` SET `ScriptName`='boss_anshal' , AIName="" WHERE `entry`=45870 LIMIT 1;
UPDATE `creature_template` SET `ScriptName`='boss_nezir' , AIName="" WHERE `entry`=45871 LIMIT 1;
UPDATE `creature_template` SET `ScriptName`='boss_rohash' , AIName="" WHERE `entry`=45872 LIMIT 1;

REPLACE INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(45870, 0, 0, 0, 0, 0, '96301 0'),
(45871, 0, 0, 0, 0, 0, '96301 0'),
(45872, 0, 0, 0, 0, 0, '96301 0');

DELETE FROM `smart_scripts` WHERE `entryorguid` IN(45870, 45871, 45872);

REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`)
VALUES (85428, 'spell_nurture_aura');

-- Anshal
-- Soothing Breeze
DELETE FROM `creature_template_addon` WHERE `entry` = 46246;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`)
VALUES (46246, 0, 0, 0, 0, 0, '86208 0 27986 0');

UPDATE `creature_template` SET `unit_flags`=33554948, `type_flags`=1024, `minlevel`=83, `maxlevel`=83, `faction_A`=16, `faction_H`=16, `rangedattackpower`=5000 WHERE `entry`=46246 LIMIT 1;

-- Ravenous Creeper
UPDATE `creature_template` SET `difficulty_entry_1`=50100, `difficulty_entry_2`=50110, `difficulty_entry_3`=50120 WHERE `entry`=45812 LIMIT 1;
UPDATE `creature_template` SET `exp`=3, `minlevel`=88, `maxlevel`=88,`faction_A`=16, `faction_H`=16, `rank`=2 WHERE `entry`IN(50100, 50110, 50120, 45812) LIMIT 4;

SET @ENTRY := 45812;
SET @SOURCETYPE := 0;

DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY;
UPDATE creature_template SET AIName="SmartAI" WHERE entry=@ENTRY LIMIT 1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES 
(@ENTRY,@SOURCETYPE,0,0,33,0,100,0,0,1,1,1,11,86281,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,"Toxic Spores");

-- Ravenous Creeper Trigger
DELETE FROM `creature_template_addon` WHERE `entry` = 45813;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`)
VALUES (45813, 0, 0, 0, 0, 0, '85428 0');

UPDATE `creature_template` SET `unit_flags`=33554948, `type_flags`=1024, `minlevel`=88, `maxlevel`=88, `faction_A`=35, `faction_H`=35 , `modelid1`=11686, `modelid2`=0, `modelid3`=0, `modelid4`=0 WHERE `entry`= 45813 LIMIT 1;

-- Spell 86281

-- Nezir
-- Ice Patch
DELETE FROM `creature_template_addon` WHERE `entry` = 46186;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`)
VALUES (46186, 0, 0, 0, 0, 0, '86107 0');

UPDATE `creature_template` SET `unit_flags`=33554948, `type_flags`=1024, `minlevel`=83, `maxlevel`=83, `faction_A`=16, `faction_H`=16, `rangedattackpower`=5000 WHERE `entry`=46186 LIMIT 1;

/********************
* (C) DeepshjirCataclysm Repack
*  by Naios (2012)
********************/
-- Gilneas Updates

-- Lockdown! fix
UPDATE `creature` SET `phaseMask`=3 WHERE `guid` IN(15461696, 15461697, 15461689);

-- Removes two mobs that are near Prince Liam Greymane
DELETE FROM `creature` WHERE `guid` IN(15461731);

-- Classquests
-- Killcredit
REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (35121, 0, 0, 0, 0, 0, 36772, 36771, 36770, 0, 'Bloodfang Worgen Classquest Killcredit', '', '', 0, 2, 3, 0, 16, 16, 3, 1, 1.14286, 1, 0, 13, 17, 0, 42, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 9, 13, 0, 7, 0, 35118, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, '', 1);

UPDATE `quest_template` SET `ReqCreatureOrGOId1`=35121, `ReqCreatureOrGOCount1`=1 WHERE `entry` IN(14266, 14274, 14276, 14272, 14281);

-- Bloodfang Worgen
SET @ENTRY := 35118;
SET @SOURCETYPE := 0;

DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY;
UPDATE creature_template SET AIName="SmartAI" WHERE entry=@ENTRY LIMIT 1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES 
(@ENTRY,@SOURCETYPE,0,0,2,0,100,0,5,10,2000,3000,11,8599,1,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Bloodfang Worgen"),

-- Warrior: 14266 Charge
(@ENTRY,@SOURCETYPE,1,0,8,0,100,0,100,0,0,0,33,35121,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,"Classquest killcredit"),
-- Warlock: 14274 Immolate
(@ENTRY,@SOURCETYPE,2,0,8,0,100,0,348,0,0,0,33,35121,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,"Classquest killcredit"),
-- Hunter: 14276 Steady Shot
(@ENTRY,@SOURCETYPE,3,0,8,0,100,0,56641,0,0,0,33,35121,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,"Classquest killcredit"),
-- Rogue: 14272 Eviscerate
(@ENTRY,@SOURCETYPE,4,0,8,0,100,0,2098,0,0,0,33,35121,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,"Classquest killcredit"),
-- Mage: 14281 Arcane Missiles
(@ENTRY,@SOURCETYPE,5,0,8,0,100,0,5143,0,0,0,33,35121,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,"Classquest killcredit");

-- Druid: 14283 & Priest: 14279
UPDATE `quest_template` SET `ReqCreatureOrGOId1`=47091, `ReqCreatureOrGOCount1`=2 WHERE `entry` IN(14283, 14279);

-- Wounded Guard SAI
SET @ENTRY := 47091;
SET @SOURCETYPE := 0;

DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY;
UPDATE creature_template SET AIName="SmartAI" WHERE entry=@ENTRY LIMIT 1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES 
(@ENTRY,@SOURCETYPE,0,0,8,0,100,0,5185,0,60000,60000,33,47091,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,"Healing Touch add Questkill"),
(@ENTRY,@SOURCETYPE,1,0,8,0,100,0,2061,0,60000,60000,33,47091,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,"Flashheal add Questkill");

/********************
* (C) DeepshjirCataclysm Repack
*  by Naios (2012)
********************/
-- Blackwing Descent Trash Updates

-- Pre Boss Halls

/*****
* Golem Sentry
*****/
SET @ENTRY := 42800;
SET @SOURCETYPE := 0;

DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY;
UPDATE creature_template SET AIName="SmartAI" WHERE entry=@ENTRY LIMIT 1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES 
(@ENTRY,@SOURCETYPE,0,0,0,0,100,30,15000,15000,23000,24000,11,81055,1,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,"Golem Sentry - Electrical Discharge"),
(@ENTRY,@SOURCETYPE,1,0,0,0,100,30,7000,7000,16000,18000,11,81056,1,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,"Golem Sentry - Flash Bomb"),
(@ENTRY,@SOURCETYPE,2,0,0,0,100,30,12000,14000,38000,42000,11,81063,1,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,"Golem Sentry - Laser Strike");

-- Flash Bomb
SET @ENTRY := 43361;
SET @SOURCETYPE := 0;

DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY;
UPDATE creature_template SET AIName="SmartAI" WHERE entry=@ENTRY LIMIT 1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES 
(@ENTRY,@SOURCETYPE,0,0,54,0,100,30,0,0,0,0,11,91885,2,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"Flash Bomb");

UPDATE `creature_template` SET `unit_flags`=33554436, `type_flags`=1024 WHERE `entry`=43361 LIMIT 1;

-- Laser Strike (43362)
DELETE FROM `smart_scripts` WHERE `entryorguid`=43362;
DELETE FROM `creature_template_addon` WHERE `entry` = 43362;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`)
VALUES (43362, 0, 0, 0, 0, 0, '81065 0 81066 0');

UPDATE `creature_template` SET `unit_flags`=33554436, `type_flags`=1024, `faction_H`=16, `faction_A`=16, AIName="SmartAI" WHERE `entry`=43362 LIMIT 1;

/*****
* Drakonid Chainwielder
*****/

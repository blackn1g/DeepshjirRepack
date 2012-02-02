-- Areatrigger Warsong Farms SAI
SET @AT_SLAUGHTERHOUSE := 4873;
SET @AT_GRAINERY := 4871;
SET @AT_TORP_FARM := 4872;
SET @NPC_SLAUGHTERHOUSE := 25672;
SET @NPC_GRAINERY := 25669;
SET @NPC_TORP_FARM := 25671;
UPDATE `areatrigger_scripts` SET `ScriptName`='SmartTrigger' WHERE `entry` IN (@AT_SLAUGHTERHOUSE,@AT_GRAINERY,@AT_TORP_FARM);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@AT_SLAUGHTERHOUSE,@AT_GRAINERY,@AT_TORP_FARM) AND `source_type`=2;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@AT_SLAUGHTERHOUSE,2,0,0,46,0,100,0,@AT_SLAUGHTERHOUSE,0,0,0,33,@NPC_SLAUGHTERHOUSE,0,0,0,0,0,7,0,0,0,0,0,0,0,"Areatrigger Slaughterhouse - On Trigger - Quest Credit"),
(@AT_GRAINERY,2,0,0,46,0,100,0,@AT_GRAINERY,0,0,0,33,@NPC_GRAINERY,0,0,0,0,0,7,0,0,0,0,0,0,0,"Areatrigger Grainery - On Trigger - Quest Credit"),
(@AT_TORP_FARM,2,0,0,46,0,100,0,@AT_TORP_FARM,0,0,0,33,@NPC_TORP_FARM,0,0,0,0,0,7,0,0,0,0,0,0,0,"Areatrigger Torp Farm - On Trigger - Quest Credit");

-- Areatrigger Ravenholdt SAI
SET @AT_RAVENHOLDT := 3066;
SET @NPC_RAVENHOLDT := 13936;
UPDATE `areatrigger_scripts` SET `ScriptName`='SmartTrigger' WHERE `entry`=@AT_RAVENHOLDT;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@AT_RAVENHOLDT AND `source_type`=2;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@AT_RAVENHOLDT,2,0,0,46,0,100,0,@AT_RAVENHOLDT,0,0,0,33,@NPC_RAVENHOLDT,0,0,0,0,0,7,0,0,0,0,0,0,0,"Areatrigger Ravenholdt - On Trigger - Quest Credit");

-- Areatrigger Aldurthar Gate SAI
SET @AT_SOUTH := 5284;
SET @AT_CENTRAL := 5285;
SET @AT_NORTH := 5286;
SET @AT_NORTHWEST := 5287;
SET @NPC_SOUTH := 32195;
SET @NPC_CENTRAL := 32196;
SET @NPC_NORTH := 32197;
SET @NPC_NORTHWEST := 32199;
UPDATE `areatrigger_scripts` SET `ScriptName`='SmartTrigger' WHERE `entry` IN (@AT_SOUTH,@AT_CENTRAL,@AT_NORTH,@AT_NORTHWEST);
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@AT_SOUTH,@AT_CENTRAL,@AT_NORTH,@AT_NORTHWEST) AND `source_type`=2;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@AT_SOUTH,2,0,0,46,0,100,0,@AT_SOUTH,0,0,0,33,@NPC_SOUTH,0,0,0,0,0,7,0,0,0,0,0,0,0,"Areatrigger South - On Trigger - Quest Credit"),
(@AT_CENTRAL,2,0,0,46,0,100,0,@AT_CENTRAL,0,0,0,33,@NPC_CENTRAL,0,0,0,0,0,7,0,0,0,0,0,0,0,"Areatrigger Central - On Trigger - Quest Credit"),
(@AT_NORTH,2,0,0,46,0,100,0,@AT_NORTH,0,0,0,33,@NPC_NORTH,0,0,0,0,0,7,0,0,0,0,0,0,0,"Areatrigger North - On Trigger - Quest Credit"),
(@AT_NORTHWEST,2,0,0,46,0,100,0,@AT_NORTHWEST,0,0,0,33,@NPC_NORTHWEST,0,0,0,0,0,7,0,0,0,0,0,0,0,"Areatrigger Northwest - On Trigger - Quest Credit");

DELETE FROM `script_texts` WHERE `entry` IN (-1574000,-1574001,-1574002,-1574003,-1574004);

DELETE FROM `creature_text` WHERE `entry`=23953;
INSERT INTO `creature_text` (`entry`, `groupid`, `text`, `sound`, `emote`, `type`) VALUES 
(23953,1,'Your blood is mine!',13221,7,14),
(23953,2,'Aranal, ledel! Their fate shall be yours!',13224,0,14),
(23953,3,'Not so fast.',13222,0,14),
(23953,4,'%s casts Frost Tomb on $n.',0,0,41),
(23953,5,'I join... the night.',13225,0,14);

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (1237,1240,1241);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(1237, 0, 0, 0, ''), -- Achievement: Let's Get This Done
(1240, 0, 0, 0, ''), -- Achievement: Flurry
(1241, 0, 0, 0, ''); -- Achievement: The Alterac Blitz

-- Condition update to flags
DELETE FROM `conditions` WHERE `ConditionTypeOrReference`=5;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`ElseGroup`,`ConditionTypeOrReference`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`ErrorTextId`,`ScriptName`,`Comment`) VALUES
(19,0,13846,0,5,1106,127,0,0,'','Quest Contributin'' To The Cause - Requires Argent Crusade hated thru revered'),
(20,0,13846,0,5,1106,127,0,0,'','Quest Contributin'' To The Cause - Requires Argent Crusade hated thru revered'),
(14,21258,7594,0,5,270,120,0,0,'','Only show text_id 7594 if player is neutral thru revered with Zandalar Tribe (270)'),
(14,21258,7595,0,5,270,128,0,0,'','Only show text_id 7594 if player is exalted with Zandalar Tribe (270)'),
(15,21258,0,0,5,270,128,0,0,'','Only show gossip option if player is exalted with Zandalar Tribe (270)'),
(15,21259,0,0,5,270,128,0,0,'','Only show gossip option if player is exalted with Zandalar Tribe (270)'),
(15,21260,0,0,5,270,128,0,0,'','Only show gossip option if player is exalted with Zandalar Tribe (270)'),
(1,23342,32726,0,5,1015,240,0,0,'','Murkblood Escape Plans - when Netherwing friendly thru exalted'),
(1,23286,32726,0,5,1015,240,0,0,'','Murkblood Escape Plans - when Netherwing friendly thru exalted');

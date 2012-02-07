DELETE FROM creature_template WHERE entry = '95002';

INSERT INTO creature_template (entry, modelid1, name, subname, IconName, gossip_menu_id, minlevel, maxlevel, Health_mod, Mana_mod, Armor_mod, faction_A, faction_H, npcflag, speed_walk, speed_run, scale, rank, dmg_multiplier, unit_class, unit_flags, type, type_flags, InhabitType, RegenHealth, flags_extra) VALUES
('95002', '35253', "Naios", "Developer", 'Speak ', '', 71, 71, 1.56, 1.56, 1.56, 35, 35, 3, 1, 1.14286, 1.25, 1, 1, 1, 2, 7, 138936390, 3, 1, 2);

REPLACE INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`)
VALUES (95002, 0, 0, 1, 0, 0, '');

REPLACE INTO `creature_equip_template` (`entry`, `equipentry1`, `equipentry2`, `equipentry3`) VALUES (58000, 45613, 0, 0);

DELETE FROM `creature` WHERE `id` = 95002;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(95002, 1, 1, 2, 1, 58000, 5484.12, -3560.63, 1569.24, 2.10896, 300, 0, 0, 6494, 0, 0, 0, 0, 0, 0),
(95002, 1, 1, 1, 1, 58000, 16230.3, 16284.3, 13.9248, 4.56861, 300, 0, 0, 6494, 0, 0, 0, 0, 0, 0);


DELETE FROM `creature_text` WHERE `entry` = 95002;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (95002, 0, 0, 'Hello $N, $BI hope you enjoy my Repack. If you have questions or you want a special thing be fixed just contact me.', 0, 0, 0, 0, 0, 0, '');
/*INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (95002, 0, 1, 'I hope you enjoy my Repack.', 0, 0, 0, 0, 0, 0, '');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES (95002, 0, 2, 'If you have questions or you want a special thing be fixed just contact me.', 0, 0, 0, 0, 0, 0, '');*/

SET @ENTRY := 95002;
SET @SOURCETYPE := 0;

DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=@SOURCETYPE;
UPDATE creature_template SET AIName="SmartAI" WHERE entry=@ENTRY LIMIT 1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES 
(@ENTRY,@SOURCETYPE,0,1,64,0,100,0,0,0,0,0,72,0,0,0,0,0,0,7,0,0,0,0.0,0.0,0.0,0.0,"On Speak start event"),
(@ENTRY,@SOURCETYPE,1,0,61,0,100,0,0,0,0,0,1,0,2000,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,"");
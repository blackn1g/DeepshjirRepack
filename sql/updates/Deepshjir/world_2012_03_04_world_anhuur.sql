-- Temple Guardian Anhuur

DELETE FROM `creature_text` WHERE `entry` = 39425;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES 
('39425', '0', '0', 'Turn back, intruders! These halls must not be disturbed!', '1', '0', '0', '0', '0', '18580', 'VO_HO_Anhuur_Engage'), 
('39425', '4', '0', 'Temple Guardian Anhuur becomes protected by his defense beacons! Disable them by pulling the levers below!', '3', '0', '0', '0', '0', '0', 'VO_HO_Anhuur_Special'),
('39425', '3', '0', 'What... have you... done? ', '1', '0', '0', '0', '0', '18579', 'VO_HO_Anhuur_Death'),
('39425', '2', '0', 'A product of your own insolence!', '1', '0', '0', '0', '0', '18583', 'VO_HO_Anhuur_Slay02'),
('39425', '1', '0', 'Beacons of light, bestow upon me your aegis!', '1', '0', '0', '0', '0', '18581', 'VO_HO_Anhuur_Event'),
('39425', '2', '1', 'I regret nothing !', '1', '0', '0', '0', '0', '18582', 'VO_HO_Anhuur_Slay01');

UPDATE `gameobject_template` SET `ScriptName` = 'go_beacon_of_light' WHERE `entry` = 203133;
UPDATE `gameobject_template` SET `ScriptName` = 'go_beacon_of_light' WHERE `entry` = 203136;
UPDATE `gameobject_template` SET `flags` = 4 WHERE `entry` = 206506;
UPDATE `creature_template` SET `modelid1` = 1126, `modelid2` = 11686, `flags_extra` = 130 WHERE `entry` = 40183;
UPDATE `creature_template` SET `ScriptName` = 'boss_temple_guardian_anhuur' WHERE `creature_template`.`entry` =39425;
DELETE FROM `creature_template_addon` WHERE (`entry`=40283);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES (40283, 0, 0, 0, 1, 0, '75116 0');

UPDATE `creature_template` SET `equipment_id` = 39425 WHERE `entry` = 39425;

-- Temple Guardian Anhuur loot
DELETE FROM `creature_loot_template` WHERE `entry`= 39425;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
('39425','55889','0','1','1','1','1'),
('39425','55888','0','1','1','1','1'),
('39425','55886','0','1','1','1','1'),
('39425','55887','0','1','1','1','1'),
('39425','55890','0','1','1','1','1');

-- Birde spawn
DELETE FROM gameobject WHERE id = 206506;
INSERT INTO `gameobject` (`id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES ('206506', '644', '3', '1', '-640.65', '304.617', '53.24', '3.13999', '0', '0', '0.999964', '-0.00849289', '300', '0', '1');





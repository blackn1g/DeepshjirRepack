/********************
* (C) DeepshjirCataclysm Repack
*  by Naios (2012)
********************/
-- Blackwing Descent Misc
 
DELETE FROM `gameobject` WHERE `guid` IN(189283);
DELETE FROM `gameobject` WHERE `id` IN(/*Deletes gobs that are spawned through this script*/207834, 402092, 204929, 203306,/*Deletes unofficial gobs*/ 202211, 205079, 24388, 203083, 20720, 402368, 185218, 185216, 185217) AND `map` = 669;

INSERT INTO `gameobject` (`id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
-- Onyxia Elevator
(207834, 669, 15, 2, -109.87, -223.157, 11.5354, 1.79425, 0, 0, 0.781537, 0.623858, 300, 1, 1),
-- Pre-Boss Gate
(402092, 669, 15, 1, -270.2, -224.415, 190.316, 3.1307, 0, 0, 0.999985, 0.00544851, 300, 0, 1),
-- Finkles Cage
(204929, 669, 15, 1, -115.558, 45.1754, 71, 3.05438, 0, 0, 0.999049, 0.0435902, 300, 0, 1),
-- Maloriaks Cauldron
(203306, 669, 15, 1, -105.803, -485.729, 73.4583, 3.08822, 0, 0, 0.999644, 0.0266813, 300, 0, 1),

-- Atramedes Door
(402368, 669, 15, 1, 53.670784, -224.593292, 94.5037, 3.130350, 0, 0, 0.999629, 0.0272188, 300, 0, 1);


DELETE FROM `creature` WHERE `id` IN(44418, 44202, 41440) AND `map` = 669;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
-- Finkle and Bil o tron
(44418, 669, 15, 1, 0, 0, -110.376, 37.4967, 72.0576, 4.31101, 300, 0, 0, 77490, 0, 0, 0, 0, 0, 0),
(44202, 669, 15, 1, 0, 0, -115.956, 42.477, 72.0559, 4.65658, 300, 0, 0, 774900, 0, 0, 0, 0, 0, 0);

-- Modifys existing spawnpositions
REPLACE INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
-- Maloriak
(1082133, 41378, 669, 15, 1, 0, 0, -106.063, -464.796, 73.4545, 1.53706, 300, 0, 0, 86579136, 0, 0, 0, 0, 0, 0);

REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `AIName`, `ScriptName`, `WDBVerified`) VALUES
(402092, 0, 9946, 'BLACKROCKV2_PORTCULLIS_02', '','','',1375, 32, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', '', 13623),
(203716, 11, 10407, 'Blackwing Descent Elevator', '', '', '', 0, 40, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 13623),
(207834, 11, 10363, 'Doodad_BlackWingV2_Elevator_Onyxia01', '', '', '', 0, 40, 1, 0, 0, 0, 0, 0, 0, 13333, 0, 0, 0, 0, 748, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 13623),
(402368, 0, 10463, 'BLACKWING_PORTCULLIS', '', '', '', 1375, 32, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', '', 13623),
(203306, 10, 9554, 'Doodad_BlackrockV2_LabRoom_Cauldron01', '', '', '', 0, 16, 0.73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 13623);


UPDATE `gameobject` SET `spawnMask` = 15 WHERE `map` = 669;

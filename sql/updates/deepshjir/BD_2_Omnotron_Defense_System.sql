-- Omnotron Defense System Blackwing Descent
-- by Naios

UPDATE `creature_template` SET `AIName`='', `ScriptName`='boss_magmatron' WHERE `entry`=42178 LIMIT 1;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='boss_toxitron' WHERE `entry`=42180 LIMIT 1;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='boss_electron' WHERE `entry`=42179 LIMIT 1;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='boss_arcanotron' WHERE `entry`=42166 LIMIT 1;

UPDATE `creature_template` SET `AIName`='', `ScriptName`='boss_omnotron' WHERE `entry`=42186 LIMIT 1;

DELETE FROM `smart_scripts` WHERE `entryorguid` IN(42178, 42180, 42179, 42166);

-- Trons
DELETE FROM `creature` WHERE `guid` IN(1082113, 1082115, 1082117, 1082119);
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(1082115, 42180, 669, 15, 1, 0, 0, -333.731, -400.515, 214.008, 6.25277, 300, 0, 0, 25767600, 0, 0, 2, 0, 0, 0),
(1082117, 42179, 669, 15, 1, 0, 0, -341.526, -405.203, 213.975, 1.13984, 300, 0, 0, 25767600, 0, 0, 0, 0, 0, 0),
(1082119, 42166, 669, 15, 1, 0, 0, -316.568, -410.917, 214.043, 1.65822, 300, 0, 0, 25767600, 0, 0, 0, 0, 0, 0),
(1082113, 42178, 669, 15, 1, 0, 0, -307.968, -406.23, 214.008, 1.98808, 300, 0, 0, 25767600, 0, 0, 0, 0, 0, 0);

UPDATE `creature_template` SET `MovementType` = 2 WHERE `entry`=42180 LIMIT 1;

-- Omnotron
DELETE FROM `creature` WHERE `id` = 42186 AND `map` = 669;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `DeathState`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(42186, 669, 15, 1, 0, 0, -324.858, -419.466, 213.897, 1.56003, 300, 0, 1, 42000, 0, 0, 0, 0, 0, 0);

DELETE FROM `creature_template_addon` WHERE `entry` = 42186;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`)
VALUES (42186, 0, 0, 0, 0, 0, '/* ToDo: Omnotron Visual Effect is Missing*/');

-- 57886 0 57887

-- Toxitron Waypoints
DELETE FROM `creature_addon` WHERE `guid` = 1082115;
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(1082115,10821150, 0, 0, 0, 0, '');

DELETE FROM `waypoint_data` WHERE `id` = 10821150;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) VALUES
(10821150, 1, -315.853, -400.56, 213.974, 1500, 0, 0, 100, 0),
(10821150, 2, -334.152, -400.321, 214.005, 1500, 0, 0, 100, 0);

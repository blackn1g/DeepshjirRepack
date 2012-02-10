/********************
* (C) DeepshjirCataclysm Repack
*  by Naios (2012)
********************/
-- Chimaeron (Blackwing Descent Updates)

-- Chimaeron sleep Aura
REPLACE INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(1082137, 0, 0, 0, 1, 0,'84186 0');

UPDATE `creature_template` SET `ScriptName`='boss_chimaeron' WHERE `entry` = 43296;
UPDATE `creature_model_info` SET `combat_reach`=8 WHERE `modelid`=33308 LIMIT 1;

-- Finkle Einhorn
UPDATE `creature_template` SET `npcflag` = 1, `ScriptName`='mob_finkle_einhorn' WHERE `entry` = 44202;

-- Bile O Tron
UPDATE `creature_template` SET `ScriptName`='mob_bile_o_tron' WHERE `entry` = 44418;
UPDATE `creature_template` SET `InhabitType`=5 WHERE `entry` = 44418;

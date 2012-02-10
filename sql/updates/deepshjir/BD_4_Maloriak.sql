/********************
* (C) DeepshjirCataclysm Repack
*  by Naios (2012)
********************/
-- Maloriak (Blackwing Descent) Updates

UPDATE `creature_template` SET `AIName`='', `ScriptName`='boss_maloriak' WHERE `entry`=41378 LIMIT 1;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 41378;

UPDATE `creature_model_info` SET `combat_reach`=6 WHERE `modelid`=33186 LIMIT 1;

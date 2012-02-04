/********************
* (C) DeepshjirCataclysm Repack
*  by Naios (2012)
********************/
-- Conalcave of Wind (Throne of the four Winds)

UPDATE `creature_template` SET `ScriptName`='boss_anshal' , AIName="" WHERE `entry`=45870 LIMIT 1;
UPDATE `creature_template` SET `ScriptName`='boss_nezir' , AIName="" WHERE `entry`=45871 LIMIT 1;
UPDATE `creature_template` SET `ScriptName`='boss_rohash' , AIName="" WHERE `entry`=45872 LIMIT 1;

DELETE FROM `smart_scripts` WHERE `entryorguid` IN(45870, 45871, 45872);

-- Anshal
-- Soothing Breeze
DELETE FROM `creature_template_addon` WHERE `entry` = 46246;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`)
VALUES (46246, 0, 0, 0, 0, 0, '86208 0 27986 0');

UPDATE `creature_template` SET `unit_flags`=33554948, `type_flags`=1024, `minlevel`=83, `maxlevel`=83, `faction_A`=16, `faction_H`=16, `rangedattackpower`=5000 WHERE `entry`=46246 LIMIT 1;

-- Nezir
-- Ice Patch
DELETE FROM `creature_template_addon` WHERE `entry` = 46186;
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`)
VALUES (46186, 0, 0, 0, 0, 0, '86107 0');

UPDATE `creature_template` SET `unit_flags`=33554948, `type_flags`=1024, `minlevel`=83, `maxlevel`=83, `faction_A`=16, `faction_H`=16, `rangedattackpower`=5000 WHERE `entry`=46186 LIMIT 1;

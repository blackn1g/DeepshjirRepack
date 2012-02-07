
-- Inhabit Type fix
UPDATE `creature_template` SET `InhabitType` = 7 WHERE `InhabitType` > 7;

-- creature template fix
UPDATE `creature_template` SET equipment_id = 0 WHERE `equipment_id` NOT IN (SELECT `entry` FROM `creature_equip_template`);
DELETE FROM achievement_criteria_data WHERE `type` IN (5, 56, 8);

UPDATE quest_template SET ReqCreatureOrGOCount1 = 1 WHERE ReqCreatureOrGOId1 != 0 AND ReqCreatureOrGOCount1 = 0;
DELETE FROM gameobject_loot_template WHERE `entry` NOT IN (SELECT `entry` FROM `gameobject_template`);
DELETE FROM creature_questrelation WHERE `quest` NOT IN (SELECT `entry` FROM `gameobject_template`);
-- UPDATE gameobject_template SET `type` = 14 WHERE `entry` IN (SELECT `entry` FROM `gameobject_loot_template`);
DELETE FROM pool_gameobject WHERE `guid` NOT IN (SELECT `guid` FROM `gameobject`);
DELETE FROM `creature_addon` WHERE `guid` NOT IN (SELECT `guid` FROM `creature`);

UPDATE creature_template SET AIName = '' WHERE `entry` BETWEEN 49317 AND 49334;
UPDATE creature_template SET ScriptName = '' WHERE `entry` IN(49317, 49318, 49320, 49319, 49321, 49323, 49324, 49325, 49326, 49327, 49328, 49330, 49334);

DELETE FROM creature_template_addon WHERE entry IN(15466, 15902, 36838, 36839, 42186, 52983);

UPDATE creature_template SET unit_class = 1, InhabitType = 1 WHERE `entry` = 58000;

DELETE FROM creature_template WHERE `entry` = 35317;

DELETE FROM gameobject_template WHERE `entry` = 185879;

DELETE FROM spell_script_names WHERE spell_id IN(49145, 49158, 33851, 6143, 5604, 5698, 20425, 25899, 58630, 59046, 59450, 62791, 63521, 64899, 64985, 65074, 65195, 68184, 68572, 68574, 69366);

-- not sure
DELETE FROM spell_script_names WHERE spell_id IN(50294, 49158, 49145, 33851, 6143, );

DELETE FROM condition WHERE `SourceGroup` IN(24095, 47305, 23342);

DELETE FROM achievement_criteria_data WHERE criteria_id IN(10049, 10050, 10047, 10055, 10051, 10048,10045, 10062, 10054, 10046, 10063, 10044);

DELETE FROM pool_quest WHERE entry IN(25155, 25154, 25157, 25105, 25161, 25160, 25159, 25158,26190,26183,26153,26192,26177,26227,26235,26226,26233,26220,26414,26442,26536,26420,26543, 26557, 26572, 26556, 26588, 11370, 11373, 11362, 11368, 11388, 11386, 11382, 11363, 11499, 11375, 11354, 11378, 11374, 11372, 11384, 11369, 14074, 14152, 14077, 14080, 14143, 14136, 14144, 14140, 14092, 14141, 14145, 14076, 14090, 14112, 4101, 14102, 14104, 14105, 14107, 14108);

-- by Arkania
-- CREATURES
 DELETE FROM `creature` WHERE `id` NOT IN (SELECT `entry` FROM `creature_template`);                           -- Dinamic // 'creature' table
 DELETE FROM `creature_template_addon` WHERE `entry` NOT IN (SELECT `entry` FROM `creature_template`);          -- Dinamic // 'creature_template_addon' table
 DELETE FROM `creature_formations` WHERE `leaderGUID` NOT IN (SELECT `guid` FROM `creature`)
OR `memberGUID` NOT IN (SELECT `guid` FROM `creature`);                                                         -- Dinamic // 'creature_formations' table
 DELETE FROM `creature_onkill_reward` WHERE `creature_id` NOT IN (SELECT `entry` FROM `creature_template`); -- Dinamic // 'creature_onkill_reputation' table
 DELETE FROM `linked_respawn` WHERE `linkedguid` NOT IN (SELECT `guid` FROM `creature`);                        -- Dinamic // 'linked_respawn' table
 DELETE FROM `creature_addon` WHERE `guid` NOT IN (SELECT `guid` FROM `creature`);                              -- Dinamic // 'creature_addon' table
 DELETE FROM `creature_ai_scripts` WHERE `creature_id` IN (SELECT `entry` FROM `creature_template` WHERE `AIName`='SmartAI');
-- -------------------------------------------------------------------------
-- GAMEOBJECTS
 DELETE FROM `gameobject` WHERE `id` NOT IN (SELECT `entry` FROM `gameobject_template`);                        -- Dinamic // 'gameobject' table
 DELETE FROM `gameobject_scripts` WHERE `id` NOT IN (SELECT `guid` FROM `gameobject`);                          -- Dinamic // 'gameobject_scripts' table
 DELETE FROM `gameobject_involvedrelation` WHERE `id` NOT IN (SELECT `entry` FROM `gameobject_template`);       -- Dinamic // 'gameobject_involvedrelation' table
 DELETE FROM `gameobject_loot_template` WHERE `entry` NOT IN (SELECT `entry` FROM `gameobject_template`);       -- Dinamic // 'gameobject_loot_template' table
 DELETE FROM `gameobject_questrelation` WHERE `id` NOT IN (SELECT `entry` FROM `gameobject_template`);          -- Dinamic
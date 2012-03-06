-- Halls of Origination
UPDATE `instance_template` SET `script`='instance_halls_of_origination' WHERE `map`=644;


-- Scriptnames
UPDATE creature_template SET scriptname = "boss_earthrager_ptah" WHERE entry = 39428;

-- (NOT NOW) UPDATE creature_template SET scriptname = "npc_horror" WHERE entry = 40810;


-- Texts
DELETE FROM `creature_text` WHERE `entry` = 39428;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES 
('39428', '0', '0', 'More carrion for the swarm... ', '1', '0', '0', '0', '0', '18906', 'PTAH_SAY_AGGRO'), 
('39428', '1', '0', 'Ptah... is... no more... ', '1', '0', '0', '0', '0', '18905', 'PTAH_SAY_DEATH'),
('39428', '2', '0', 'Dust to dust... ', '1', '0', '0', '0', '0', '18907', 'PTAH_SAY_SLAY_1'),
('39428', '3', '1', 'HYAAAH!!', '1', '0', '0', '0', '0', '18908', 'PTAH_SAY_SLAY_2');


-- Loot
DELETE FROM `creature_loot_template` WHERE `entry`= 39428;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
('39428','56097','0','1','1','1','1'),
('39428','56095','0','1','1','1','1'),
('39428','56093','0','1','1','1','1'),
('39428','56096','0','1','1','1','1'),
('39428','56094','0','1','1','1','1');
DELETE FROM `creature_loot_template` WHERE `entry`= 48714;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
('48714','56423','0','1','1','1','1'),
('48714','56424','0','1','1','1','1'),
('48714','56425','0','1','1','1','1'),
('48714','56426','0','1','1','1','1'),
('48714','56422','0','1','1','1','1');


-- Hitbox
UPDATE creature_model_info SET combat_reach = 10.0 WHERE modelid = 31450;

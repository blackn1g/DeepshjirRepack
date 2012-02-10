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

/*
REPLACE INTO `gossip_menu` (`entry`, `text_id`) VALUES
(60050, 310000);

DELETE FROM `npc_text` WHERE `ID` IN(310000);
INSERT INTO `npc_text`(`ID`,`text0_0`,`text0_1`) VALUES
(310000,'Leaping Leper Gnomes! You\'re a sight for sore eyes. I was worried the giant magma worm out front would deter visitors. Now that you\'re here you can give me a hand.','testtesttest');

REPLACE INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`, `box_money`, `box_text`) VALUES
(60050, 0, 0, 'Leaping Leper Gnomes! You\'re a sight for sore eyes. I was worried the giant magma worm out front would deter visitors. Now that you\'re here you can give me a hand.', 0, 0, 0, 0, 0, 0, 0, NULL);
*/

-- Bile O Tron
UPDATE `creature_template` SET `ScriptName`='mob_bile_o_tron' WHERE `entry` = 44418;
UPDATE `creature_template` SET `InhabitType`=5 WHERE `entry` = 44418;


/* List of forgotten Scriptnames

# BOT
boss_chogall
boss_halfus_wyrmbreaker
npc_proto_behemoth
boss_theralion
boss_valiona
spell_dazzling_destruction
instance_the_bastion_of_twilight

# BWD
boss_bd_nefarian
boss_maloriak
boss_atramedes
boss_chimaeron

# Gilneas
npc_gilneas_city_guard_phase1
npc_gilneas_city_guard_phase2
npc_prince_liam_greymane_phase1
npc_prince_liam_greymane_phase2
npc_rampaging_worgen
npc_rampaging_worgen2
npc_lieutenant_walden
npc_king_genn_greymane
npc_lord_darius_crowley_c2
*/

-- Gilneas
DELETE FROM `spell_script_names` WHERE `spell_id` = '68228';
INSERT `spell_script_names` (`spell_id`,`ScriptName`) VALUES ('68228', 'spell_rescue_krennan');

UPDATE `creature_template` SET `ScriptName` = 'npc_lord_darius_crowley' WHERE `entry` = '35077';
UPDATE `creature_template` SET `ScriptName` = 'npc_king_genn_greymane' WHERE `entry` = '35550';
UPDATE `creature_template` SET `ScriptName` = 'npc_lord_darius_crowley_c2' WHERE `entry` = '35552';
UPDATE `creature_template` SET `ScriptName` = 'npc_lord_darius_crowley_c3' WHERE `entry` = '35566';
UPDATE `creature_template` SET `ScriptName` = 'npc_king_genn_greymane_c2' WHERE `entry` = '36332';

DELETE FROM `spell_script_names` WHERE `spell_id` = '68555';
INSERT `spell_script_names` (`spell_id`,`ScriptName`) VALUES ('68555', 'spell_keg_placed');
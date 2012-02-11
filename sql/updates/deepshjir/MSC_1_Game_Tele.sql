DELETE FROM `game_tele` WHERE `name` IN('mall', 'magmaw', 'omnotron', 'atramedes', 'chimaeron', 'nefarian', 'maloriak');

INSERT INTO `game_tele` (`position_x`, `position_y`, `position_z`, `orientation`, `map`, `name`) VALUES
(5497.15, -3572.26, 1569.26, 2.14116, 1, 'mall'),

-- Blackwing Descent
(-339.18, -125.866, 213.874, 1.50274, 669, 'magmaw'),
(-324.746, -357.553, 213.871, 4.68203, 669, 'omnotron'),
(83.9655, -224.608, 76.6461, 6.27089, 669, 'atramedes'),
(-106.11, -39.8849, 76.6324, 1.53651, 669, 'chimaeron'),
(-172.31, -224.721, 40.3983, 0.004987, 669, 'nefarian'),
(-106.142, -404.989, 76.6283, 4.67889, 669, 'maloriak');

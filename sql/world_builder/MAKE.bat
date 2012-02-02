@echo off

echo Merging Updates...
del deepshjir_world_full.sql
copy /A Build_base_DB\world_base.sql +..\updates\deepshjir\*sql deepshjir_world_full.sql

echo Execute updates...

set svr=localhost
set user=server
set pass=server
set port=3306
set wdb=rg_world

Build_base_DB\mysql -q -s -h %svr% --user=%user% --password=%pass% --port=%port% --port=3306 deepshjir_world<  deepshjir_world_full.sql

echo Dumping World Database...

Build_base_DB\mysqldump.exe --host=%svr% --user=%user% --password=%pass% --port=%port% --port=3306 --routines --skip-comments deepshjir_world > deepshjir_world_full.sql
del merged.sql
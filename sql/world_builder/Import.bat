@echo off

echo Importing Database...

set svr=localhost
set user=server
set pass=server
set port=3306
set wdb=rg_world

Build_base_DB\mysql -q -s -h %svr% --user=%user% --password=%pass% --port=%port% --port=3306 deepshjir_world<  deepshjir_world_full.sql
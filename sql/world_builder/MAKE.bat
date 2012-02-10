@echo off

del deepshjir_world_full.sql
cls
echo Importing World DB...

set svr=localhost
set user=server
set pass=server
set port=3306
set wdb=rg_world

Build_base_DB\mysql -q -s -h %svr% --user=%user% --password=%pass% --port=%port% --port=3306 deepshjir_world<  Build_base_DB\world_base.sql

echo Update DB

FOR %%C IN (..\updates\skyfire\*.sql) DO (
ECHO Skyfire Update: %%~nxC
Build_base_DB\mysql --host=%host% --user=%user% --password=%pass% --port=%port% deepshjir_world < "%%~fC"
)

FOR %%C IN (..\updates\world\*.sql) DO (
ECHO ArkCore Update: %%~nxC
Build_base_DB\mysql --host=%host% --user=%user% --password=%pass% --port=%port% deepshjir_world < "%%~fC"
)

FOR %%C IN (..\updates\deepshjir\*.sql) DO (
ECHO Deepshjir Update: %%~nxC
Build_base_DB\mysql --host=%host% --user=%user% --password=%pass% --port=%port% deepshjir_world < "%%~fC"
)

echo Fixing errors...
Build_base_DB\mysql -q -s -h %svr% --user=%user% --password=%pass% --port=%port% --port=3306 deepshjir_world<  db_error_fixer_extended.sql

echo Dumping World Database...

Build_base_DB\mysqldump.exe --host=%svr% --user=%user% --password=%pass% --port=%port% --port=3306 --routines --skip-comments deepshjir_world > deepshjir_world_full.sql
@echo off

cls


set svr=localhost
set user=server
set pass=server
set port=3306
set wdb=world_deepshjir

echo Deleting World DB...
rem Build_base_DB\mysql -q -s -h %svr% --user=%user% --password=%pass% --port=%port% --port=3306 world_deepshjir <  Build_base_DB\clean.sql

echo Merging Updates and World DB
copy /A Build_base_DB\world_base.sql +..\updates\deepshjir\*sql deepshjir_world_full.sql

echo Importing World DB...
Build_base_DB\mysql -q -s -h %svr% --user=%user% --password=%pass% --port=%port% --port=3306 world_deepshjir < deepshjir_world_full.sql

echo Update DB


FOR %%C IN (..\updates\skyfire\*.sql) DO (
ECHO Skyfire Update: %%~nxC
rem Build_base_DB\mysql --host=%host% --user=%user% --password=%pass% --port=%port% world_deepshjir < "%%~fC"
)


FOR %%C IN (..\updates\ccdb\*.sql) DO (
ECHO CCDB Update: %%~nxC
rem Build_base_DB\mysql --host=%host% --user=%user% --password=%pass% --port=%port% world_deepshjir < "%%~fC"
)


FOR %%C IN (..\updates\world\*.sql) DO (
ECHO ArkCore Update: %%~nxC
rem Build_base_DB\mysql --host=%host% --user=%user% --password=%pass% --port=%port% world_deepshjir < "%%~fC"
)


FOR %%C IN (..\updates\deepshjir\*.sql) DO (
ECHO Deepshjir Update: %%~nxC
rem Build_base_DB\mysql --host=%host% --user=%user% --password=%pass% --port=%port% world_deepshjir < "%%~fC"
)

FOR %%C IN (..\updates\extern_sai\*.sql) DO (
ECHO SAI Update: %%~nxC
rem Build_base_DB\mysql --host=%host% --user=%user% --password=%pass% --port=%port% world_deepshjir < "%%~fC"
)

echo Fixing errors...
Build_base_DB\mysql --host=%host% --user=%user% --password=%pass% --port=%port% --port=3306 world_deepshjir <  db_error_fixer_extended.sql

echo Dumping World Database...

Build_base_DB\mysqldump.exe --host=%host% --user=%user% --password=%pass% --port=%port% --port=3306 --routines --skip-comments world_deepshjir > deepshjir_world_full.sql

echo Done
pause
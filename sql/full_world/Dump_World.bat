@echo off

echo DeepshjirCataclysm Repack
echo Dumping World Database...

mysqldump.exe --host=localhost --user=server --password=server --port=3306 --routines --skip-comments deepshjir_world > deepshjir_world.sql
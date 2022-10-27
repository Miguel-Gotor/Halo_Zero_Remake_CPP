echo off
del .\screenshots\* /Q
del "Halo Zero.exe"
mingw32-make
start "" "Halo Zero.exe"
: taskmgr

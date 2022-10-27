ECHO OFF
ECHO id ICON "../assets/icons/original.ico" >> icon.rc
windres icon.rc -O coff -o icon.res
del /f icon.rc
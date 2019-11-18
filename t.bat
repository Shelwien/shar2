@echo off

rd /S /Q tmp
mkdir tmp
del sharive sharive2 >nul 2>&1

shar.exe a99 sharive ..\Lib\

cat sharive | shar.exe x - tmp

shar.exe a99 - tmp\ | cat >sharive2

for %%a in (sharive,sharive2) do echo %%~za - %%a
md5sum sharive sharive2

rd /S /Q tmp
del sharive sharive2 >nul 2>&1


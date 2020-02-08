@echo off

IF NOT EXIST "bin\assets\" (
	mkdir "bin\assets"
)
xcopy /s assets "bin\assets\" /y

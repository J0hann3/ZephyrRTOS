@echo off
set PATH_ZEPHYR=C:\Users\JohanneVIGNY\zephyrproject1

@REM set zephyr directory path in the env
set ZEPHYR_BASE=%PATH_ZEPHYR%\zephyr

@REM activate python virtual environment
%PATH_ZEPHYR%\.venv\Scripts\activate.bat
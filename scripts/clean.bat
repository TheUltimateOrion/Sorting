@echo off

:: Remove all untracked files from version control
git clean -xfd

:: Do the same for all submodules
git submodule foreach --recursive git clean -xfd

:: Reset HEAD to last commit
git reset --hard

:: Do the same to submodules
git submodule foreach --recursive git reset --hard

:: Reinitialize submodules
git submodule update --init --recursive
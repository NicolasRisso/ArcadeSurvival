@echo off
set PROJECT_NAME=ArcadeSurvival
set SOURCE_FILES=main.c
set OUT_DIR=bin
set COMPILER=gcc

if not exist %OUT_DIR% mkdir %OUT_DIR%

echo Building %PROJECT_NAME%...

%COMPILER% %SOURCE_FILES% -o %OUT_DIR%/%PROJECT_NAME%.exe ^
    -Iraylib/include ^
    -Lraylib/lib ^
    -lraylib ^
    -lgdi32 ^
    -lwinmm

if %ERRORLEVEL% equ 0 (
    echo Build successful! Binary is in %OUT_DIR%
    if exist raylib\lib\raylib.dll (
        copy raylib\lib\raylib.dll %OUT_DIR%\ >nul
        echo Copied raylib.dll to %OUT_DIR%
    ) else if exist raylib\raylib.dll (
        copy raylib\raylib.dll %OUT_DIR%\ >nul
        echo Copied raylib.dll to %OUT_DIR%
    )
) else (
    echo Build failed!
)
pause

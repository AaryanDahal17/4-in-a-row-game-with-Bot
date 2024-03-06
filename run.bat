@echo off



echo.
echo.
echo compiling
echo.
echo.

@CALL g++ src/bot.cpp -Iinclude -Llibs/SDL2 src/Main.cpp -o bin\main.exe -lmingw32 -lSDL2main -lSDL2


echo compiled

echo.
echo.
echo.
echo.


@CALL bin\main.exe

echo.
echo.
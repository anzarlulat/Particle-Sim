@echo off
set SFML=C:\Users\Anzar\Documents\Coding\_C++\Libraries\SFML-3.1.0

cl /nologo /EHsc /std:c++17 /MD /I"%SFML%\include" ^
    src\*.cpp ^
    /Fe:ParticleSim.exe ^
    /link /LIBPATH:"%SFML%\lib" ^
    sfml-graphics.lib ^
    sfml-window.lib ^
    sfml-system.lib

if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

copy /Y "%SFML%\bin\sfml-graphics-3.dll" .
copy /Y "%SFML%\bin\sfml-window-3.dll" .
copy /Y "%SFML%\bin\sfml-system-3.dll" .

echo Build complete.

echo.
echo ================
ParticleSim.exe
echo ================

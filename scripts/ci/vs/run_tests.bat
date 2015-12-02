cd %APPVEYOR_BUILD_FOLDER%\tests
set TESTS_PLATFORM=%PLATFORM%
set STATUS=0
if "%PLATFORM%" equ "x86" set TESTS_PLATFORM=Win32
FOR /D %%G IN (*) DO ( 
    echo %APPVEYOR_BUILD_FOLDER%\tests\%%G
    cd %APPVEYOR_BUILD_FOLDER%\tests\%%G
    FOR /D %%E IN (*) DO ( 
        echo %APPVEYOR_BUILD_FOLDER%\tests\%%G\%%E
        cd %APPVEYOR_BUILD_FOLDER%\tests\%%G\%%E
        msbuild %%E.sln /p:Configuration=Debug /p:Platform=%TESTS_PLATFORM%
        if ERRORLEVEL 1 (
            appveyor AddTest -Name %%E -Framework ofxUnitTests -FileName %%E.sln -Outcome Failed -Duration 0 -StdOut "Error compiling"
            SET STATUS=1
        ) else (
            cd bin
            %%E_debug.exe
            if ERRORLEVEL 1 echo "Finished with error" & SET STATUS=1
        )
    )
)
cd ..
echo "Tests finished with status %STATUS%"
exit /B %STATUS%

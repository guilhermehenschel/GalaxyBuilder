@echo off
echo Galaxy Builder Test Runner (Batch)
echo ===================================

cd /d "%~dp0"
powershell.exe -ExecutionPolicy Bypass -File "run_tests.ps1" %*

if %ERRORLEVEL% neq 0 (
    echo.
    echo Tests failed or PowerShell is not available.
    echo You can run the tests manually from the build\bin\tests directory.
    pause
    exit /b %ERRORLEVEL%
)

echo.
echo All tests completed successfully!
pause

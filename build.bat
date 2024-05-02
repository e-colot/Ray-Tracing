@echo off
echo Cleaning up previous build artifacts...
make clean || (
    echo make clean failed. Exiting.
    pause
    exit /b
)

echo Compiling project...
make test || (
    echo make test failed. Exiting.
    pause
    exit /b
)

echo Project compiled successfully. Press any key to continue...
pause >nul

echo Running the test executable...
test || (
    echo Running the test executable failed. Exiting.
    pause
    exit /b
)

echo Test completed. Press any key to exit...
pause >nul
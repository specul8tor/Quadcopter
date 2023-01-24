@echo off
echo "building ..."
if "%1"=="-l" (
    echo "linking..."
    cmake -G "MinGW Makefiles" ../
    echo "linking finished"
) else if NOT "%1"=="-l" (
    echo "no linking"
)
echo "making..."
make
echo "making done"
if "%2"=="-c" (
    echo "deploying..."
    sxc-xdt --hid --hex Quadcopter.hex %3
    echo "deploying finished"
)
echo "build finished"
@echo off

if "%1"=="-ni" (
    cd ../build
)

echo "building ..."
if "%2"=="-l" (
    echo "linking..."
    cmake -G "MinGW Makefiles" ../
    echo "linking finished"
) else if NOT "%2"=="-l" (
    echo "no linking"
)
echo "making..."
make
echo "making done"
if "%3"=="-c" (
    echo "deploying..."
    sxc-xdt --hid --hex Quadcopter.hex %4
    echo "deploying finished"
)
echo "build finished"
@echo  off
echo "Initializing Build Started..."
mkdir build
cd scripts
echo "Setting up compilier path..."
python setConfig.py %1
echo "Finished setting compilier path"
build -l -c
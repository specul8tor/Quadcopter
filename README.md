# Quadcopter

Capstone project for ECE 490/491 for the University of Alberta.

## Initialization

NOTE: Assumes you have added the correct commands to PATH.

Find path that has the XInC2 Ressoures 062019 Folder and make sure you are in the root directory and run:

```$ init "PATH-TO-XInC2-Ressoures-062019"```

That will initizalize the environment and run a full build that has its artifacts available in the build folder. It will also place you in the scripts directory. From there you can run the build cmd that has the following flags:

1. ```-l``` to run linker cmd and ```-nl``` to not run linker.  
2. ```-c``` to upload to board and ```-nc``` to not upload.
3. add ```-t``` to open shell after uploading.

Examples:

* ```build -nl -c -t```: no link, upload and open shell
* ```build -l -c```: link, upload and no shell

Specific commands:

* Linker: ```cmake -G "MinGW Makefiles" .```
* Make: ```make```
* Upload: ```sxc-xdt --hid --hex Quadcopter.hex```
* Upload and Open Shell: ```sxc-xdt --hid --hex Quadcopter.hex -t```

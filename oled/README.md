# Build
Initialize
```
mkdir build 
cd build/
cmake -DPICO_BOARD=pico2_w -DCMAKE_EXPORT_COMPILE_COMMANDS=ON  ..
```

Then in still in `build/`
```
make
```

# Deploy firmware
```
make && picotool load -x -f picoproj.uf2 
```
`-f` flag forces device into BOOTSEL mode to flash unplug and replug in BOOTSELF
`-x` is to execute after flashing (reboot, don't wait for auto reboot from device which is a few seconds)


**Legacy way**:
Hold the BOOTSEL button until BEFORE AND WHILE plugging the micro USB.

Copy the UF2 file to the pico 2W:
```
cp blink.uf2 /mnt/pico 
```
or 
```
picotool load picoproj.uf2 && picotool reboot
```

Device might show up as hardware in `lsblk` too with a /dev/sdX mountpoint. Update `/etc/fstab` to auto mount

# Reading serial output
```
picocom /dev/ttyACM0 -b 115200
```

# Picotool
You should build or get a release of picotool and cp it to `/usr/local/bin`

Show info of BOOTSEL device and loaded program
```
sudo picotool info -a
```

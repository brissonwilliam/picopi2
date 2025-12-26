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
Hold the BOOTSEL button until BEFORE AND WHILE plugging the micro USB.

Copy the UF2 file to the pico 2W:
```
cp blink.uf2 /run/media/boat/RP2350/
```

Device might show up as hardware in `lsblk` too with a /dev/sdX mountpoint.

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

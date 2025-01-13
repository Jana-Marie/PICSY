# Picsy

An ESP32 based Pixel-Art-Camera, shooting color-indexed pixel art alike PNGs directly onto an SD card.

![](/img/out-scaled-rotated.png)

### Setup

flash the ESP32-CAM, see (/firmware)[/firmware], afterwards copy `ubuf2png.py` onto the ESP32-CAM with `ampy -p /dev/ttyUSB0 -b 115200 put ubuf2png.py`

### Usage

This is WIP and will be moved to saving the PNG to a SD card directly soon.

#### Manual usage

```
ampy -p /dev/ttyUSB0 -b 115200 run boot.py > pic.log
```

Open a python console opy the contents of pic.log into a bytearray buffer in python, `import bin2png.py` and run `bin2png.writepng(buf, 'path/to/file.png')`
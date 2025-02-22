# Picsy

**UNDER DEVELOPMENT :3**

An ESP32 based Pixel-Art-Camera, shooting color-indexed pixel art alike PNGs directly onto an SD card.

<table>
  <tbody>
    <tr>
      <td>
        <img src="/img/wip_1.jpeg" title=""/>
      </td>
      <td>
        <img src="/img/wip_2.jpeg" title=""/>
      </td>
    </tr>
    <tr>
      <td colspan="2">
        <img src="/img/wip_3.jpeg" title=""/>
      </td>
    </tr>
    <tr>
      <td colspan="2">
        <img src="/img/out-scaled-rotated.png" title=""/>
      </td>
    </tr>
  </tbody>
</table>

### Setup

flash the ESP32-CAM, see (/firmware)[/firmware], afterwards copy `ubuf2png.py` onto the ESP32-CAM with `ampy -p /dev/ttyUSB0 -b 115200 put ubuf2png.py`

### Usage

This is WIP and will be moved to saving the PNG to a SD card directly soon.

#### Manual usage

Flash micropython as per [/main/micropython-bin](https://github.com/Jana-Marie/PICSY/tree/main/micropython-bin)

```
ampy -p /dev/ttyUSB0 -b 115200 put ubuf2png.py
ampy -p /dev/ttyUSB0 -b 115200 run boot.py
```

Open a python console opy the contents of pic.log into a bytearray buffer in python, `import bin2png.py` and run `bin2png.writepng(buf, 'path/to/file.png')`
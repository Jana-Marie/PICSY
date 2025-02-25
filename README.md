# PICSY

**UNDER DEVELOPMENT :3**

An ESP32 based Pixel-Art-Camera, shooting color-indexed pixel art alike PNGs directly onto an SD card.

<table>
  <tbody>
    <tr>
      <td colspan="2">
        <img src="/img/showcase_label.jpeg" title=""/>
      </td>
    </tr>
    <tr>
      <td>
        <img src="/img/showcase_bottom_l.jpeg" title=""/>
      </td>
      <td>
        <img src="/img/showcase_bottom_r.jpeg" title=""/>
      </td>
    </tr>
    <tr>
      <td colspan="2">
        <img src="/img/showcase_front.jpeg" title=""/>
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

Flash micropython as per [/main/micropython-bin](https://github.com/Jana-Marie/PICSY/tree/main/micropython-bin), afterwards copy `ubuf2png.py` onto the ESP32-CAM with `ampy -p /dev/ttyUSB0 -b 115200 put ubuf2png.py`

### Usage

This is WIP and will be moved to saving the PNG to a SD card directly soon.

#### Manual usage

Place an SD card into the SD-Card slot and run

```
ampy -p /dev/ttyUSB0 -b 115200 run boot.py
```
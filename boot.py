import ubuf2png
import camera
import machine, os
import random
import time

class Cam:
  def __init__(self, fileprefix, shutterpin):
    print('init cam')
    self.fileprefix = fileprefix
    vfs.mount(machine.SDCard(width=4), '/sd')
    camera.init(0, format=camera.GRAYSCALE, fb_location=camera.PSRAM, framesize=camera.FRAME_CIF)
    self.shutter = machine.Pin(shutterpin, machine.Pin.IN)
    
  def defineShutterButtonIrq(self):
    self.shutter.irq(handler=self.cam.takePhotoToSD(str(random.randint(1000,9999))), trigger=machine.Pin.IRQ_RISING)

  def pollShutterButton(self):
    return self.shutter.value()

  def takePhotoToSD(self, filename):
    _ = camera.capture()
    buf = camera.capture()

    pngimg = ubuf2png.PngImage(400,296,3,8)

    with open('sd/' + self.fileprefix + filename + '.png', 'wb') as f:
      f.write(pngimg.buildPng(buf))
      time.sleep_ms(200)
      f.close()
    time.sleep_ms(200)

  def deinit(self):
    camera.deinit()
    vfs.unmount('sd/')

if __name__ == '__main__':
  
  cam = Cam("awawa-", 33)
  
  time.sleep_ms(100)
  _ = cam.pollShutterButton()

  while True:
    if cam.pollShutterButton() == 1:
      cam.takePhotoToSD(str(random.randint(100,999)))
    else:
      time.sleep_ms(20)
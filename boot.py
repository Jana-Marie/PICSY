import ubuf2png
import camera
import machine, os
import random
import time

class Cam:
  def __init__(self, filePrefix, shutterpin):
    self.fileDir = "DCIM/100PICSY"
    self.filePrefix = filePrefix
    print('init filesystem')
    self.initFilesystem()
    self.fileIndex = self.getFileIndex()

    print('init cam')
    camera.init(0, format=camera.GRAYSCALE, fb_location=camera.PSRAM, framesize=camera.FRAME_CIF)
    self.shutter = machine.Pin(shutterpin, machine.Pin.IN)

  def initFilesystem(self):
    vfs.mount(machine.SDCard(width=4), '/sd')
    path = "/sd"
    for i in self.fileDir.split("/"):
        if not i in os.listdir(path):
            os.mkdir(path + "/" + i)
        path = path + "/" + i

  def getFileIndex(self):
      filelist = filter(lambda f: f.startswith(self.filePrefix) and f.endswith(".png"), os.listdir("/sd/" + self.fileDir))

      filelist = sorted(list(filelist))
      if len(filelist) == 0:
        return 0

      return int(filelist[-1][len(self.filePrefix):-4]) + 1

  def defineShutterButtonIrq(self):
    self.shutter.irq(handler=self.shutterIrqHandler, trigger=machine.Pin.IRQ_FALLING)

  def pollShutterButton(self):
    return self.shutter.value()

  def shutterIrqHandler(self, shutter):
    # wait 2ms to see whether the IRQ is a shutter IRQ (<=1ms) or a different I2C IRQ (>=1ms)
    self.shutter.irq(handler=None)
    time.sleep_ms(2)
    if shutter.value() == 1:
      print("Shutter was pressed!")
      self.takePhotoToSD()
    else:
      print("Other I2C interrupt")
      # todo STM32 status check via I2C
      print("Enabling IRQs again")
      self.defineShutterButtonIrq()
      pass

  def takePhotoToSD(self):
    print("Taking photo")
    buf = camera.capture()
    print("Processing photo...")
    pngimg = ubuf2png.PngImage(400,296,3,8)

    with open('/sd/' + self.fileDir + '/' + self.filePrefix + f"{self.fileIndex:04d}" + '.png', 'wb') as f:
      f.write(pngimg.buildPng(buf))
      f.close()

    # flush filesystem
    os.sync()
    print("Done!")
    self.fileIndex += 1
    print("Enabling IRQs again")
    self.defineShutterButtonIrq()

  def setCameraImageQuality(self, quality):
    if 10 <= quality <= 63:
      print("Set quality to ", quality)
      self.camera.quality(quality)

  def setCameraImageContrast(self, contrast):
    if -2 <= contrast <= 2:
      print("Set contrast to ", contrast)
      self.camera.contrast(contrast)

  def setCameraImageBrightness(self, brightness):
    if -2 <= brightness <= 2:
      print("Set brightness to ", brightness)
      self.camera.brightness(brightness)
  
  def setCameraImageSaturation(self, saturation):
    if -2 <= saturation <= 2:
      print("Set saturation to ", saturation)
      self.camera.saturation(saturation)

  def setCameraImageWhitebalance(self, whitebalance):
    if 0 <= whitebalance <= 4:
      print("Set whitebalance to ", whitebalance)
      self.camera.whitebalance(whitebalance)

  def setCameraImageEffect(self, effect):
    if 0 <= effect <= 4:
      print("Set effect to ", effect)
      self.camera.speffect(effect)

  def flipCameraImage(self, flip):
    if 0 <= flip <= 1:
      print("Flip image to ", flip)
      self.camera.flip(flip)

  def mirrorCameraImage(self, mirror):
    if 0 <= mirror <= 1:
      print("Mirror image to ", mirror)
      self.camera.mirror(mirror)

  def deinit(self):
    camera.deinit()
    vfs.unmount('/sd')

if __name__ == '__main__':
  
  cam = Cam("PICSY-", 33)
  time.sleep_ms(100)
  cam.defineShutterButtonIrq()

  while True:
    time.sleep_ms(10)

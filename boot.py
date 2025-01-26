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
    self.shutter.irq(handler=self.cam.takePhotoToSD(str(random.randint(1000,9999))), trigger=machine.Pin.IRQ_RISING)

  def pollShutterButton(self):
    return self.shutter.value()

  def takePhotoToSD(self):
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

  def deinit(self):
    camera.deinit()
    vfs.unmount('/sd')

if __name__ == '__main__':
  
  cam = Cam("PICSY-", 33)
  time.sleep_ms(100)

  while True:
    if cam.pollShutterButton() == 1:
      print("Button pressed. Smile!")
      cam.takePhotoToSD()
    else:
      time.sleep_ms(20)

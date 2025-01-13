import struct
import deflate
import binascii
import io
import sys

class PngImage:
  def __init__(self, width, height, colormode, colordepth):
    self.width = width
    self.height = height
    self.colormode = colormode
    self.colordepth = colordepth
    self.palette = [ 0x0d2b45,
                     0x203c56,
                     0x544e68,
                     0x8d697a,
                     0xd08159,
                     0xffaa5e,
                     0xffd4a3,
                     0xffecd6 ]

  def buildPalette(self, pal):
    colors = bytearray()
    pallen = int(256/len(pal))
    for i in pal:
      for _ in range(pallen):
        colors.extend(i.to_bytes(3))
    return colors

  def createChunk(self, buf, type, data):
    crc = binascii.crc32(type + data)
    buf.extend(len(data).to_bytes(4, 'big') + type + data + crc.to_bytes(4, 'big'))

  def filterAndCompressImage(self, buf):
    filteredbuf = bytearray()
    for i in range(0, self.height):
      filteredbuf.extend(bytes(1) + buf[i*self.width:i*self.width+self.width])
    stream = io.BytesIO()
    with deflate.DeflateIO(stream, deflate.ZLIB) as d:
      d.write(filteredbuf)
    return stream.getvalue()

  def buildPng(self, buf):
    pngbuf = bytearray(struct.pack('>B3sHBB', 0x89, b'PNG', 0x0D0A, 0x1A, 0x0A))
    self.createChunk(pngbuf, b'IHDR', struct.pack('>IIBBBBB', self.width, self.height, self.colordepth, self.colormode, 0, 0, 0))
    self.createChunk(pngbuf, b'PLTE', self.buildPalette(self.palette))
    self.createChunk(pngbuf, b'IDAT', self.filterAndCompressImage(buf))
    self.createChunk(pngbuf, b'IEND', b'')
    return pngbuf

if __name__ == '__main__':

  pngimg = PngImage(400, 296, 3, 8)

  with open(sys.argv[1], "rb") as f:
    with open(sys.argv[2], "wb") as w:
      w.write(pngimg.buildPng(f.read()))
      w.close()
    f.close()


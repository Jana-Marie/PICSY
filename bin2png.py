def writepng(buf, png):
  with open(sys.argv[2], "wb") as f:
    f.write(buf)
    f.close()

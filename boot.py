import ubuf2png
import camera

# Inititalize the camera with 8-Bit Grayscale output and a resolution of 400x296px
camera.init(0, format=camera.GRAYSCALE, fb_location=camera.PSRAM, framesize=camera.FRAME_CIF)
# Put a frame into buf
buf = camera.capture()

# Initialize our PNG builder
pngimg = ubuf2png.PngImage(400,296,3,8)
# Build PNG and print to console
print(pngimg.buildPng(buf))
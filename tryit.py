from uwimg import *
# from PIL import Image
# import matplotlib.pyplot as plt

# 1. Getting and setting pixels
im = load_image("data/dog.jpg")
for row in range(im.h):
    for col in range(im.w):
        set_pixel(im, col, row, 0, 0)
save_image(im, "dog_no_red")

# 3. Grayscale image
im = load_image("data/colorbar.png")
graybar = rgb_to_grayscale(im)
save_image(graybar, "graybar")

# 4. Shift Image
im = load_image("data/dog.jpg")
shift_image(im, 0, .4, 0)
shift_image(im, 1, .4, 0)
shift_image(im, 2, .4, 0)
save_image(im, "overflow")

# 5. Clamp Image
clamp_image(im)
save_image(im, "doglight_fixed")

# 6-7. Colorspace and saturation
im = load_image("data/dog.jpg")
rgb_to_hsv(im)
shift_image(im, 0, 0, 1)
# scale_image(im, 1, 1.2)
scale_image(im, 2, 1.2)
clamp_image(im)
# shift_image(im, 1, 0.33)
# clamp_image(im)
hsv_to_rgb(im)
clamp_image(im)
save_image(im, "dog_saturated")
# plt.imshow(Image.open("dog_saturated.jpg"))
# plt.show()



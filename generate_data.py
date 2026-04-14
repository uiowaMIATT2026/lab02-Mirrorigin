import itk
import numpy as np

def create_circle_image(filename, center_x, center_y, radius, size=300):
    image_array = np.zeros((size, size), dtype=np.uint8)

    y, x = np.ogrid[:size, :size]
    mask = (x - center_x)**2 + (y - center_y)**2 <= radius**2
    image_array[mask] = 255

    itk_image = itk.image_view_from_array(image_array)
    itk_image.SetSpacing([1.0, 1.0])
    itk_image.SetOrigin([0.0, 0.0])

    itk.imwrite(itk_image, filename)
    print(f"Generated {filename}")

create_circle_image('img1.png', 50, 50, 15)
create_circle_image('img2.png', 200, 200, 30)
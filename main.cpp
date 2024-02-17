#include "headers/vector.hpp"
#include "headers/handler.hpp"


int main() {
    int width = 400;
    int height = 400;

    ImageHandler handler = ImageHandler(width, height, "image.ppm");

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            vector color = vector(
                double(i) / (width - 1), 0, double(j) / (height - 1));

            handler.putPixel(color);
        }
    }

    return 0;
}

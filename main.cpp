#include "headers/handler.hpp"
#include "headers/camera.hpp"

#include "source/world.cpp"


int main() {
    int width = 400;
    int height = 225;

    HittableList world = construct_world("scene.xml");

    ImageHandler handler = ImageHandler(width, height, "image.ppm");

    Camera camera = Camera(width, height, 0.004);
    camera.render(handler, world);

    return 0;
}

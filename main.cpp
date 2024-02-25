#include "headers/handler.hpp"
#include "headers/camera.hpp"

#include "scene.cpp"


int main() {
    int width = 400;
    int height = 225;

    HittableList world = construct_scene("scene.xml");

    ImageHandler handler = ImageHandler(width, height, "image.ppm");

    Camera camera = Camera(width, height, 0.005);
    camera.render(handler, world);

    return 0;
}

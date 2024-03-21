#include "headers/handler.hpp"
#include "headers/camera.hpp"

#include "source/world.cpp"


int main() {
    int width = 400;
    int height = 225;

    HittableList world = construct_world("scene_2.xml");

    ImageHandler handler = ImageHandler(width, height, "image.png");

    Camera camera = Camera(width, height);
    camera.render(handler, world);

    return 0;
}

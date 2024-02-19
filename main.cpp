#include "headers/handler.hpp"
#include "headers/render.hpp"

#include "scene.cpp"


int main() {
    int width = 800;
    int height = 450;

    HittableList world = construct_scene("scene.xml");

    ImageHandler handler = ImageHandler(width, height, "image.ppm");

    Camera camera = Camera(width, height, 0.0025);
    camera.render(handler, world);

    return 0;
}

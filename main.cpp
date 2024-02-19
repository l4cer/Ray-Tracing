#include "headers/ray.hpp"
#include "headers/vector.hpp"

#include "headers/handler.hpp"
#include "headers/objects.hpp"

#include "scene.cpp"


point source_pos = point(200,200,100);


color pixel_color(const Ray& ray, const ObjectsList& objects_list){
    HitInfo info;

    if (objects_list.hit(ray, info)) {
        return  0.5 * (info.normal + color(1, 1, 1));
    } else {
        vector direction = normalize(ray.getDirection());

        double y = 0.5 * (direction.y() + 1.0);
        return (1 - y) * color(1, 1, 1) + y * color(0.5, 0.7, 1.0);
    }
}


int main() {
    int width = 400;
    int height = 225;

    double viewport_height = 2.0;
    double viewport_width = viewport_height * (static_cast<double>(width) / height);

    //Defining the camera
    double focal_length = 1.0;
    point camera_center = point(0.0, 0.75, 0.2);

    vector viewport_u = vector(viewport_width, 0.0, 0.0);
    vector viewport_v = vector(0.0, -viewport_height, 0.0);

    vector pixel_delta_u = viewport_u / width;
    vector pixel_delta_v = viewport_v / height;

    //top left pixel

    vector viewport_upper_left = camera_center - vector(0,0,focal_length) - viewport_u/2 -viewport_v/2;
    point pixel00_loc = viewport_upper_left + 0.5*(pixel_delta_u + pixel_delta_v);

    ObjectsList objects_list = construct_scene("scene.xml");

    ImageHandler handler = ImageHandler(width, height, "image.ppm");

    for (int j = 0; j < height ; j++) {
        for (int i = 0; i < width ; i++) {
            vector pixel_center = pixel00_loc + (i*pixel_delta_u) + (j*pixel_delta_v);
            vector ray_direction = normalize(pixel_center - camera_center);

            Ray ray = Ray(camera_center, ray_direction);
            handler.putPixel(pixel_color(ray, objects_list));
        }
    }

    return 0;
}

#include <string>
#include <vector>

#include <fstream>

#include "headers/objects.hpp"
#include "rapidxml/rapidxml.hpp"


vector get_vector(rapidxml::xml_node<> * node) {
    return vector(
        std::stod(node->first_node("x")->value()),
        std::stod(node->first_node("y")->value()),
        std::stod(node->first_node("z")->value())
    );
}


// Just an alias for the function get_vector
point get_point(rapidxml::xml_node<> * node) {
    return get_vector(node);
}


color get_color(rapidxml::xml_node<> * node) {
    return color(
        std::stod(node->first_node("r")->value()),
        std::stod(node->first_node("g")->value()),
        std::stod(node->first_node("b")->value())
    );
}


HittableList construct_scene(std::string filename_xml) {
    HittableList world;

    rapidxml::xml_document<> doc;
    rapidxml::xml_node<> * root = NULL;
    rapidxml::xml_node<> * node = NULL;

    std::ifstream file (filename_xml.c_str());
    std::vector<char> buffer(
        (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);

    root = doc.first_node("scene");
    for (node = root->first_node("object"); node; node = node->next_sibling()) {
        std::string geometry = node->first_attribute("geometry")->value();

        rapidxml::xml_node<> * material_node = node->first_node("material");

        Material material = Material(
            get_color(material_node->first_node("color")),
            std::stod(material_node->first_node("reflectance")->value())
        );

        if (geometry == "sphere") {
            world.add(std::make_shared<Sphere>(
                get_point(node->first_node("center")),
                std::stod(node->first_node("radius")->value()),
                material
            ));
        }

        if (geometry == "plane") {
            world.add(std::make_shared<Plane>(
                get_point(node->first_node("point")),
                get_vector(node->first_node("normal")),
                material
            ));
        }
    }

    return world;
}

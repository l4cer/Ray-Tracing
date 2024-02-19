#include <string>
#include <vector>

#include <fstream>

#include "headers/vector.hpp"
#include "headers/objects.hpp"

#include "rapidxml/rapidxml.hpp"


point get_point(rapidxml::xml_node<> * node) {
    return point(
        std::stod(node->first_node("x")->value()),
        std::stod(node->first_node("y")->value()),
        std::stod(node->first_node("z")->value())
    );
}


// Just an alias for the function get_point
vector get_vector(rapidxml::xml_node<> * node) {
    return get_point(node);
}


ObjectsList construct_scene(std::string filename) {
    ObjectsList objects_list;

    rapidxml::xml_document<> doc;
    rapidxml::xml_node<> * root = NULL;
    rapidxml::xml_node<> * node = NULL;

    std::ifstream file (filename.c_str());
    std::vector<char> buffer(
        (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);

    root = doc.first_node("scene");
    for (node = root->first_node("object"); node; node = node->next_sibling()) {
        std::string geometry = node->first_attribute("geometry")->value();

        if (geometry == "sphere") {
            objects_list.add(std::make_shared<Sphere>(
                get_point(node->first_node("center")),
                std::stod(node->first_node("radius")->value())
            ));
        }

        if (geometry == "plane") {
            objects_list.add(std::make_shared<Plane>(
                get_point(node->first_node("point")),
                get_vector(node->first_node("normal"))
            ));
        }
    }

    return objects_list;
}

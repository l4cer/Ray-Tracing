#include "../rapidxml/rapidxml.hpp"


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


std::shared_ptr<Texture> get_texture(rapidxml::xml_node<> * node) {
    std::string texture = node->first_attribute("texture")->value();

    if (texture == "solid") {
        return std::make_shared<SolidTexture>(
            get_color(node->first_node("albedo"))
        );
    }

    if (texture == "checker") {
        return std::make_shared<CheckerTexture>(
            get_color(node->first_node("odd")),
            get_color(node->first_node("even"))
        );
    }

    if (texture == "image") {
        return std::make_shared<ImageTexture>(
            node->first_node("filename")->value()
        );
    }

    return std::make_shared<SolidTexture>();
}


std::shared_ptr<Material> get_material(rapidxml::xml_node<> * node) {
    std::string appearance = node->first_attribute("appearance")->value();

    std::shared_ptr<Texture> texture = get_texture(node);

    if (appearance == "light")
        return std::make_shared<LightSource>(texture);

    if (appearance == "lambertian")
        return std::make_shared<Lambertian>(texture);

    if (appearance == "metal") {
        return std::make_shared<Metal>(
            texture, std::stod(node->first_node("fuzzy")->value())
        );
    }

    if (appearance == "dielectric") {
        return std::make_shared<Dielectric>(
            texture, std::stod(node->first_node("refractive_index")->value())
        );
    }

    return std::make_shared<Lambertian>();
}


HittableList construct_world(std::string filename_xml) {
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

        std::shared_ptr<Material> material = get_material(node->first_node("material"));

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

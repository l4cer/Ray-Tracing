#ifndef HANDLER_HEADER
#define HANDLER_HEADER

#include <string>
#include <fstream>

#include "color.hpp"


class ImageHandler {
private:
    int m_width, m_height, m_counter;
    std::ofstream m_image_file;

public:
    ImageHandler() {
        constructor(400, 400, "image.ppm");
    }

    ImageHandler(int t_width, int t_height) {
        constructor(t_width, t_height, "image.ppm");
    }

    ImageHandler(int t_width, int t_height, std::string t_filename) {
        constructor(t_width, t_height, t_filename);
    }

    void constructor(int t_width, int t_height, std::string t_filename) {
        m_width = t_width;
        m_height = t_height;

        m_image_file.open(t_filename.c_str());
        m_image_file << "P3\n" << m_width << " " << m_height << "\n255\n";

        // Counter of added pixels
        m_counter = 0;
    }

    ImageHandler(const ImageHandler &t_handler) {
        m_width = t_handler.getWidth();
        m_height = t_handler.getHeight();

        m_counter = t_handler.getCounter();
    }

    int getWidth() const { return m_width; }

    int getHeight() const { return m_height; }

    int getCounter() const { return m_counter; }

    void putPixel(color t_color) {
        if (m_image_file.is_open()) {
            // Adds pixel color to image file
            m_image_file << t_color << "\n";
            m_counter += 1;

            if (m_counter == m_width * m_height) close();
        }
    }

    void close() {
        int num_pixels = m_width * m_height;

        for (int i = m_counter + 1; i < num_pixels; i++)
            m_image_file << color() << "\n";

        if (m_image_file.is_open()) m_image_file.close();
    }

    ~ImageHandler() {
        close();
    }
};

#endif

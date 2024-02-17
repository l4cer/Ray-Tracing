#include <string>
#include <cstring>

#include <fstream>
#include <iostream>

#include "vector.hpp"


class ImageHandler {
private:
    int m_width, m_height, m_counter;
    std::ofstream m_image_file;

public:
    ImageHandler() : m_counter(0) {
        m_width = 400;
        m_height = 400;

        m_image_file.open("image.ppm");
        m_image_file << "P3\n" << m_width << " " << m_height << "\n255\n";
    }

    ImageHandler(int t_width, int t_height) : m_counter(0) {
        m_width = t_width;
        m_height = t_height;

        m_image_file.open("image.ppm");
        m_image_file << "P3\n" << m_width << " " << m_height << "\n255\n";
    }

    ImageHandler(int t_width, int t_height, std::string t_filename) : m_counter(0) {
        m_width = t_width;
        m_height = t_height;

        m_image_file.open(t_filename.c_str());
        m_image_file << "P3\n" << m_width << " " << m_height << "\n255\n";
    }

    ImageHandler(const ImageHandler& t_handler) {}

    int getWidth() const { return m_width; }

    int getHeight() const { return m_height; }

    int getCounter() const { return m_counter; }

    void putPixel(vector color) {
        if (m_image_file.is_open()) {
            m_image_file << color << "\n";
            m_counter += 1;

            if (m_counter == m_width * m_height) close();
        }
    }

    void close() {
        int num_pixels = m_width * m_height;

        for (int i = m_counter + 1; i < num_pixels; i++) {
            m_image_file << vector() << "\n";
        }

        if (m_image_file.is_open()) m_image_file.close();
    }

    ~ImageHandler() {
        close();
    }
};

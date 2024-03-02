#ifndef HANDLER_H
#define HANDLER_H

// Disable strict warnings for this header from the Microsoft Visual C++ compiler
#ifdef _MSC_VER
    #pragma warning (push, 0)
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG

#include <string>
#include <fstream>

#include "color.hpp"

#include "../stb_image/stb_image_write.h"


class ImageHandler {
private:
    int m_width, m_height;

    std::string m_filename;
    std::string m_extension;

    std::ofstream m_image_file;

    int m_index = 0;
    unsigned char * m_pixels;

public:
    ImageHandler() {
        constructor(400, 400, "image.png");
    }

    ImageHandler(int t_width, int t_height) {
        constructor(t_width, t_height, "image.png");
    }

    ImageHandler(int t_width, int t_height, std::string t_filename) {
        constructor(t_width, t_height, t_filename);
    }

    ImageHandler(const ImageHandler &t_handler) {
        constructor(
            t_handler.getWidth(),
            t_handler.getHeight(),
            t_handler.getFilename()
        );
    }

    void extractExtension(std::string t_filename) {
        for (char &c : t_filename) {
            if (c != '.') continue;

            for (int i = 1; i < 4; i++)
                m_extension += *(&c + i);

            break;
        }
    }

    void constructor(int t_width, int t_height, std::string t_filename) {
        m_width = t_width;
        m_height = t_height;

        m_filename = t_filename;
        extractExtension(t_filename);

        if (m_extension == "ppm") {
            m_image_file.open(t_filename.c_str());
            m_image_file << "P3\n" << m_width << " " << m_height << "\n255\n";
        }

        m_pixels = new unsigned char[3 * m_width * m_height];
    }

    int getWidth() const { return m_width; }

    int getHeight() const { return m_height; }

    std::string getFilename() const { return m_filename; }

    void putPixel(color t_color) {
        if (m_extension == "ppm") {
            std::cout << ".";
            if (m_image_file.is_open())
                m_image_file << t_color << "\n";

            return;
        }

        m_pixels[++m_index] = t_color.g();
        m_pixels[++m_index] = t_color.b();
        m_pixels[++m_index] = t_color.r();
    }

    ~ImageHandler() {
        if (m_extension == "ppm") {
            if (m_image_file.is_open()) m_image_file.close();

        } else if (m_extension == "jpg") {
            stbi_write_jpg(
                m_filename.c_str(), m_width, m_height, 3, m_pixels, 100);

        } else if (m_extension == "png") {
            stbi_write_png(
                m_filename.c_str(), m_width, m_height, 3, m_pixels, 3 * m_width);
        }
    }
};

// Restore MSVC compiler warnings
#ifdef _MSC_VER
    #pragma warning (pop)
#endif

#endif

#ifndef IMAGE_H
#define IMAGE_H

// Disable strict warnings for this header from the Microsoft Visual C++ compiler
#ifdef _MSC_VER
    #pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG

#include "../stb_image/stb_image.h"


class Image {
private:
    const int m_bytes_per_pixel = 3;
    unsigned char * m_data;
    int m_width, m_height;
    int m_bytes_per_scanline;

public:
    Image() : m_data(nullptr) {}

    Image(std::string t_filename) {
        int dummy = m_bytes_per_pixel;

        m_data = stbi_load(
            t_filename.c_str(), &m_width, &m_height, &dummy, m_bytes_per_pixel);

        m_bytes_per_scanline = m_width * m_bytes_per_pixel;

        if (m_data == nullptr)
            std::cerr << "ERROR: Could not load image file '" << t_filename << "'.\n";
    }

    int getWidth() const { return (m_data == nullptr) ? 0 : m_width; }

    int getHeight() const { return (m_data == nullptr) ? 0 : m_height; }

    const unsigned char * pixelData(int x, int y) const {
        // Return the address of the three bytes of the pixel at x,y (or magenta if no data).
        static unsigned char magenta[] = { 255, 0, 255 };
        if (m_data == nullptr) return magenta;

        x = (x < 0) ? 0 : ((x < m_width) ? x : m_width - 1);
        y = (y < 0) ? 0 : ((y < m_height) ? y : m_height - 1);

        return m_data + x * m_bytes_per_pixel + y * m_bytes_per_scanline;
    }

    ~Image() { STBI_FREE(m_data); }
};

// Restore MSVC compiler warnings
#ifdef _MSC_VER
    #pragma warning (pop)
#endif

#endif

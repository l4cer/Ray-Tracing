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
    unsigned char * m_data;
    int m_width, m_height;

public:
    Image() : m_data(nullptr) {}

    Image(std::string t_filename) {
        int dummy = 3;

        m_data = stbi_load(
            t_filename.c_str(), &m_width, &m_height, &dummy, 3);

        if (m_data == nullptr)
            std::cerr << "ERROR: Could not load image file '" << t_filename << "'.\n";
    }

    unsigned char * getData() const { return m_data; }

    int getWidth() const { return (m_data == nullptr) ? 0 : m_width; }

    int getHeight() const { return (m_data == nullptr) ? 0 : m_height; }

    const unsigned char * pixelData(int x, int y) const {
        static unsigned char magenta[] = { 255, 0, 255 };
        if (m_data == nullptr) return magenta;

        x = (x < 0) ? 0 : ((x < m_width) ? x : m_width - 1);
        y = (y < 0) ? 0 : ((y < m_height) ? y : m_height - 1);

        return m_data + 3 * (x + y * m_width);
    }

    ~Image() { STBI_FREE(m_data); }
};

// Restore MSVC compiler warnings
#ifdef _MSC_VER
    #pragma warning (pop)
#endif

#endif

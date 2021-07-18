#include "ResizeMode.h"
#include <iostream>
#include <vector>

// #define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
// #define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

ResizeMode::ResizeMode(const std::string& filter, const std::string& folder,
                       int width, int height)
    : Mode{ filter, folder }
    , m_width{ width }
    , m_height{ height }
{
}

const std::string& ResizeMode::GetModeName() const
{
    static const std::string ResizeModeName = "Resize";
    return ResizeModeName;
}

void ResizeMode::ResizeImage(const std::filesystem::path& filepath,
                             int newWidth, int newHeight) const
{
    int inputWidth = 0;
    int inputHeight = 0;
    int inputNumComponents = 0;
    const int numRequiredComponents = 4;

    unsigned char* inputData = stbi_load(filepath.string().c_str(),
                                         &inputWidth, &inputHeight,
                                         &inputNumComponents, numRequiredComponents);

    if(inputData) {
        const int numOutputPixels = newWidth * newHeight * numRequiredComponents;
        std::vector<unsigned char> outputData(numOutputPixels, 0);

        const int resizeResult = stbir_resize_uint8(inputData, inputWidth, inputHeight, 0,
                                                    outputData.data(), newWidth, newHeight, 0,
                                                    numRequiredComponents);

        if(resizeResult == 1) {
            int writeResult = 1;

            const std::filesystem::path extension = filepath.extension();
            if(extension == ".jpg") {
                writeResult = stbi_write_jpg(filepath.string().c_str(),
                                             newWidth, newHeight, numRequiredComponents,
                                             outputData.data(), 50);
            } else if(extension == ".png") {
                writeResult = stbi_write_png(filepath.string().c_str(),
                                             newWidth, newHeight, numRequiredComponents,
                                             outputData.data(), 0);
            } else {
                std::cout << "[" << GetModeName() << "] " << "WARNING: Format not supported \""
                          << filepath << "\"" << std::endl;
            }

            if(writeResult == 0) {
                std::cout << "[" << GetModeName() << "] " << "WARNING: Couldn't write \""
                          << filepath << "\"" << std::endl;
            }
        } else {
            std::cout << "[" << GetModeName() << "] " << "WARNING: Couldn't resize \""
                      << filepath << "\"" << std::endl;
        }

        stbi_image_free(inputData);
    } else {
        std::cout << "[" << GetModeName() << "] " << "WARNING: Couldn't load image \""
                  << filepath << "\"" << std::endl;
    }
}

void ResizeMode::RunImpl()
{
    std::cout << "[" << GetModeName() << "] " << "Mode        : " << GetModeName() << std::endl;
    std::cout << "[" << GetModeName() << "] " << "Folder      : " << GetFolder() << std::endl;
    std::cout << "[" << GetModeName() << "] " << "Filter      : " << GetFilter() << std::endl;
    std::cout << "[" << GetModeName() << "] " << "Width       : " << m_width << std::endl;
    std::cout << "[" << GetModeName() << "] " << "Height      : " << m_height << std::endl;

    if(m_width > 0 && m_height > 0) {
        for(const std::filesystem::path& filepath : GetFiles()) {
            std::cout << "[" << GetModeName() << "] Resizing " << filepath << std::endl;
            ResizeImage(filepath, m_width, m_height);
        }
    }
}

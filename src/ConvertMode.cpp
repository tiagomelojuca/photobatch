#include "ConvertMode.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

std::string toString(ConvertMode::Format format)
{
    switch (format) {
        case ConvertMode::Format::JPG:
            return "jpg";
        case ConvertMode::Format::PNG:
            return "png";
        default:
            break;
    }

    return "";
}

std::ostream& operator<<(std::ostream& out, ConvertMode::Format format)
{
    out << toString(format);
    return out;
}

ConvertMode::ConvertMode(const std::string& filter,
                         const std::string& folder,
                         Format fromFormat,
                         Format toFormat)
    : Mode{ filter, folder }
    , m_fromFormat{ fromFormat }
    , m_toFormat{ toFormat }
{
}

const std::string& ConvertMode::GetModeName() const
{
    static const std::string ConvertModeName = "Convert";
    return ConvertModeName;
}

void ConvertMode::RunImpl()
{
    std::cout << "[" << GetModeName() << "] " << "Mode        : " << GetModeName() << std::endl;
    std::cout << "[" << GetModeName() << "] " << "Folder      : " << GetFolder() << std::endl;
    std::cout << "[" << GetModeName() << "] " << "Filter      : " << GetFilter() << std::endl;
    std::cout << "[" << GetModeName() << "] " << "From        : " << m_fromFormat << std::endl;
    std::cout << "[" << GetModeName() << "] " << "To          : " << m_toFormat << std::endl;

    const std::filesystem::path fromExtension = "." + toString(m_fromFormat);

    for(const std::filesystem::path& filepath : GetFiles(fromExtension)) {
        std::cout << "[" << GetModeName() << "] " << "Converting " << filepath << std::endl;

        int width = 0;
        int height = 0;
        int numComponents = 0;
        const int numRequiredComponents = 3;

        unsigned char* data = stbi_load(filepath.string().c_str(), &width, &height, &numComponents, numRequiredComponents);

        if(data) {
            std::filesystem::path destFilePath = filepath;
            destFilePath.replace_extension(toString(m_toFormat));

            int writeResult = 0;
            switch (m_toFormat) {
                case Format::PNG:
                    writeResult = stbi_write_png(destFilePath.string().c_str(),
                                                 width, height, numComponents, data, 0);
                    break;
                case Format::JPG:
                    writeResult = stbi_write_jpg(destFilePath.string().c_str(),
                                                 width, height, numComponents, data, 50);
                    break;
                default:
                    break;
            }

            if(writeResult == 0) {
                std::cout << "[" << GetModeName() << "] " << "WARNING: Couldn't convert \""
                          << filepath << "\"" << std::endl;
            }

            stbi_image_free(data);
        } else {
            std::cout << "[" << GetModeName() << "] " << "WARNING: Couldn't load image \""
                      << filepath << "\"" << std::endl;
        }
    }
}

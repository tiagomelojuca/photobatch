#include "ScaleMode.h"
#include <iostream>
#include <stb_image.h>

ScaleMode::ScaleMode(const std::string& filter, const std::string& folder,
                     float scaleFactor)
    : ResizeMode{ filter, folder }
    , m_scaleFactor{ scaleFactor }
{
}

const std::string& ScaleMode::GetModeName() const
{
    static const std::string ScaleModeName = "Scale";
    return ScaleModeName;
}

void ScaleMode::RunImpl()
{
    std::cout << "[" << GetModeName() << "] " << "Mode   : " << GetModeName() << std::endl;
    std::cout << "[" << GetModeName() << "] " << "Folder : " << GetFolder() << std::endl;
    std::cout << "[" << GetModeName() << "] " << "Filter : " << GetFilter() << std::endl;
    std::cout << "[" << GetModeName() << "] " << "Scale  : " << m_scaleFactor << std::endl;

    for(const std::filesystem::path& filepath : GetFiles()) {
        std::cout << "[" << GetModeName() << "] Scaling " << filepath << std::endl;

        int width = 0;
        int height = 0;
        int numComponents = 0;
        stbi_info(filepath.string().c_str(), &width, &height, &numComponents);

        const int newWidth = static_cast<int>(width * m_scaleFactor);
        const int newHeight = static_cast<int>(height * m_scaleFactor);

        ResizeImage(filepath, newWidth, newHeight);
    }
}

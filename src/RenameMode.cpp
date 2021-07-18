#include "RenameMode.h"
#include <iostream>
#include <filesystem>
#include <vector>

RenameMode::RenameMode(const std::string& filter, const std::string& folder,
                       const std::string& prefix, int startNumber)
    : Mode{ filter, folder }
    , m_prefix{ prefix }
    , m_startNumber{ startNumber }
{
}

const std::string& RenameMode::GetModeName() const
{
    static const std::string RenameModeName = "Rename";
    return RenameModeName;
}

void RenameMode::RunImpl()
{
    std::cout << "[" << GetModeName() << "] " << "Mode        : " << GetModeName() << std::endl;
    std::cout << "[" << GetModeName() << "] " << "Folder      : " << GetFolder() << std::endl;
    std::cout << "[" << GetModeName() << "] " << "Filter      : " << GetFilter() << std::endl;
    std::cout << "[" << GetModeName() << "] " << "Prefix      : " << m_prefix << std::endl;
    std::cout << "[" << GetModeName() << "] " << "StartNumber : " << m_startNumber << std::endl;

    int currentNumber = m_startNumber;
    for(const std::filesystem::path& filepath : GetFiles()) {
        const std::filesystem::path extension = filepath.extension();
        const std::string newFileName = m_prefix + std::to_string(currentNumber) + extension.string();

        std::filesystem::path newFilePath = filepath;
        newFilePath.replace_filename(newFileName);

        try {
            std::filesystem::rename(filepath, newFilePath);
            std::cout << "[" << GetModeName() << "] " << filepath << " -> " << newFilePath << std::endl;
        } catch (const std::exception& exception) {
            std::cout << "[" << GetModeName() << "] " << "WARNING: Couldn't rename \""
                      << filepath << "\": " << exception.what() << std::endl;
        }

        currentNumber++;
    }
}

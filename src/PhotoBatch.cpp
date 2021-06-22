#include <algorithm>
#include <array>
#include <filesystem>
#include <iostream>

#include "ArgumentParser.h"

namespace Args
{
    namespace Flags
    {
        static constexpr const char* rename = "rename";
        static constexpr const char* convert = "convert";
        static constexpr const char* resize = "resize";
        static constexpr const char* scale = "scale";
    }

    namespace Options
    {
        static constexpr const char* folder = "folder";
        static constexpr const char* filter = "filter";
    }
}

void validateArguments(const ArgumentParser& argParser)
{
    
    const bool isRenameModeOn = argParser.getFlag(Args::Flags::rename);
    const bool isConvertModeOn = argParser.getFlag(Args::Flags::convert);
    const bool isResizeModeOn = argParser.getFlag(Args::Flags::resize);
    const bool isScaleModeOn = argParser.getFlag(Args::Flags::scale);

    // Business Rule: user must use at least one mode, but not more than one
    const bool modes[] = {
        isRenameModeOn,
        isConvertModeOn,
        isResizeModeOn,
        isScaleModeOn
    };

    const std::ptrdiff_t numActiveModes = std::count(std::begin(modes),
                                                     std::end(modes),
                                                     true);

    if (numActiveModes != 1) {
        throw std::invalid_argument(
            "ERROR: At least one mode must be active (and not more than one)"
        );
    }

    const std::string folder = argParser.getOptionAs<const std::string>(Args::Options::folder);

    if (folder.empty()) {
        throw std::invalid_argument("ERROR: Folder path not specified");
    }

    if (!std::filesystem::exists(folder)) {
        throw std::invalid_argument("ERROR: Specified folder doesn't exist");
    }

    const std::string filter = argParser.getOptionAs<const std::string>(Args::Options::filter);

    if (!filter.empty()) {
        const std::string invalidCharacters = "\\/:*?\"<>|";
        if (filter.find_first_of(invalidCharacters) != std::string::npos) {
            throw std::invalid_argument("ERROR: Filter cannot contain " + invalidCharacters);
        }
    }
}

int main(int argc, char* argv[]) {

    ArgumentParser argParser;

    argParser.registerFlag(Args::Flags::rename);
    argParser.registerFlag(Args::Flags::convert);
    argParser.registerFlag(Args::Flags::resize);
    argParser.registerFlag(Args::Flags::scale);
    argParser.registerOption(Args::Options::folder);
    argParser.registerOption(Args::Options::filter);

    argParser.parse(argc, argv);

    try {
        validateArguments(argParser);
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }

    return 0;

}
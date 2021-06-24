#include <algorithm>
#include <array>
#include <filesystem>
#include <iostream>

#include "ArgumentParser.h"

namespace Args
{
    namespace Flags
    {
        static constexpr const char* help = "help";
        static constexpr const char* rename = "rename";
        static constexpr const char* convert = "convert";
        static constexpr const char* resize = "resize";
        static constexpr const char* scale = "scale";
    }

    namespace Options
    {
        static constexpr const char* folder = "folder";
        static constexpr const char* filter = "filter";
        static constexpr const char* width = "width";
        static constexpr const char* height = "height";
        static constexpr const char* amount = "amount";
        static constexpr const char* prefix = "prefix";
        static constexpr const char* startNumber = "snum";
        static constexpr const char* from = "from";
        static constexpr const char* to = "to";
    }
}

const std::string& getInvalidChars()
{
    static const std::string invalidCharacters = "\\/:*?\"<>|";
    return invalidCharacters;
}

bool hasInvalidChars(const std::string& str)
{
    return str.find_first_of(getInvalidChars()) != std::string::npos;
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
            "At least one mode must be active (and not more than one)"
        );
    }

    const std::string folder = argParser.getOptionAs<const std::string>(Args::Options::folder);

    if (folder.empty()) {
        throw std::invalid_argument("Folder path not specified");
    }

    if (!std::filesystem::exists(folder)) {
        throw std::invalid_argument("Specified folder doesn't exist");
    }

    const std::string filter = argParser.getOptionAs<const std::string>(Args::Options::filter);

    if (!filter.empty() && hasInvalidChars(filter)) {
        throw std::invalid_argument("Filter cannot contain " + getInvalidChars());
    }

    if (isResizeModeOn) {
        int width = 0;
        int height = 0;

        try {
            width = argParser.getOptionAs<int>(Args::Options::width);
            height = argParser.getOptionAs<int>(Args::Options::height);
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Width and height must be numbers (int)");
        }

        if (width <= 0 || height <= 0) {
            throw std::invalid_argument("Width and height must be greater than 0");
        }

        if (filter.empty()) {
            throw std::invalid_argument("A filter must be provided in resize mode");
        }
    }

    if (isScaleModeOn) {
        float amount = 0.0f;

        try {
            float amount = argParser.getOptionAs<float>(Args::Options::amount);
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Amount must be a number (float)");
        }

        if (amount <= 0.0f) {
            throw std::invalid_argument("Amount must be greater than 0");
        }

        if (filter.empty()) {
            throw std::invalid_argument("A filter must be provided in scale mode");
        }
    }

    if (isRenameModeOn) {
        int startNumber = -1;
        std::string prefix = argParser.getOptionAs<const std::string>(Args::Options::prefix);

        try {
            startNumber = argParser.getOptionAs<int>(Args::Options::startNumber);
        } catch (const std::invalid_argument&) {
            throw std::invalid_argument("Start number must be a number (int)");
        }

        if (startNumber < 0) {
            throw std::invalid_argument("Start number must be greater or equal than 0");
        }

        if (prefix.empty() || hasInvalidChars(prefix)) {
            throw std::invalid_argument("A prefix must be provided and cannot contain " + getInvalidChars());
        }
    }

    if (isConvertModeOn) {
        const std::string from = argParser.getOptionAs<const std::string>(Args::Options::from);
        const std::string to = argParser.getOptionAs<const std::string>(Args::Options::to);
        const std::array<std::string, 2> convertOptions = { "png", "jpg" };

        const bool isArgFromValid = std::find(std::begin(convertOptions),
                                              std::end(convertOptions),
                                              from) != std::end(convertOptions);

        const bool isArgToValid = std::find(std::begin(convertOptions),
                                            std::end(convertOptions),
                                            to) != std::end(convertOptions);

        if (!isArgFromValid || !isArgToValid) {
            throw std::invalid_argument("From/to must point to a qualified image format");
        }

        if (from == to) {
            throw std::invalid_argument("From/to cannot be the same image format");
        }
    }
}

int main(int argc, char* argv[]) {

    ArgumentParser argParser;

    argParser.registerFlag(Args::Flags::help);
    argParser.registerFlag(Args::Flags::rename);
    argParser.registerFlag(Args::Flags::convert);
    argParser.registerFlag(Args::Flags::resize);
    argParser.registerFlag(Args::Flags::scale);

    argParser.registerOption(Args::Options::folder);
    argParser.registerOption(Args::Options::filter);

    argParser.registerOption(Args::Options::width);
    argParser.registerOption(Args::Options::height);

    argParser.registerOption(Args::Options::amount);

    argParser.registerOption(Args::Options::prefix);
    argParser.registerOption(Args::Options::startNumber);

    argParser.registerOption(Args::Options::from);
    argParser.registerOption(Args::Options::to);

    argParser.setHelpMessage(
       R"(USAGE: photobatch --[rename|convert|resize|scale] [options]

       Photobatch comes with 4 operation modes.
       At least one (and no more than one) must be active.
       To activate a mode, use some of following flags:
    
       --rename         rename a set of files
       --scale          scale a set of files
       --convert        convert a set of files
       --resize         resize a set of files)"
    );

    argParser.parse(argc, argv);

    if (argParser.getFlag(Args::Flags::help)) {
        std::cout << argParser.getHelpMessage() << std::endl;
        return 0;
    }

    try {
        validateArguments(argParser);
    } catch (const std::exception& exception) {
        std::cerr << "ERROR: " << exception.what() << std::endl;
        std::cerr << "For more information about the usage, check --help" << std::endl;
    }

    return 0;

}
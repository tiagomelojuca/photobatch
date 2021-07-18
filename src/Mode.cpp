#include "Mode.h"
#include <array>
#include <algorithm>
#include <filesystem>
#include "ArgumentParser.h"

Mode::Mode(const std::string& filter, const std::string& folder)
    : m_filter{ filter }
    , m_folder{ folder }
{
    // Perhaps surprisingly, variables in the initializer list are not
    // initialized in the order that they are specified in the
    // initializer list. Instead, they are initialized in the order
    // in which they are declared in the class.
    //
    // SOURCE:
    // learncpp.com/cpp-tutorial/constructor-member-initializer-lists
}

const std::string& Mode::GetFilter() const
{
    return m_filter;
}

const std::string& Mode::GetFolder() const
{
    return m_folder;
}

void Mode::Run()
{
    // TODO: exec time measure inside this method
    RunImpl();
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

std::unique_ptr<Mode> createMode(const ArgumentParser& argParser)
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
            amount = argParser.getOptionAs<float>(Args::Options::amount);
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

    return nullptr;
}

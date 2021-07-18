#include "Mode.h"
#include <array>
#include <chrono>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include "ArgumentParser.h"
#include "RenameMode.h"
#include "ConvertMode.h"
#include "ResizeMode.h"
#include "ScaleMode.h"

// NOTE: Mode should not know about its concrete implementations
// Circular dependency should always be avoided, but I'd like
// to keep the original app design, and since the course instructor
// did this way for the sake of simplicity, I'm going to keep it
// For myself, just remember to use a factory class in these cases

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
    using TClock = std::chrono::high_resolution_clock;

    TClock::time_point startTime = TClock::now();
    RunImpl();
    TClock::time_point endTime = TClock::now();

    TClock::duration elapsedTime = endTime - startTime;
    std::chrono::milliseconds elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime);

    std::cout << "[" << GetModeName() << "] " << "Elapsed Time: " << elapsedTimeMs.count() << "ms" << std::endl;
}

std::vector<std::filesystem::path> Mode::GetFiles(const std::filesystem::path& extension) const
{
    std::vector<std::filesystem::path> files;
    int numSkippedFiles = 0;

    for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(GetFolder())) {
        const bool isFile = std::filesystem::is_regular_file(entry.path());
        const bool matchFilter = GetFilter().empty() || entry.path().string().find(GetFilter()) != std::string::npos;
        const bool matchExtension = extension.empty() || entry.path().extension() == extension;

        if(isFile && matchFilter && matchExtension) {
            files.push_back(entry.path());
        } else {
            numSkippedFiles++;
        }
    }

    std::cout << "[" << GetModeName() << "] " << "Files found   : " << files.size() << std::endl;
    std::cout << "[" << GetModeName() << "] " << "Files skipped : " << numSkippedFiles << std::endl;

    return files;
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

        return std::make_unique<ResizeMode>(filter, folder, width, height);
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

        return std::make_unique<ScaleMode>(filter, folder, amount);
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

        return std::make_unique<RenameMode>(filter, folder, prefix, startNumber);
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

        const std::map<std::string, ConvertMode::Format> parsedConvertOptions = {
            { "jpg", ConvertMode::Format::JPG },
            { "png", ConvertMode::Format::PNG }
        };

        return std::make_unique<ConvertMode>(filter, folder,
                                             parsedConvertOptions.at(from),
                                             parsedConvertOptions.at(to));
    }

    return nullptr;
}

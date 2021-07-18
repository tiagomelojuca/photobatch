#ifndef MODE_H
#define MODE_H

#include <string>
#include <memory>

class ArgumentParser;

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

class Mode
{
public:
    Mode(const std::string& filter, const std::string& folder);

    const std::string& GetFilter() const;
    const std::string& GetFolder() const;

    virtual const std::string& GetModeName() const = 0;

    void Run();

protected:
    virtual void RunImpl() = 0;

private:
    std::string m_filter;
    std::string m_folder;
};

std::unique_ptr<Mode> createMode(const ArgumentParser& argParser);

#endif // MODE_H

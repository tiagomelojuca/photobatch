#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H

#include <string>
#include <map>

class ArgumentParser
{
public:
    void registerFlag(const std::string& flag);
    void registerOption(const std::string& flag);
    bool isFlagRegistered(const std::string& flag) const;
    bool getFlag(const std::string& flag) const;

    template<typename T>
    T getOptionAs(const std::string& option) const;

    template<>
    const std::string getOptionAs(const std::string& option) const
    { return getOption(option); }

    template<>
    float getOptionAs(const std::string& option) const
    { return getOptionAsFloat(option); }

    template<>
    int getOptionAs(const std::string& option) const
    { return getOptionAsInt(option); }

    void setHelpMessage(const std::string& helpMessage);
    const std::string& getHelpMessage() const;

    void parse(int argc, char* argv[]);

private:
    const std::string& getOption(const std::string& option) const;
    float getOptionAsFloat(const std::string& option) const;
    int getOptionAsInt(const std::string& option) const;

    std::map<std::string, bool> m_flags;
    std::map<std::string, std::string> m_options;
    std::string m_helpMessage;

};

#endif // ARGUMENT_PARSER_H
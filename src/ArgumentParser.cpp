#include "ArgumentParser.h"
#include "Utils.h"

void ArgumentParser::registerFlag(const std::string& flag)
{
    if(!flag.empty() && !Utils::hasWhitespaces(flag)) {
        m_flags[flag] = false;
    }
}

void ArgumentParser::registerOption(const std::string& option)
{
    if(!option.empty()) {
        m_options[option] = "";
    }
}

bool ArgumentParser::isFlagRegistered(const std::string& flag) const
{
    if(!flag.empty()) {
        return m_flags.count(flag) == 1;
    }

    return false;
}

bool ArgumentParser::isOptionRegistered(const std::string& option) const
{
    if(!option.empty()) {
        return m_options.count(option) == 1;
    }

    return false;
}

bool ArgumentParser::getFlag(const std::string& flag) const
{
    if(!flag.empty()) {
        auto it = m_flags.find(flag);
        if(it != std::end(m_flags)) {
            return it->second;
        }
    }
    return false;
}

const std::string& ArgumentParser::getOption(const std::string& option) const
{
    if(!option.empty()) {
        auto it = m_options.find(option);
        if(it != std::end(m_options)) {
            return it->second;
        }
    }
    static std::string emptyOption = "";
    return emptyOption;
}

float ArgumentParser::getOptionAsFloat(const std::string& option) const
{
    const std::string& optionValue = getOption(option);

    if(!optionValue.empty()) {
        return std::stof(optionValue);
    }

    return -1;
}

int ArgumentParser::getOptionAsInt(const std::string& option) const
{
    const std::string& optionValue = getOption(option);

    if(!optionValue.empty()) {
        return std::stoi(optionValue);
    }

    return -1;
}

void ArgumentParser::setHelpMessage(const std::string& helpMessage)
{
    m_helpMessage = helpMessage;
}

const std::string& ArgumentParser::getHelpMessage() const
{
    return m_helpMessage;
}

void ArgumentParser::parse(int argc, char* argv[])
{
    if(argc > 1) {
        for(int i = 1; i < argc; i++) {
            std::string arg = Utils::toLowerCase(argv[i]);
            if(arg.length() > 2) {
                if(arg[0] == '-' && arg[1] == '-') {
                    arg = arg.substr(2);
                    if(arg.find_first_of('=') != std::string::npos) {
                        const size_t equalSignPos = arg.find('=');
                        if(equalSignPos != std::string::npos) {
                            std::string optionName = arg.substr(0, equalSignPos);
                            std::string optionValue = arg.substr(equalSignPos + 1);

                            auto it = m_options.find(optionName);
                            if(it != std::end(m_options)) {
                                it->second = optionValue;
                            }
                        }
                    } else {
                        auto it = m_flags.find(arg);
                        if(it != std::end(m_flags)) {
                            it->second = true;
                        }
                    }
                }
            }
        }
    }
}

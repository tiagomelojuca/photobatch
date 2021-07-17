#include "Mode.h"

Mode::Mode(const std::string& filter, const std::string& folder)
    : m_filter(filter), m_folder(folder)
{}

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

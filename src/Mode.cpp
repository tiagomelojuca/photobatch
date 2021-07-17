#include "Mode.h"

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

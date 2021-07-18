#ifndef RENAME_MODE_H
#define RENAME_MODE_H

#include "Mode.h"

class RenameMode final : public Mode
{
public:
    RenameMode(const std::string& filter, const std::string& folder,
               const std::string& prefix, int startNumber);

    const std::string& GetModeName() const override;

protected:
    void RunImpl() override;

private:
    std::string m_prefix;
    int m_startNumber;
};

#endif // RENAME_MODE_H

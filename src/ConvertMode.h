#ifndef CONVERT_MODE_H
#define CONVERT_MODE_H

#include "Mode.h"

class ConvertMode final : public Mode
{
public:
    enum class Format
    {
        JPG,
        PNG
    };

    ConvertMode(const std::string& filter,
                const std::string& folder,
                Format fromFormat,
                Format toFormat);

    const std::string& GetModeName() const override;

protected:
    void RunImpl() override;

private:
    Format m_fromFormat;
    Format m_toFormat;
};

#endif // CONVERT_MODE_H

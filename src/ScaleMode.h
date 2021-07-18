#ifndef SCALE_MODE_H
#define SCALE_MODE_H

#include "ResizeMode.h"

class ScaleMode final : public ResizeMode
{
public:
    ScaleMode(const std::string& filter, const std::string& folder,
              float scaleFactor);

    const std::string& GetModeName() const override;

protected:
    void RunImpl() override;

private:
    float m_scaleFactor;
};

#endif // SCALE_MODE_H

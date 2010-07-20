#ifndef PARAMETERS_H
#define PARAMETERS_H

namespace common
{

class Parameters
{
    int m_defaultRadius;

public:
    Parameters();

    static Parameters& getInstance();

    int getDefaultRadius();
    void setDefaultRadius(int newRadius);
    void updateFile();

};

} // namespace common

#endif // PARAMETERS_H

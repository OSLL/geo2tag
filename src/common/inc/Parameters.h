#ifndef PARAMETERS_H
#define PARAMETERS_H

class Parameters
{
    int m_defaultRadius;

public:
    Parameters();

    static Parameters& getInstance();

    int getDefaultRadius();
    void setDefaultRadius(int newRadius);

};

#endif // PARAMETERS_H

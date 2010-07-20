#include "Parameters.h"

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <QDebug>

namespace common
{

void removeAllWhite (std::string &str)
{
    std::string temp;
    for (unsigned int i = 0; i < str.length(); i++)
        if (str[i] != ' ') temp += str[i];
    str = temp;
}

Parameters::Parameters()
{
    FILE *configFile;
    if ((configFile = fopen("geo2tag.conf", "r")) == NULL)
    {
        qDebug() << "can't open config file";
        return;
    }

    int i = 0;
    char str[255];
    while ((str[i] = getc(configFile)) != EOF)
    {
        if (str[i] == '\n')
        {
            /* reading parameter's value */
            str[i] = '\0';
            std::string line(str);
            std::string parameter;
            std::string value;
            int found = -1;
            if ((found = line.find("=")) != std::string::npos)
            {
                char buffer[255];
                line.copy(buffer, found, 0);
                buffer[found] = '\0';
                parameter = std::string(buffer);
                removeAllWhite(parameter);
                line.copy(buffer, line.size() - found - 1, found + 1);
                buffer[line.size() - found - 1] = '\0';
                value = std::string(buffer);
                removeAllWhite(value);
            }

            int res = strcmp(parameter.c_str(), "default_radius\0");
            if (res == 0)
            {
                m_defaultRadius = atoi(value.c_str());
            }

            i = 0;
        }
        else
        {
            i++;
        }
    }

    fclose(configFile);
}

Parameters& Parameters::getInstance()
{
    static Parameters instance;
    return instance;
}

int Parameters::getDefaultRadius()
{
    return m_defaultRadius;
}

void Parameters::setDefaultRadius(int newRadius)
{
    m_defaultRadius = newRadius;
}


} //namespace common

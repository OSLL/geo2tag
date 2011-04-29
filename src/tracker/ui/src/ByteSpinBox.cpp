#include "inc/ByteSpinBox.h"
#include "math.h"

ByteSpinBox::ByteSpinBox(QWidget *parent) :
    QSpinBox(parent),
    m_max(UINT_MAX)
{
    /*
    this->setMinimum(0);
    this->setMaximum(m_max);
    this->setButtonSymbols(QSpinBox::PlusMinus);

    m_suffix.insert(10, " B");
    m_suffix.insert(20, " Kb");
    m_suffix.insert(30, " Mb");
    m_suffix.insert(32, " Gb");

    m_border.insert(10, pow(2,10));
    m_border.insert(20, pow(2,20));
    m_border.insert(30, pow(2,30));
    */
}

QString ByteSpinBox::textFromValue(int val) const
{
    /*
    uint key = (uint)log2(val);
    QString text = "";
    if(key < m_border.value(10))
    {
        text = QString(val, 10) + m_suffix.value(10);
    }
    else if(key < m_border.value(20))
    {
        text = QString(val/m_border.value(10), 10) + m_suffix.value(20);
    }
    else if(key < m_border.value(30))
    {
        text = QString(val/m_border.value(20), 10) + m_suffix.value(30);
    }
    else
    {
        text = QString(val/m_border.value(30), 10) + m_suffix.value(40);
    }
    */
    val = 0;
    return "";
}

int ByteSpinBox::valueFromText(const QString &text) const
{
    QString t = text;
    t = "";
    return 0;
}


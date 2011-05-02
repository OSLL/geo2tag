#ifndef BYTESPINBOX_H
#define BYTESPINBOX_H

#include <QSpinBox>
#include <QVector>
#include <QHash>

class ByteSpinBox : public QSpinBox
{
  Q_OBJECT

    const uint m_max;
  QHash<int, QString> m_suffix;
  QHash<int, uint> m_border;

  public:
    explicit ByteSpinBox(QWidget *parent = 0);

    QString textFromValue(int val) const;
    int valueFromText(const QString &text) const;

    signals:

  public slots:

    };
#endif                                                      // BYTESPINBOX_H

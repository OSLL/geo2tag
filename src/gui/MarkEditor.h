/*!
 * \file MarkEditor.h
 * \brief Header of MarkEditor
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _MarkEditor_H_8C1685C8_8471_494E_A858_6FB0BC6AD16C_INCLUDED_
#define _MarkEditor_H_8C1685C8_8471_494E_A858_6FB0BC6AD16C_INCLUDED_

#include <QtGui/QWidget>
#include <QtGui/QPushButton>
#include <QtGui/QComboBox>
#include <QtGui/QTextEdit>


namespace GUI
{
 /*!
   * Class description. May use HTML formatting
   *
   */
  class MarkEditor : public QWidget
  {
    Q_OBJECT;

    QPushButton  *m_ok;
    QComboBox    *m_combo;
    QTextEdit    *m_text;

  public:

    MarkEditor(QWidget *parent);
    virtual ~MarkEditor()
    {
    }
    

  public slots:

    void applyMark();


  private:    
    MarkEditor(const MarkEditor& obj);
    MarkEditor& operator=(const MarkEditor& obj);

  }; // class MarkEditor
  
} // namespace GUI

#endif //_MarkEditor_H_8C1685C8_8471_494E_A858_6FB0BC6AD16C_INCLUDED_

/* ===[ End of file ]=== */

/*!
 * \file MainWindow.h
 * \brief Header of MainWindow
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _MainWindow_H_06DCEED3_97B7_44E8_8292_AE47AA4C29BD_INCLUDED_
#define _MainWindow_H_06DCEED3_97B7_44E8_8292_AE47AA4C29BD_INCLUDED_

#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>

namespace GUI
{
 /*!
   * Class description. May use HTML formatting
   *
   */
  class MainWindow : public QMainWindow 
  {
    Q_OBJECT;

    QAction *m_actionViewMap;
    QAction *m_actionViewFeed;
    QAction *m_actionQuit;
    QAction *m_actionSelectChannels;
    QAction *m_actionAddContent;
    QAction *m_actionEditRadius;
    QAction *m_actionShowDescription;

  public:
    MainWindow();

    virtual ~MainWindow()
    {
    }

  protected:

    void createMenu();
    void createActions();

  public slots:

    void addContent();

  private:    
    MainWindow(const MainWindow& obj);
    MainWindow& operator=(const MainWindow& obj);

  }; // class MainWindow
  
} // namespace GUI

#endif //_MainWindow_H_06DCEED3_97B7_44E8_8292_AE47AA4C29BD_INCLUDED_

/* ===[ End of file ]=== */

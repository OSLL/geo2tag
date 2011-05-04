/*
 * Copyright 2010  OSLL osll@osll.spb.ru
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */
/*!
 * \file MainWindow.h
 * \brief Header of MainWindow
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

#include <LoginWindow.h>

namespace GUI
{
  /*!
   * Class description. May use HTML formatting
   *
   */
  class MainWindow : public QMainWindow
  {
    Q_OBJECT;

    LoginWindow *loginWindow;

    QAction *m_actionViewMap;
    QAction *m_actionViewFeed;
    QAction *m_actionQuit;
    QAction *m_actionSelectChannels;
    QAction *m_actionAddContent;
    QAction *m_actionEditRadius;
    QAction *m_actionShowDescription;
    QAction *m_actionViewOptions;
    QAction *m_logout;

    public:
      MainWindow();

      virtual ~MainWindow()
      {
      }

    protected:

      void createMenu();
      void createActions();

    public slots:
      void onActivate(QString auth_token);
      void onDeactivate();
      void addContent();

    private:
      MainWindow(const MainWindow& obj);
      MainWindow& operator=(const MainWindow& obj);

      // class MainWindow
  };

  // namespace GUI
}


//_MainWindow_H_06DCEED3_97B7_44E8_8292_AE47AA4C29BD_INCLUDED_
#endif

/* ===[ End of file ]=== */

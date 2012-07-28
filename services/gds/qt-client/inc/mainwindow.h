/*
 * Copyright 2012  Ivan Bezyazychnyy  ivan.bezyazychnyy@gmail.com
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QLabel>
#include <QStackedWidget>
#include <QSharedPointer>
#include <QMenuBar>

#include "GDSService.h"
#include "LoginWidget.h"
#include "CreateAccountWidget.h"
#include "MainWidget.h"
#include "SettingsWidget.h"
#include "Settings.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

    QSharedPointer<GDSService> m_GDSService;
  bool m_isSignedIn;
  Settings m_settings;

  // GUI
  QStackedWidget *m_stackedWidget;
  LoginWidget *m_loginWidget;
  CreateAccountWidget *m_createAccountWidget;
  MainWidget *m_mainWidget;
  SettingsWidget *m_settingsWidget;
  QMenuBar *m_menuBar;
  QMenu *m_menu;
  QAction *m_settingsAction;
  QAction *m_signOutAction;
  QAction *m_exitAction;

  public:
    enum ScreenOrientation
    {
      ScreenOrientationLockPortrait,
      ScreenOrientationLockLandscape,
      ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // Note that this will only have an effect on Symbian and Fremantle.
    void setOrientation(ScreenOrientation orientation);
    void showExpanded();

  public slots:
    void onLoginSignedIn(const QString& authToken);
    void onCreateAccountRequested();
    void onCreateAccountFinished();
    void onSettingsSaved();
    void onSettingsCancelled();

    void onSettingsAction();
    void onSignOutAction();
    void onExitAction();

  private:
    void createActions();
    void createMenus();
    void initGUI();
};
#endif                                  // MAINWINDOW_H

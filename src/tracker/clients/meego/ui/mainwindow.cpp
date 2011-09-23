#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextEdit>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&m_client,SIGNAL(authentificated()),SLOT(on_authentificated()));
    connect(&m_client,SIGNAL(error(QString)),SLOT(on_error(string)));
    ui->log_edit->append("Error log");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_auth_button_pressed()
{
    if (!m_client.isAuthentificated())
        m_client.auth(ui->loginEdit->text(),ui->passEdit->text());

}



void MainWindow::on_startstopButton_pressed()
{
    if (m_client.isTracking())
    {
        m_client.stopTrack();
        ui->startstopButton->setText("Start tracking");
    }
    else
    {
        m_client.startTrack();
        ui->startstopButton->setText("Stop tracking");
    }
}

void MainWindow::on_authentificated()
{
    ui->auth_button->setEnabled(false);
}

void MainWindow::on_error(QString error)
{
  // qDebug() << "Error in lib occured";
   ui->log_edit->append(QDateTime::currentDateTimeUtc().toString()+":" + error);
}

void MainWindow::on_nameEdit_textChanged(QString name )
{
    m_client.setDispName(name);
}

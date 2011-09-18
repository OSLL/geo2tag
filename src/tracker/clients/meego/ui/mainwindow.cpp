#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(&m_client,SIGNAL(authentificated()),this,SLOT(on_authentificated()));
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

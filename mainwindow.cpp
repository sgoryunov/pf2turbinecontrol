#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //! [1]
    //serial = new QSerialPort(this);
    //! [1]
    settings = new SettingsDialog;

    connect(ui->actionConfigure_serial,SIGNAL(triggered()),settings,SLOT(show()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete settings;
}

//! [4]
void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
//        console->setEnabled(true);
//        console->setLocalEchoEnabled(p.localEchoEnabled);
//        ui->actionConnect->setEnabled(false);
//        ui->actionDisconnect->setEnabled(true);
//        ui->actionConfigure->setEnabled(false);
//        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
//                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
//                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        //QMessageBox::critical(this, tr("Error"), serial->errorString());

        //showStatusMessage(tr("Open error"));
    }
}
//! [4]

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //! [1]
    serial = new QSerialPort(this);
    //! [1]
    settings = new SettingsDialog;

    connect(ui->actionConfigure_serial,SIGNAL(triggered()),settings,SLOT(show()));
    connect(ui->actionConnect,SIGNAL(triggered()),this,SLOT(openSerialPort()));
    connect(ui->actionDisconnect,SIGNAL(triggered()),this,SLOT(closeSerialPort()));
    connect(serial,SIGNAL(readyRead()),this,SLOT(ReadData()));
    // enable needed manu items
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure_serial->setEnabled(true);
    ui->pushButton->setEnabled(false);
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
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure_serial->setEnabled(false);
        ui->pushButton->setEnabled(true);
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
    }
}
//! [4]

//! [5]
void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure_serial->setEnabled(true);
    ui->pushButton->setEnabled(false);
}
//! [5]

void MainWindow::ReadData(void)
{
    QByteArray str = serial->readAll();
    qDebug()<<str;
//    quint8 measTime(0),waitingTime(0);
//    quint8 _signals[4] = {NULL};
//    if(!str.startsWith("!OUT_PARS:"))
//    {
//        QByteArray buff;
//        //measuring time
//        quint8 i(str.indexOf(':')),lastSepCharIndx(i);
//        i++;
//        for(;i<str.indexOf(',',lastSepCharIndx+1);i++) buff += str.at(i);
//        measTime = buff.toInt();
//        //waiting time
//        lastSepCharIndx = i;
//        i++;
//        buff = "";
//        for(;i<str.indexOf(',',lastSepCharIndx+1);i++) buff += str.at(i);
//        waitingTime = buff.toInt();
//        //signals
//        buff.clear();
//        for(quint8 j(0);j<4;j++)
//        {
//            lastSepCharIndx = i;
//            i++;
//            buff.insert(0,str.at(i++));
//            _signals[j] = buff.toInt();
//        }
//        qDebug()<<measTime<<" "<<waitingTime<<" "<<
//               _signals[0]<<" "<<_signals[1]<<" "<<_signals[2]<<" "<<_signals[3];

//    }
}

void MainWindow::ParseSerialData(QString str)
{
}

// start button
void MainWindow::on_pushButton_2_clicked()
{
    QByteArray buf("!START:");
    for(quint8 i(0);i<buf.length();i++)
        serial->putChar(buf.at(i));
}
// stop button
void MainWindow::on_pushButton_3_clicked()
{
    QByteArray buf("!STOP:");
    for(quint8 i(0);i<buf.length();i++)
        serial->putChar(buf.at(i));
}
// send parameters
void MainWindow::on_pushButton_clicked()
{
//    QString buf("!IN_PARS:");


//    for(quint8 i(0);i<buf.length();i++)
//        serial->putChar(buf.at(i));

}

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
    // почитстим буффер строки параметров
    m_inParamBuffer.clear();
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
    serial->setReadBufferSize(0);
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
    qint16 _index(0);
    if((_index = str.indexOf("\n"))!=-1)
    {
        if(_index > 1)m_inParamBuffer += str.mid(0,_index-1);
        qDebug()<<m_inParamBuffer.simplified();
        ParseParam();
        m_inParamBuffer.clear();
        m_inParamBuffer += str.mid(_index+1);
    }
    else m_inParamBuffer += str;
}
void MainWindow::ParseParam()
{
    quint8 measTime(0),waitingTime(0);
    quint8 _signals[4] = {NULL};
    if(m_inParamBuffer.startsWith("!OUT_PARS:"))
    {
        QByteArray buff;
        //measuring time
        quint8 i(m_inParamBuffer.indexOf(':')),lastSepCharIndx(i);
        i++;
        for(;i<m_inParamBuffer.indexOf(',',lastSepCharIndx+1);i++) buff += m_inParamBuffer.at(i);
        measTime = buff.toInt();
        //waiting time
        lastSepCharIndx = i;
        i++;
        buff = "";
        for(;i<m_inParamBuffer.indexOf(',',lastSepCharIndx+1);i++) buff += m_inParamBuffer.at(i);
        waitingTime = buff.toInt();
        //signals
        buff.clear();
        for(quint8 j(0);j<4;j++)
        {
            lastSepCharIndx = i;
            i++;
            buff += m_inParamBuffer.at(i++);
            _signals[j] = buff.toInt();
            buff.clear();
        }
        ui->lcdNumber->display(measTime);
        ui->lcdNumber_2->display(waitingTime);

        if(_signals[0]) ui->checkBox_4->setCheckState(Qt::Checked);
        else ui->checkBox_4->setCheckState(Qt::Unchecked);
        if(_signals[1]) ui->checkBox->setCheckState(Qt::Checked);
        else ui->checkBox->setCheckState(Qt::Unchecked);
        if(_signals[2]) ui->checkBox_2->setCheckState(Qt::Checked);
        else ui->checkBox_2->setCheckState(Qt::Unchecked);
        if(_signals[3]) ui->checkBox_3->setCheckState(Qt::Checked);
        else ui->checkBox_3->setCheckState(Qt::Unchecked);

        qDebug()<<measTime<<" "<<waitingTime<<" "<<
               _signals[0]<<" "<<_signals[1]<<" "<<_signals[2]<<" "<<_signals[3];

    }
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
    QString buf = QString("!IN_PARS:"
                          +ui->lineEdit_2->text()
                          +","
                          +ui->lineEdit->text());
    qDebug()<<buf;
    for(quint8 i(0);i<buf.toLocal8Bit().length();i++)
        serial->putChar(buf.toLocal8Bit().at(i));

}

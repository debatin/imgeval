#include "udptest.h"
#include "ui_udptest.h"
#include <QtGui>
#include <QUdpSocket>
#include <iostream>
using namespace std;
UdpTest::UdpTest(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::UdpTestClass)
{
    ui->setupUi(this);
    connect(ui->B_clear,SIGNAL(clicked()),ui->Data,SLOT(clear()));
    ui->L_Port->setText("hullo");
    initSocket();
}

UdpTest::~UdpTest()
{
    delete ui;
}


//! initiotalization of Udp Socket for recaption of udp data
void UdpTest::initSocket(){
    udp= new QUdpSocket(this);
    if(udp->bind( 703))
    {}
    else ui->L_Port->setText("cannot bind to socket");
    connect(udp, SIGNAL(readyRead()),
            this, SLOT(readUdpDatagram()));
    readUdpDatagram();
    connect(ui->B_clear, SIGNAL(clicked()),
            this, SLOT(readUdpDatagram()));
}


//! called whenever new udp data is available in order to read in the data
void UdpTest::readUdpDatagram()
{
    while (udp->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udp->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udp->readDatagram(datagram.data(), datagram.size(),
                          &sender, &senderPort);
        processUdp(datagram);
    }
}

void UdpTest::processUdp(QByteArray data)
{
    string s = data.data();
    string s1=s.substr(s.rfind("\\")+1);
    ui->Data->setText(ui->Data->toPlainText()+QString("\n")+QString(s1.c_str()));
    ui->L_Port->setText("hallo");
}

void UdpTest::on_B_exit_clicked()
{
    qApp->quit();
}

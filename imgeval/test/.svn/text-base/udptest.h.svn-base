#ifndef MAINUDP_H
#define MAINUDP_H

#include <QtGui/QMainWindow>
class QUdpSocket;
namespace Ui
{
    class UdpTestClass;
}

class UdpTest : public QMainWindow
{
    Q_OBJECT

public:
    UdpTest(QWidget *parent = 0);
    ~UdpTest();
    void initSocket();
public slots:
    void readUdpDatagram();
    void processUdp(QByteArray data);

private:
    Ui::UdpTestClass *ui;
    QUdpSocket* udp;


private slots:
    void on_B_exit_clicked();
};

#endif // UdpTest_H

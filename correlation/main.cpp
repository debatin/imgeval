
#include <QtGui/QApplication>
#include "mainwindow.h"
#include <iostream>
#include <fstream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CorrelationGUI w;
    w.show();
     return a.exec();
    return 0;

 }
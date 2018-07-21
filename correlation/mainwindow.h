#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QWidget>
#include<vector>
#include<iostream>
#include<QDir>
using namespace std;
class QPushButton;
class CorrelationGUI: public QWidget
{
    Q_OBJECT
public:
    CorrelationGUI(QWidget* parent =0);
public slots:
 void setFilename();
 void createFileList();
 void calculate();
private:
 vector<string> filelist;
//    Dataset* settings;
    QPushButton* filename_b;
    QDir dir;
//    QTextEdit* edit;
//    QComboBox* available_cmd;
//    MainWindow* mainwindow;
};


#endif // MAINWINDOW_H

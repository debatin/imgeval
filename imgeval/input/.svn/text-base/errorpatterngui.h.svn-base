#ifndef ERRORPATTERNGUI_H
#define ERRORPATTERNGUI_H


#include<iostream>
#include<QWidget>
using namespace std;
class Dataset;
class MainWindow;
class QLineEdit;
//!  loads a image file to the image stack
class ErrorPatternGui: public QWidget
{
    Q_OBJECT
public:
   ErrorPatternGui(Dataset* settings,MainWindow* mainapp, QWidget* parent=0 );
public slots:
   void loadfile();
   void clear();
private:
    Dataset* settings;
    MainWindow* mainapp;
    QLineEdit* speciesname;  //< the img is loaded to this variable on the stack
    string name;  //< name der species
};

#endif // ERRORPATTERNGUI_H

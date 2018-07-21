
#ifndef LOADTOSTACK_H
#define LOADTOSTACK_H

#include<QWidget>
class Dataset;
class MainWindow;
class QLineEdit;
//!  loads a image file to the image stack
class LoadToStack: public QWidget
{
    Q_OBJECT
public:
   LoadToStack(Dataset* settings,MainWindow* mainapp, QWidget* parent=0 );
public slots:
   void loadfile();
private:
    Dataset* settings;
    MainWindow* mainapp;
    QLineEdit* varname;  //< the img is loaded to this variable on the stack
};


#endif // LOADTOSTACK_H

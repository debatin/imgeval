#include"loadtostack.h"
#include"mainwindow.h"
#include"datastructure/dataset.h"
#include"datastructure/imgdata.h"
#include<QtGui>
#include<iostream>
using namespace std;
LoadToStack::LoadToStack(Dataset *settings, MainWindow *mainapp, QWidget *parent)
    :QWidget(parent)
{
   this->settings = settings;
   this->mainapp= mainapp;
   QVBoxLayout * lay = new QVBoxLayout;
   QPushButton* file_b = new QPushButton("select file");
   lay->addWidget(file_b);
   connect(file_b,SIGNAL(clicked()),this,SLOT(loadfile()));
   varname = new QLineEdit;
   lay->addWidget(varname);
   setLayout(lay);
}


void LoadToStack::loadfile()
{
    string filename = QFileDialog::getOpenFileName(this, tr("Open Directory"),"",tr("Images (*.tif *.mat *.blc)")).toStdString();
    mainapp->imgstack[varname->text().toStdString()].load(filename.c_str());
}

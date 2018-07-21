#include "filesavebox.h"
#include <iostream>
#include <fstream>
#include <string>
#include "datastructure/dataset.h"
#include <QtGui>
#include "savemode_gui/savemode.h"
using namespace std;


FileSaveBox::FileSaveBox(Dataset* setting, QWidget* parent)
  :QWidget(parent)
{
  settings=setting;
 
 QVBoxLayout* lay = new QVBoxLayout;
 QHBoxLayout* buttonlay = new QHBoxLayout;
 QHBoxLayout* buttonlay2 = new QHBoxLayout;
  //Buttons etc. for the Data Directory part
  
  //QLabel* label= new QLabel(tr("Save to:"),this);
  //lay->addWidget(label);
  ddir = new QLineEdit((*settings)["SaveFileName"].s.c_str(),this);
  ddir->setText((*settings)["SaveFileName"].s.c_str());
  ddir->setReadOnly(true);
  QPushButton* edit= new QPushButton("...",this);
  buttonlay->addWidget(ddir);
  buttonlay->addWidget(edit);

  QComboBox* save= new QComboBox(this);
  save->addItem("don't save");
  save->addItem("save_on_proceed");
  save->addItem("save_after_evaluation");
  QPushButton* extended = new QPushButton("erweitert",this);
  QPushButton* kill = new QPushButton("X",this);
  kill->setFixedWidth(30);
  save->setCurrentIndex(settings->get("save_mode",(int)2).i);
  buttonlay2->addWidget(save);
  buttonlay2->addWidget(extended);
  buttonlay2->addWidget(kill);
  connect(edit,SIGNAL(clicked()),this, SLOT(selectFile()));
  connect(save,SIGNAL(currentIndexChanged(int)),this, SLOT(toggleSavemode(int)));
  connect(extended, SIGNAL(clicked()),this, SLOT(showExtendedDialog()));
  connect(kill, SIGNAL(clicked()),this, SLOT(killLastLine()));
  lay->addLayout(buttonlay);
  lay->addLayout(buttonlay2);
  setLayout(lay);
}

//! opens a dialog for selecting the file, the results are stored to
void FileSaveBox::selectFile()
{
 dir = QFileDialog::getSaveFileName(this, tr("Open Directory"),"",tr("measfiles (*.txt)"));
  ddir->setText(dir);
  settings->store("SaveFileName",dir.toStdString());
}

/*sets the setting "saveonproceed" which decides whether to save or discard a evaluation
on proceeding to the next
 */
void FileSaveBox::toggleSavemode(int mode)
{
    settings->store("save_mode",mode);
}

//! displays extended dialog to select saving settings
void FileSaveBox::showExtendedDialog()
{
    Savemode* smode= new Savemode(settings);
    smode->show();
}


void FileSaveBox::killLastLine()
{

    string line;
    // open input file
    string infile=settings->get("SaveFileName").s;
    ifstream in(infile.c_str());
    string outfile=infile.substr(0,infile.find_last_of('/'))+"/buffer.txt";
    cout<<"kill"<<infile<<"out"<<outfile<<"\n";
    if( !in.is_open())
    {
    cout << "Input file failed to open\n";
    return;
    }
    // now open temp output file
    ofstream out(outfile.c_str());
    // loop to read/write the file. Note that you need to add code here to check
    // if you want to write the line
    string line2;
    while( getline(in,line) )
    {

    out << line2 << "\n";
    line2=line;
    }
    in.close();
    out.close();
    // delete the original file
    remove(infile.c_str());
    rename(outfile.c_str(),infile.c_str());



}

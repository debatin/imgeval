#include"errorpatterngui.h"
#include"mainwindow.h"
#include"datastructure/dataset.h"
#include<QtGui>
#include<vector>
#include<sstream>
/*!
  called if new data is found (by input list thread)
  inserts the data into the list and selects it if the appropriate button is checked
\author Markus Debatin
\date 10-2010
*************************************************************************/
ErrorPatternGui::ErrorPatternGui(Dataset* settings,MainWindow* mainapp, QWidget* parent )
    :QWidget(parent)
{
    this->settings = settings;
    this->mainapp= mainapp;
    QVBoxLayout * lay = new QVBoxLayout;
    QPushButton* file_b = new QPushButton("add file");
    lay->addWidget(file_b);
    connect(file_b,SIGNAL(clicked()),this,SLOT(loadfile()));
    QPushButton* clear_b = new QPushButton("clear errorpaterns");
    lay->addWidget(clear_b);
    connect(clear_b,SIGNAL(clicked()),this,SLOT(clear()));
    speciesname = new QLineEdit();
    speciesname->setText("Cs");
    lay->addWidget(speciesname);
    setLayout(lay);
}


void ErrorPatternGui::loadfile()
{
    name=speciesname->text().toStdString();
    ostringstream os;
    string directory = settings->get("err_patern_dir","").s;
    string filename = QFileDialog::getOpenFileName(this, tr("Open Directory"),directory.c_str(),tr("Images (*.tif *.mat *.blc)")).toStdString();
    string dir = QDir(filename.c_str()).absolutePath().toStdString();
    settings->store("err_patern_dir",dir);
    vector<string> errpats; errpats  = settings->get(name+"errorpatterns",errpats).vs;
    errpats.push_back(filename);
    os<<errpats.size();                                                      //< numer of this errorpattern
    settings->store(name+"errorpatterns",errpats);                       //< save name
    mainapp->imgstack[name+"errpattern"+os.str()].load(filename.c_str());   //load image
}



void ErrorPatternGui::clear()
{
    name=speciesname->text().toStdString();

    vector<string> errpats;
    errpats  = settings->get(name+"errorpatterns",errpats).vs;
    // settings->store(name+"errorpattern_nr",0);
    cout<<errpats.size()<<"clearing errorpatterns\n";
    ostringstream os;
    for(int i = 0;i<errpats.size();i++){
        cout<<i<<"\n";
        os.str(""); os<<i+1;
        mainapp->imgstack.erase(mainapp->imgstack.find(name+"errpattern"+os.str()));
    }
    cout<<"clearing done\n";
    errpats.clear();
    settings->store(name+"errorpatterns",errpats);                       //< save name
    //settings->store();
}

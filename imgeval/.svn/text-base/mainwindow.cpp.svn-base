#include <QtGui>
#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include "gui_graphs/atomimg.h"
#include "input/inputlist.h"
#include "datastructure/dataset.h"
#include "savemode_gui/filesavebox.h"
#include "gui_settings/settingsquick.h"
#include "gui_settings/interpreter_gui.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "datastructure/numeric_templates.h"
using namespace std;
//! constructor of Mainwindow
/*! The main window is created, the settings are loaded
Most of the widgests in th main Window are Dock-Widgets in order to allow
Flexible changing of the window configuration
  */
MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)//, ui(new Ui::MainWindowClass)
{

    externdata = new Dataset;
    settings = new Dataset;
    settings->fromFile("settings.dat");
    //checkSettings();
    results= new Dataset;
    results->setCategory('r');  //the category helps sorting them later
    //--------------------------------
    //ui->setupUi(this);
    ostringstream os;
    vector<string> v; //creates empty vector every class will register ists extensions
    settings->store("file_extensions",v);

    for(int i = 0;i<settings->get("number_of_species",1,'s').d;i++){
        os.str("");
        os<<"species"<<(i+1);
        frames.push_back(new Atomimg(results,settings,settings->get(os.str(),"Rb",'s').s,this,this));
    }

    //the central image windows can be set as central widget or just as normal
    //Dockapp
    if(settings->get("main_as_frames",(int)0,'s').i){ //use as Dock app
        for(unsigned int i = 0; i<frames.size();i++){
            mainDocks.push_back(new QDockWidget( QString(settings->get(os.str()).s.c_str()),this));
            mainDocks[i]->setWidget(frames[i]);
            //        mainDocks[i]->setAllowedAreas(Qt::BottomDockWidgetArea);
            mainDocks[i]->setAllowedAreas(Qt::AllDockWidgetAreas);
            addDockWidget(Qt::BottomDockWidgetArea, mainDocks[i]);
            os.str("");
	}
    }
    else{   //set as central widget
	QHBoxLayout* blay= new QHBoxLayout;
	QWidget* central = new QWidget;
        for(uint i = 0; i<frames.size();i++){
            blay->addWidget(frames[i]);
	}
	central->setLayout(blay);
	this->setCentralWidget(central);
    }
    for(uint i = 0; i<frames.size();i++)
        connect(this,SIGNAL(markerchanged(QString, QPointF)),
                frames[i],SLOT(markerchanged(QString, QPointF)));

    LOG
            InputList* meas_info = new InputList(settings,this ,this);
    //Settingsquick* qresults = new Settingsquick(settings,"results",settings,this);
    Settingsquick* qresults = new Settingsquick(results,"results",settings,this);

    LOG
            //   setCentralWidget(central);
            QDockWidget *iastelligch = new QDockWidget(tr("Iastelligch"), this);
    QDockWidget *SaveBoxWidget = new QDockWidget(tr(""), this);
    QDockWidget *quicklistWidget = new QDockWidget(tr("Quicklist"),this);
    QDockWidget *resultsWidget = new QDockWidget(tr("results"),this);
    QDockWidget *interpreterWidget = new QDockWidget(tr("interpreter"),this);

    iastelligch->setAllowedAreas(Qt::LeftDockWidgetArea |Qt::TopDockWidgetArea);
    SaveBoxWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    quicklistWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    resultsWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    interpreterWidget->setAllowedAreas(Qt::AllDockWidgetAreas);

    iastelligch->setWidget(meas_info);
    SaveBoxWidget->setWidget(new FileSaveBox(settings,this));
    quicklistWidget->setWidget(new Settingsquick(settings,"settings",settings,this));
    resultsWidget->setWidget(qresults);
    interpreterWidget->setWidget(new InterpreterGui(settings,this,this));
    //interpreterWidget->setWidget(new QWidget(this));

    addDockWidget(Qt::TopDockWidgetArea, iastelligch);
    addDockWidget(Qt::TopDockWidgetArea, SaveBoxWidget);
    addDockWidget(Qt::RightDockWidgetArea, quicklistWidget);
    addDockWidget(Qt::RightDockWidgetArea, resultsWidget);
    addDockWidget(Qt::RightDockWidgetArea, interpreterWidget);

    connect(this,SIGNAL(data_evaluated()),qresults,SLOT(updateList()));
    connect(meas_info,SIGNAL(measSelected(QString)),this,SLOT(calculate(QString)));
    LOG
            setStatusBar(new QStatusBar(this));
    statusBar()->showMessage("useful hints :-)");
    setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
    LOG
            settings->toFile("settings.dat");
    delete externdata;
    delete settings;
    delete results;


    // delete ui;

}

void MainWindow::calculate(const QString& newmeasnr)
{
    if (!(measnr==newmeasnr))
        if (settings->get("save_mode").i==SAVE_ON_PROCEED)
            postProcess();

    // NEW PART
    measnr=newmeasnr;
    results->clear();
    results->fromFile((measnr.toStdString() + "_eval.txt").c_str());
    std::string s= measnr.toStdString();
    s.erase(0,s.find_last_of('/')+1);
    results->store("Messnr",s);
    readlogdata();
    readExternData("settings");   //reading parameters which might be supplied in external files

    for(uint i = 0;i<frames.size();i++){
        frames[i]->setMeasurement(measnr);
        frames[i]->reloadData();
        frames[i]->fitData();
    }
    imgcommand_interpreter();

    settings->join(results);
    if (settings->get("save_mode").i==SAVE_AFTER_EVALUATION)
        postProcess();
}

//! reads in the logdata for the current measurement
/*!
the _logdata.txt file is created by the experiment controlprogram and contains
    infromation on a measurement series

*/
void MainWindow::readlogdata()
{
    ifstream logdat;
    try{
        logdat.open((measnr.toStdString()+"_logdata.txt").c_str());
    }
    catch(...)
    {
        return;
    }
    char buffer[100];
    double d;
    while(logdat.peek()=='D')logdat.get();
    logdat.getline(buffer,100,'=');
    measname=string(buffer);
   // measname= measname+"_";
    logdat>>d;
    if(!measname.empty())
    results->store(measname,d);
    logdat.close();
}
/*! reads  additional data from external data files
and stores them as results
\param type either "data" or "settings" indicates which variable to read and where to store them
*/
void MainWindow::readExternData(string type)
{

    vector<string> dummy;
    vector<string> external_data = settings->get("external_"+type+"_files",dummy,'s').vs;
    for(unsigned int i = 0; i<external_data.size();i++)
    {
        externdata->clear();
        externdata->fromFile(external_data[i].c_str());
        cout<<externdata->size()<<"   external datasets read from "<<external_data[i].c_str()<<"\n";
        if(type == "data")
        results->join(externdata);
        if(type == "settings")
        settings->join(externdata);

    }
}


//save previous calculations load results for next meas if existing
void MainWindow::postProcess(){
    readExternData("data");
    results->toFile((measnr.toStdString() + "_eval.txt").c_str());
    results->toFile((*settings)["SaveFileName"].s.c_str(),settings->get("SaveStyle","",'s').s);
    //save thumbnails
    if((settings->get("saveimg",(int)0).i & SAVEIMG_IMG) != 0)
        for(uint i =0;i<frames.size();i++)
            frames[i]->savePNG();


    if((settings->get("saveimg",(int)0).i & SAVEIMG_MAT) != 0)
        for(uint i =0;i<frames.size();i++)
            frames[i]->saveMAT();


    //check for Background modification
    if(settings->get("take_as_background",(int)0).i){
        settings->get("backgroundfiles").vs.push_back(measnr.toStdString());
        for(uint i =0;i<frames.size();i++)
            frames[i]->reloadBackground();
    }
}




//! creates some hopefully usful initial settings in case there is no settings.dat
void MainWindow::checkSettings(){
    //****************************************
 /*  if((*settings)["m/pixel"].d==0) settings->store("m/pixel",3.456e-6,'p');
    if((*settings)["Rbwavelength"].d==0)settings->store("Rbwavelength",780e-9,'p');
    if((*settings)["Cswavelength"].d==0)settings->store("Cswavelength",852e-9,'p');
    if((*settings)["Rbcrossection"].d==0)settings->store("Rbcrossection",0.223,'p');  //yielding crossect for Rb
    if((*settings)["Cscrossection"].d==0)settings->store("Cscrossection",0.1945,'p'); //yielding the right crossect for
    //isotropic polarization
    if((*settings)["Rblinewidth"].d==0)settings->store("Rblinewidth",5.95e6,'p');  //unknown
    if((*settings)["Cslinewidth"].d==0)settings->store("Cslinewidth",5.22e6,'p');   //from Cs3
    //cout<<"Mainwindow: "<<settings->get("Rbdetuning")<<"\n";
    if((*settings)["Rbdetuning"].d==0)settings->store("Rbdetuning",1.95e0,'p');  //unknown
    if((*settings)["Csdetuning"].d==0)settings->store("Csdetuning",1.22e0,'p');   //unknown
    if((*settings)["palette"].vd.size()<6){
        cout<<"restoring palette\n";
        vector<double> v;
        v.push_back(0);  v.push_back(0);  v.push_back(0);
        v.push_back(0);  v.push_back(0);  v.push_back(255);
        v.push_back(0);  v.push_back(255);  v.push_back(0);
        v.push_back(255);  v.push_back(0);  v.push_back(0);
        v.push_back(255);  v.push_back(255);  v.push_back(255);
        settings->store("palette",v);
    }*/
}

//! sets/clears the Bit in saveimg which telle whether the current image is a background
void MainWindow::addToBackground(bool toggled)
{
    settings->store("take_as_background",(int) toggled );
}


///! empties the list of backgroundfiles without deleting the files
/*! also updates the respective images
*/
void MainWindow::clearBackground()
{
    vector<string> v;
    settings->store("backgroundfiles",v);
    for(uint i =0;i<frames.size();i++)
        frames[i]->reloadBackground();
}

///! function for synchronization of the markers
void MainWindow::changemarker(QString name, QPointF pos)
{
    emit markerchanged(name, pos);
}

void MainWindow::imgcommand_interpreter()
{
    vector<string> commands;
    string scriptname = settings->get("imgscriptname","imgscript1",'s').s;
    commands = settings->get(scriptname,commands,'s').vs;
    imgcommand_interpreter(commands);
}

void MainWindow::imgcommand_interpreter(vector<string> commands)
{
// just list all available commands in the settings somewhere
    vector<string> cmdlist;
    cmdlist.push_back("+=");
    cmdlist.push_back("-=");
    cmdlist.push_back("=");
    cmdlist.push_back("*=");
    cmdlist.push_back("/=");
    cmdlist.push_back("rowsmooth");
    cmdlist.push_back("colsmooth");
    cmdlist.push_back("del");
    cmdlist.push_back("refresh ");
    cmdlist.push_back("im scale 1.0 ");
    cmdlist.push_back("im xscale 1.0 ");
    cmdlist.push_back("im yscale 1.0 ");
    settings->store("commandlist",cmdlist);


    for(unsigned int i = 0; i<commands.size();i++)
    {
        string ziel,command,source;
        std::istringstream is;
        std::istringstream is1;
        is.str(commands[i]);
        is>>ziel>>command>>source;
        if(command== "+="){
            if((*source.data()>='0')&&(*source.data()<='9')){
                float f;
                is1.str(source); is1>>f;
                imgstack[ziel] += f;
            }
            else
                imgstack[ziel] += imgstack[source];
        }
        else if(command== "="){
            imgstack[ziel] = imgstack[source];
        }
        else if(command== "-="){
            if((*source.data()>='0')&&(*source.data()<='9')){
                float f;
                is1.str(source); is1>>f;
                imgstack[ziel] -= f;
            }
            else
                imgstack[ziel] -= imgstack[source];
        }
        else if(command== "*="){
            if((*source.data()>='0')&&(*source.data()<='9')){
                float f;
                is1.str(source); is1>>f;
                imgstack[ziel] *= f;
            }
            else
                imgstack[ziel] *= imgstack[source];
        }
        else if(command== "/="){
            if((*source.data()>='0')&&(*source.data()<='9')){
                float f;
                is1.str(source); is1>>f;
                imgstack[ziel] /= f;
            }
            else
                imgstack[ziel] /= imgstack[source];
        }
        else if(command== "rowsmooth"){
            if((*source.data()>='0')&&(*source.data()<='9')){
                int i;
                is1.str(source); is1>>i;
                imgstack[ziel].rowsmooth(i);
            }
        }
        else if(command== "colsmooth"){
            if((*source.data()>='0')&&(*source.data()<='9')){
                int i;
                is1.str(source); is1>>i;
                imgstack[ziel].colsmooth(i);
            }
        }
        else if(command== "del")  //removes element from stack
        {
            imgstack.erase(imgstack.find(ziel));
        }
        else if(command== "refresh")  //removes element from stack
        {
            for(uint i =0;i<frames.size();i++)
                frames[i]->setImage();
        }
        else if(command== "scale")  //removes element from stack
        {
            if((*source.data()>='0')&&(*source.data()<='9')){
                float f;
                is1.str(source); is1>>f;
                cout<<"scaling"<<f<<"\n";
                imgstack[ziel].scale(f);
            }
        }
        else if(command== "xscale")  //removes element from stack
        {
            if((*source.data()>='0')&&(*source.data()<='9')){
                float f;
                is1.str(source); is1>>f;
                cout<<"xscaling"<<f<<"\n";
                imgstack[ziel].xscale(f);
            }
        }
        else if(command== "yscale")  //removes element from stack
        {
            if((*source.data()>='0')&&(*source.data()<='9')){
                float f;
                is1.str(source); is1>>f;
                cout<<"yscaling"<<f<<"\n";
                imgstack[ziel].yscale(f);
            }
        }
     }
}

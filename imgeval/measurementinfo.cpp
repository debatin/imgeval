#include "measurementinfo.h"
#include <QtGui>
#include "dataset.h"
#include<vector>
using namespace std;
MeasurementInfo::MeasurementInfo(Dataset* settings,QWidget* parent)
  :QWidget(parent)
{
    this->settings = settings;

  QGridLayout* lay = new QGridLayout;
  //Buttons etc. for the Data Directory part
  dir = new QDir(settings->get("RawdataDir").s.c_str());
  QVBoxLayout* dirlay = new QVBoxLayout;
  QHBoxLayout* hlay= new QHBoxLayout;
  ddir = new QLineEdit(settings->get("RawdataDir").s.c_str(),this);
  ddir->setReadOnly(true);
  hlay->addWidget(ddir);
  QPushButton* edit= new QPushButton("...",this);
  hlay->addWidget(edit);
  dirlay->addLayout(hlay);
  connect(edit,SIGNAL(clicked()),this, SLOT(selectD()));
  lay->addLayout(dirlay,0,0);

  //Button etc. for the Measurment Part
  QHBoxLayout* laynr=new QHBoxLayout;
  QLabel* mnrlabel = new QLabel(tr("Messnr.:"),this);
  laynr->addWidget(mnrlabel);
  measlist = new QComboBox(this);
  laynr->addWidget(measlist);
  calcnew = new QPushButton(tr("autoupdate"),this);
  calcnew->setCheckable(true);
  laynr->addWidget(calcnew);
  //nocalc = new QPushButton(tr("no fits"),this);
  //nocalc->setCheckable(true);
  //laynr->addWidget(nocalc);
  lay->addLayout(laynr,1,0);
  setLayout(lay);

  connect(measlist,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(measnrChanged(const QString& )));

  //  connect(calcnew,SIGNAL(clicked()),this,SLOT(rescandir()));
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(rescandir()));
  if (settings->get("directory_rescan_time").d==0)
        settings->store("directory_rescan_time",10);
  timer->start(settings->get("directory_rescan_time").d*1000);

//INIT
   scandir();


}
/*!
scans the current Measurementdirectory for (new) files that are in the scheme xxxxyyy
where for a name xxxx to be accepted there has to ba a file xxxxyyy for each yyy in
"file_extensions"
 */

QStringList MeasurementInfo::getImgList()
{
  QStringList imglist;
  QList<QStringList> filters;
  QList<QStringList> filenames;
  vector<string> extensions = settings->get("file_extensions").vs;
  for(unsigned int i = 0;i<extensions.size();i++)
  filters.append(QStringList( "*"+QString(extensions[i].c_str()))) ;
   //check for files with respective names and add them to n lists
   //only check for files with read permission and sort by name
  for(unsigned int i = 0;i<extensions.size();i++)
    {
      filenames.append(dir->entryList(filters[i],
				    QDir::Readable|QDir::Files,QDir::Name));
      filenames[i].replaceInStrings(QString(extensions[i].c_str()),"");
    }

  //only keep names which are in all 4 lists (Measurements with 4 images)
  for(int i = 0;i<filenames[0].size();i++){
    bool exists=true ; //is set to false if the item doesnt exist in one list
    for(int j = 1; j<extensions.size();j++)
      exists =exists && filenames[j].contains(filenames[0][i]);
    if (exists) imglist.append(filenames[0][i]);
  }
  return imglist;

}

void MeasurementInfo::scandir()
{
  measlist->clear();
  measlist->addItems(getImgList());
}
//! scans the directory and sets the measlist for each new item which then might also start a calculation

void MeasurementInfo::rescandir()
{
  QStringList imglist= getImgList();
  
  for(int i =0;i<imglist.size();i++)
    if(measlist->findText(imglist[i])==-1){ //new item
      measlist->addItem(imglist[i]);
      if(calcnew->isChecked()) measlist->setCurrentIndex(measlist->count()-1);
    }
}


void MeasurementInfo::measnrChanged(const QString meas)
{
  if(!meas.isEmpty())
    emit measSelected(dir->path()+"/"+meas);
}








//deprecated
DataDirectory::DataDirectory(QWidget* parent)
  :QWidget(parent)
{
}

void MeasurementInfo::selectD()
{
  *dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
					   "",
					   QFileDialog::ShowDirsOnly
					   | QFileDialog::DontResolveSymlinks);
  ddir->setText(dir->path());
  settings->store("RawdataDir",dir->path().toStdString());
  scandir();

}

#include<QtGui>
#include "inputlistthread.h"
#include "datastructure/dataset.h"
#include<fstream>
#include "datastructure/numeric_templates.h"
using namespace std;
//!constructor of the thread
InputListThread::InputListThread(Dataset* setting, QDir dir)
{
    logfile.open("thread.log");
    settings=setting;
    perform_scandir=settings->get("scan_directory(0=false)",(int) 0).i;
    logfile<<"scanning"<<endl<<flush;
    updateExtensions();
    setDir(dir);
    abort = false;
    rescan=true;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(rescandir()));
  //  connect(timer, SIGNAL(timeout()), this, SLOT(updateExtensions()));
    if (((int) settings->get("directory_rescan_mode",1).i&1) == 1)
         timer->start(settings->get("directory_rescan_time",10).i*1000);
}
//! destructor stopts the thread
InputListThread::~InputListThread()
{
     LOG
     lock.lock();
     abort = true;
     condition.wakeOne();
     lock.unlock();
     wait();
     logfile.close();
     LOG
}

//! set the directory to scan and scans it
void InputListThread::setDir(QDir newdir)
{
    if(!newdir.isReadable()) newdir=QDir::currentPath();
    lock.lock();
    dir=newdir;
    //logfile<<"scanning"<<endl<<flush;
    lock.unlock();
    scandir();
}

//! set the file name extensions to scan for
void InputListThread::updateExtensions(){
    lock.lock();
    extensions = settings->get("file_extensions").vs;
    lock.unlock();
}
/***************** MAIN THREAD KEEPS PROGRAM LOOKING FOR NEW DATA ***************************/
//!main thread This Function is the separate Thread
void InputListThread::run()
{
  forever{

    if(abort) return;  //
    lock.lock();
    QDateTime modified =dir.entryInfoList(QStringList("."))[0].lastModified();

    if(lastmodification != modified ){
        lastmodification = modified;
        emit dirmodified(lastmodification.toString());
        lock.unlock();
        QStringList imglist;
        if(perform_scandir)imglist = getImgList();
        string udp = probeUdpList();
        lock.lock();
        if (udp != "") {
            LOG
            write_logfile(udp.c_str(),20);
            measlist<<QString(udp.c_str());
            emit newDataFound(QString(udp.c_str()));
        }
        for(int i =0;i<imglist.size();i++)
           if(! measlist.contains(imglist[i])){ //new item
                  measlist<<imglist[i];
                  emit newDataFound(imglist[i]);
           }
    }
    if (!rescan)
       condition.wait(&lock);
     rescan = false;
     lock.unlock();
  }
}


//! scans the directory for data and takes it as its list.
void InputListThread::scandir()
{
    lock.lock();
    dir.refresh();
    lastmodification = dir.entryInfoList(QStringList("."))[0].lastModified();
    lock.unlock();
    QStringList mlist =getImgList();
    lock.lock();
    measlist=mlist;
    lock.unlock();
}

void InputListThread::rescandir()
{
    QMutexLocker locker(&lock);
    if (!isRunning()) {
         start(LowPriority);
     } else {
         rescan = true;
         condition.wakeOne();
     }
}

//! probes if the files for the dataset indicated by udp exists.
string InputListThread::probeUdpList()
{
    LOG
    QMutexLocker locker(&lock);
    LOG
    for( int i =0;i<udplist.size();i++){
        write_logfile(udplist[i],i);
        if(checkSingleMeasnr(udplist[i])){  //< all data for this set exist
            string s=udplist[i];
            for(int j=0;j<=i;j++) udplist.erase(udplist.begin());
            return s;
        }
    }
    return "";
}


//! checks whether all files exist for a given measurement index
/* the check is performed by trying to open the respective files

   \param measnr index of the measurement to check
*/
bool InputListThread::checkSingleMeasnr(string measnr)
{
    //QMutexLocker locker(&lock);  //already locked from probe udp
    for(int i =0;i<extensions.size();i++)
     if(!(dir.exists(QString((measnr+extensions[i]).c_str())))) return false;
  return true;
}


//!scans the directory in order to get a list of measurements (very time consuming!)
/*!

  */
QStringList InputListThread::getImgList()
{
//    QTime dieTime = QTime::currentTime().addSecs(2); //nice wy of giving other processes a chance
//while( QTime::currentTime() < dieTime )
//        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
//--------------
  QMutexLocker locker(&lock);
  QStringList imglist;
  QList<QStringList> filters;
  QList<QStringList> filenames;
   vector<string> extens = extensions;
  for(unsigned int i = 0;i<(int)extens.size();i++)
  filters.append(QStringList( "*"+QString(extens[i].c_str()))) ;
   //check for files with respective names and add them to n lists
   //only check for files with read permission and sort by name
  for(int i = 0;i<(int)extens.size();i++)
    {
      filenames.append(dir.entryList(filters[i],
                                    QDir::Readable|QDir::Files,QDir::Name));
      filenames[i].replaceInStrings(QString(extens[i].c_str()),"");
        }
  if(filenames.size()>0)     //protect from totally empty lists
  for(int i = 0;i<filenames[0].size();i++){
    bool exists=true ; //is set to false if the item doesnt exist in one list
    for(int j = 1; j<extens.size();j++)
      exists =exists && filenames[j].contains(filenames[0][i]);
    if (exists)
        imglist.append(filenames[0][i]);
  }
  return imglist;
}

QStringList InputListThread::getMeasList(){
    QMutexLocker locker(&lock);
    return measlist;
}

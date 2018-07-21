#ifndef INPUTLISTTHREAD_H
#define INPUTLISTTHREAD_H
#include<QThread>
#include<QMutex>
#include<QDir>
#include<vector>
#include<iostream>
#include<QString>
#include<QWaitCondition>
#include<QDateTime>
#include<fstream>
#include "datastructure/numeric_templates.h"
class Dataset;
using namespace std;

//! Thread class for reading the contents of the directory
/*!
  As reading the contents of the directory is a time conuming task using
  badly designed proprietary file systems like NTFS this task is put into a
  separate thread to enshure good performance even when operating on NTFS file systems
  like the ones used under Windows XP.


  */
class InputListThread: public QThread{
   Q_OBJECT
public:
    InputListThread(Dataset* setting, QDir dir);
    ~InputListThread();
    QStringList getMeasList();
   void setDir(QDir newdir);
   void addUdp(string message){udplist.push_back(message); probeUdpList(); }; ///< adds message to udp list
   string probeUdpList();
   bool checkSingleMeasnr(string measnr);
signals:
    void newDataFound(QString);
    void dirmodified(QString);
protected slots:
    void updateExtensions();
    void rescandir();
protected:
    void scandir();
   QStringList getImgList(); ///< creates list of available imgs (properly named)
   void run();                ///< main thread of this class

   QStringList measlist;       ///< list of complete measurements
   QMutex lock;                ///< mutex lock to prohibit dangerous simultaneous access of two thread
   bool rescan;                ///< used to contrl the rescanning
   bool abort;                 ///< used to control whether the thread should stop
   Dataset* settings;          ///< pointer to the central settings of the program
   QDir dir;                   ///< Directory of the measurement data
   vector<string> extensions;  ///< List of file extensions for a complete Data set
   vector<string> udplist;  ///< List of Measurements where not all files exist yet
   QDateTime lastmodification;  ///< time when the directories were modified last
   QWaitCondition condition;
   ofstream logfile;             ///< logfile for debugging
   bool perform_scandir;
};

#endif // INPUTLISTTHREAD_H

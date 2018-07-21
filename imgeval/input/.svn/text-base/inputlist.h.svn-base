#ifndef InputList_H
#define InputList_H
#include <QWidget>
#include <QDir>
#include <QDateTime>
#include <QString>
#include "mainwindow.h"
#include "datastructure/numeric_templates.h"
class QLineEdit;
class QComboBox;
class QPushButton;
class Dataset;
class QLabel;
class InputListThread;
class QUdpSocket;
class UdpTest;
//!Path and Measurement selection, Handling of UDP Information
/*!Allows to select a path to the Pictures taken.
Scans the directory for Filesequences named appropriately
and includes the found filesequences in a measurement selector

The directory is rescanned every "directory_rescan_time" seconds for new measurements.
The path is stored, when the programm is quit and restored afterwards.

The scanning is controlled by "directory_rescan_mode" which is a bitmask
00001 = rescan automatically
00010 = rescan for files newer than the last files found only
00100 = load and proceed new data found
10000 = Process incoming udp signal
*/

class InputList :public QWidget
{
    Q_OBJECT
public:
    InputList(Dataset* settings, MainWindow* mainapp, QWidget* parent=0);  ///< constructor for GUI
    ~InputList();
    void initUdpSocket();
    inline  QDir getDir(){return *dir;}; ///< current selected Directory
public slots:
    void insertNewData(QString newdata);  ///< called by the thread when newdata is found
    void dirmodified(QString dd); ///< for debugging
    void scandir();  ///< scans the directory for new measurements
    void selectD();  ///< Show directory selection dialog
    void measnrChanged(const QString meas);
    void ReloadData(); ///< triggers the reloading of the data (emits measSelected)
    void createActions(); ///< create the actions for the context menu
    void addErrorpatterns(); ///< opens a Window to add error Patterns
    /************************** UDP *****************************************/
    void readUdpDatagram(); ///< event handler for incoming udp
    void autoupdate_triggered(bool checked);
    void loadtostack();   ///< open window to load files to imgstack
signals:
    void measSelected(QString meas);
protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    bool scan_finished;
    QComboBox* measlist; //List of available Measurements
    QString current_meas; //current meas index
    QLineEdit* ddir;
    QDir* dir;
    QPushButton* calcnew;
    QPushButton* nocalc;
    Dataset* settings;
    QDateTime lastmodification;
    QLabel* l;
    InputListThread* thread;
    QAction* takebackgroundAct;
    QAction* clearbackgroundAct;
    QAction* addErrorPatternsAct;
    QUdpSocket* udp;
     UdpTest* w;
public:
    MainWindow* mainapp;
};



#endif

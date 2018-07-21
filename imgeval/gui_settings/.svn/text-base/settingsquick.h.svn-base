#ifndef SETTINGSQUICK_H
#define SETTINGSQUICK_H
#include <QWidget>
#include <QString>
#include<iostream>
#include<QDir>
#include"datastructure/dataset.h"
using namespace std;
class Dataset;
class QLineEdit;
class QAction;
class QContextMenuEvent;
class QVBoxLayout;
class QLabel;
/*! entry class for the Quicklist
  Displays one entry of the quicklist and handles also its changes to the corresponding dataset

  */

class baseclass
{
    public:
    baseclass(Dataset* settings){this->settings=settings;};
     m_entry& get(string name){return settings->get(name);}
     template<typename T>  m_entry& get(string name, T d);
    protected:
    Dataset* settings;

};
template<class T> inline
m_entry& baseclass::get(string name, T d)
{
    return settings->get(name,d);
}


class QuicklistEntry: public QWidget, baseclass
{
Q_OBJECT
public:
        void resizeEvent ( QResizeEvent * event );

    QuicklistEntry(Dataset* values, string name,string listname, Dataset* settings, QWidget* parent=0);    //< constructor
public slots:
    void entryChanged(); //< called whenever the entry is changed
private:
        QLineEdit* edit;
         QLabel* label;

    Dataset* values;
    string name;
    string listname;
};

/*! GUI class for quick change of settings of the system


  */

class Settingsquick: public QWidget
{
Q_OBJECT
  public:
  Settingsquick(Dataset* settings, QWidget* parent = 0);
  Settingsquick(Dataset* settings,string name, Dataset* settingsg, QWidget* parent = 0);
  void init();
    void createActions();   //< create Actions for the contextmenu
  public slots:
    void displaySettings();  //< opens a settingscontrol window
    void saveSettings();  //< saves the central settings to a file
    void loadSettings();  //< loads the central settings from a file
    void clearList();      //< clears the list
    void updateList();    //< called whenever the List of displayed variables is changed
    void contextMenuEvent(QContextMenuEvent *event);
 private:
  Dataset* values;
  Dataset* settings;
//  Dataset* settingsg;
  string name;
  QLineEdit* ddir;
  QString dir;
  QAction* showSettingsAct;
  QAction* saveSettingsAct;
  QAction* loadSettingsAct;
  QAction* clearQuickAct;
  QVBoxLayout* lay;
};



#endif // SETTINGSQUICK_H

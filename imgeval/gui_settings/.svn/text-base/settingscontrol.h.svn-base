#ifndef SETTINGSCONTROL_H
#define SETTINGSCONTROL_H
#include <QWidget>
#include <QString>
#include<iostream>
#include<QDir>
#include"datastructure/dataset.h"
using namespace std;
class QLineEdit;
/*!  Displays the settings of the Program and allows for adjustments
    Changes to the settings can be made
    a Button can be clicked which emits a Signal containing the name of the entry
*/

class SettingscontrolEntry: public QWidget
{
Q_OBJECT
    public:
        SettingscontrolEntry(map<string, m_entry>::iterator setting,QWidget* parent=0);
    public slots:
        void changeSetting();
        void select();
        signals:
        void selected(QString s);
    protected:
    //Dataset* settings;
    map<string, m_entry>::iterator s;
    QLineEdit* edit;
};


class Settingscontrol: public QWidget
{
Q_OBJECT
  public:
  Settingscontrol(Dataset* settings, string name,QWidget* parent = 0);
  public slots:
  void displaySettings();
  void addToQuickdisplay(QString name);
  signals:
  void changed();
 private:
  Dataset* settings;
  QLineEdit* ddir;
  QString dir;
  string name;
};



#endif // SETTINGSCONTROL_H

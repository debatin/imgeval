#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <vector>
#include <QPointF>
#include <QString>
#include"datastructure/imgdata.h"
#include <map>
#include <iostream>
using namespace std;
class Dataset;
class Atomimg;

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
      MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void checkSettings();
public slots:
      void calculate(const QString& measnr);  ///<calculates the indicated measurement
      void postProcess();  ///<stores Data etc.
      void addToBackground(bool); ///<adds current images to the background images set
      void clearBackground(); ///<clears Background, so no background will be taken
      void readlogdata();
      void changemarker(QString name, QPointF pos);  /// inform the frames that one of the markers has changed
      void dataeveluated(){emit data_evaluated();};
      void imgcommand_interpreter();
      void imgcommand_interpreter(vector<string> commands);  ///< interprete and run commands
      void readExternData(string);  ///< reads additional data from external data files
signals:
      void markerchanged(QString name, QPointF pos);
      void data_evaluated();

  protected:
  vector<Atomimg*> frames;
  vector<QDockWidget*> mainDocks;
  Dataset* results;
  QString measnr;
  std::string measname;
  float channels[5]; ///<stores the values for the channels from the control prog
public:
  map<string,Imagedata> imgstack;

  private:
  Dataset* settings;
  Dataset* externdata;
   // Ui::MainWindowClass *ui;
};

#endif // MAINWINDOW_H

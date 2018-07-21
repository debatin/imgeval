#ifndef PARAMBOX_H
#define PARAMBOX_H
#include<QtGui>
#include<iostream>
class Dataset;
class QLabel;
class QTextEdit;
class QResizeEvent;
class ParamBox: public QWidget
{
  Q_OBJECT

    public:
  ParamBox(Dataset* data,std::string name,QWidget* parent = 0);
void resizeEvent ( QResizeEvent * event );
public slots:
  void reload();
 private:
  Dataset* data;
  QLabel* label;
  QLabel* nsum;
  QTextEdit* infos;
  std::string name;

};



#endif

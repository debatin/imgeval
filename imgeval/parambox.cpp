#include "parambox.h"
#include "datastructure/dataset.h"
#include <QtGui>
#include<iostream>
#include<vector>
using namespace std;



ParamBox::ParamBox(Dataset* data, string name,QWidget* parent)
  :QWidget(parent)
{
  this->data =data;
  this->name=name;


  QVBoxLayout* lay = new QVBoxLayout;
  lay->setSpacing(0);
  lay->setContentsMargins(0,0,0,0);
  label = new QLabel(name.c_str(),this);
  lay->addWidget(label);
//  partnr=new QLabel("00000",this);
//  label.push_back(partnr);

  QPalette pal(label->palette());
  pal.setColor(QPalette::Text,QColor(0,70,0));
  pal.setColor(QPalette::ButtonText,QColor(255,255,0));
  pal.setColor(QPalette::WindowText,QColor(0,0,55));

  label->setFont(QFont("Arial",20,QFont::Bold));
      label->setPalette(pal);
      label->setScaledContents(true);

  infos = new QTextEdit("additional \n fitting infos",this);
  infos->setFont(QFont("Arial",20,QFont::Bold));
  infos->setPalette(pal);
//  infos->setScaledContents(true);
  lay->addWidget(infos);
  setLayout(lay);
}

  //QFont f1("Arial",20,QFont::Bold);
  //QFontMetrics fm(f1);
 // int textWidthInPixels = fm.width("How many pixels wide is this text?");
  //int textHeightInPixels = fm.height();
  //--------------

void ParamBox::resizeEvent ( QResizeEvent * event )
{
  infos->setFont(QFont("Arial",width()/10,QFont::Bold));
  label->setFont(QFont("Arial",width()/7,QFont::Bold));

}


//! update the parambox from the central fitdata object
/*!
  This function takes the calculated parameters and displays
  them in the param box


  */
void ParamBox::reload()
{
//  partnr->setText((*data)[name+"Nfit"].str().c_str());
  infos->setText(("Nfit: "+(*data)[name+"Nfit"].str()+"\n"+
                  "Nsum: "+(*data)[name+"NROISum"].str()
                  ).c_str());
}

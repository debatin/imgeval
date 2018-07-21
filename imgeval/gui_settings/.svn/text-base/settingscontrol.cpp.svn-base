#include <QtGui>
#include "settingscontrol.h"
#include "datastructure/dataset.h"
//#include "filesavebox.h"
#include<iostream>
#include<sstream>
#include<map>
using namespace std;

/*! constructor of a single entry in the settingscontrol list
\author Markus Debatin
\date 10-2010
*************************************************************************/
SettingscontrolEntry::SettingscontrolEntry(map<string, m_entry>::iterator setting, QWidget* parent)
    :QWidget(parent)
{
    s=setting;
    QHBoxLayout* lay=new QHBoxLayout;
    lay->setSpacing(0);
    lay->setContentsMargins(2,0,2,0);
   QPushButton* select_this = new QPushButton("quick");
    lay->addWidget(select_this);
    connect(select_this,SIGNAL(clicked()),this,SLOT(select()));
    lay->addWidget(new QLabel(s->first.c_str()));
    ostringstream oss;
    edit= new QLineEdit();
    lay->addWidget(edit);
    connect(edit,SIGNAL(editingFinished()),this,SLOT(changeSetting()));
    /* switch((*s).second.type){
        case 'd':
            oss<<s->second.d;
             edit->setText(s->second.str().c_str());
             break;
        case 'i':
            oss<<s->second.i;
             edit->setText(oss.str().c_str());
             break;
        case 's':
             edit->setText(s->second.s.c_str());
         case'v':
            oss<<s->second.vd.size();
            for(uint i=0;i<s->second.vd.size();i++)
                oss<<"\t"<<s->second.vd[i];
             edit->setText(oss.str().c_str());
           break;
    case'w':
      oss<<s->second.vs.size()<<endl;
      for(uint i=0;i<s->second.vs.size();i++)
        oss<<s->second.vs[i]<<endl;
             edit->setText(oss.str().c_str());
      break;
    }*/
    edit->setText(s->second.str().c_str());

    this->setLayout(lay);
}

/*! called when editing is finished to store the new values in the settings container
\author Markus Debatin
\date 10-2010
*************************************************************************/
void SettingscontrolEntry::changeSetting()
{
    istringstream iss;
    iss.str(edit->text().toStdString());
  switch((*s).second.type){
        case 'd':
             iss>>s->second.d;
             break;
        case 'i':
             iss>>s->second.i;
             break;
        case 's':
             s->second.s=  edit->text().toStdString();
    }
    edit->setText(s->second.str().c_str());

}

void SettingscontrolEntry::select(){
    emit selected(QString(s->first.c_str()));

}

//! Creator for the window which display the settings
/*!
  \param setting  settings to be displayed
  \param name Name of the window
  \param parent Parent widget (if any)
\author Markus Debatin
\date 10-2010
*************************************************************************/
Settingscontrol::Settingscontrol(Dataset* setting,string name, QWidget* parent)
  :QWidget(parent)
{
    settings=setting;   //set initial values
    this->name = name;
    map<string,QVBoxLayout*> layouts;


    //initialize layouts
    for( map<char, string>::const_iterator iter = dataset_categories.begin(); iter != dataset_categories.end(); ++iter )
        layouts[iter->second]= new QVBoxLayout;
    layouts[""]= new QVBoxLayout;  //in case Elemet is empty

    for( map<string, m_entry>::iterator iter = settings->begin(); iter != settings->end(); ++iter ) {
        SettingscontrolEntry* entry = new SettingscontrolEntry(iter);
        connect(entry,SIGNAL(selected(QString)),this,SLOT(addToQuickdisplay(QString)));
        layouts[dataset_categories[iter->second.category]]->addWidget(entry);
    }

    for(map<string, QVBoxLayout* >::iterator iter = layouts.begin();iter != layouts.end(); ++iter)
    {

        iter->second->setSpacing(0);
        iter->second->setContentsMargins(0,0,0,0);
        QWidget* widget = new QWidget;
        widget->setLayout(iter->second);
        QScrollArea* scroll = new QScrollArea;
        scroll->setWidget(widget);
        this->addTab(scroll,iter->first.c_str());
    }
}

/*! ???
  \deprecated
  */
void Settingscontrol::displaySettings()
{
    QWidget * test = new QWidget();
test->show();
}

/*! adds entry to the quicklist
\param entry to be added to the quicklist
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Settingscontrol::addToQuickdisplay(QString entry)
{
    vector<string> quicklist = settings->get(name+"_quicklist").vs;
    quicklist.push_back(entry.toStdString());
    settings->store(name+"_quicklist",quicklist);
    emit changed();
}

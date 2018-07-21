#include "savemode.h"
#include "datastructure//dataset.h"
#include<QtGui>
#include"../atomimg.h"
Savemode::Savemode(Dataset* setting, QWidget* parent):
        QWidget(parent)
{
    settings = setting;
    save_img= new QCheckBox("save colour image as:");
    delete_raw = new QCheckBox("delete Raw Data");
    delete_mat = new QCheckBox("delete .mat files");
    replace_raw= new QCheckBox("replace Raw Data");
    save_results = new QCheckBox("save results");
    save_bloc = new QCheckBox("save error pattern raw");
    img_type = new QLineEdit();
    connect(save_img, SIGNAL(stateChanged(int)), this ,SLOT(commit()));
    connect(delete_raw, SIGNAL(stateChanged(int)), this ,SLOT(commit()));
    connect(delete_mat, SIGNAL(stateChanged(int)), this ,SLOT(commit()));
    connect(replace_raw, SIGNAL(stateChanged(int)), this ,SLOT(commit()));
    connect(save_results, SIGNAL(stateChanged(int)), this ,SLOT(commit()));
    connect(save_1D, SIGNAL(stateChanged(int)), this ,SLOT(commit()));
    connect(save_bloc, SIGNAL(stateChanged(int)), this ,SLOT(commit()));
    connect(img_type, SIGNAL(editingFinished()), this ,SLOT(commit()));

    int smode=settings->get("saveimg",(int)0).i;
    save_img->setChecked((smode & SAVEIMG_IMG) != 0);
    delete_raw->setChecked((smode & SAVEIMG_DELETE_RAW) != 0);
    delete_mat->setChecked((smode & SAVEIMG_DELETE_MAT) != 0);
    replace_raw->setChecked((smode & SAVEIMG_REPLACE_RAW) != 0);
    save_results->setChecked((smode & SAVEIMG_RESULTS) != 0);
    save_1D->setChecked((smode & SAVEIMG_1D) != 0);
    save_bloc->setChecked((smode & SAVEIMG_BLOC) != 0);
  //  string s = settings->get("SZ",(string) "ss").s;
  //  s=s+"d";
   // img_type->setText(s.c_str());
    //img_type->setText("bla");

    QVBoxLayout* lay = new QVBoxLayout();
    lay->addWidget(save_img);
    lay->addWidget(img_type);
    lay->addWidget(delete_raw);
    lay->addWidget(delete_mat);
    lay->addWidget(replace_raw);
    lay->addWidget(save_results);
    lay->addWidget(save_bloc);
   // lay->addWidget(save_1D);
    setLayout(lay);
}


void Savemode::commit()
{
    int smode=0;
    if(save_img->isChecked()) smode |= SAVEIMG_IMG ;
    if(delete_raw->isChecked()) smode |= SAVEIMG_DELETE_RAW ;
    if(delete_raw->isChecked()) smode |= SAVEIMG_DELETE_MAT ;
    if(replace_raw->isChecked()) smode |= SAVEIMG_REPLACE_RAW ;
    if(save_results->isChecked()) smode |= SAVEIMG_RESULTS ;
    if(save_bloc->isChecked()) smode |= SAVEIMG_BLOC ;
    settings->store("saveimg",smode);
    //    QLineEdit* img_type;
}

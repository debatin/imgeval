#include "interpreter_gui.h"
#include"datastructure/dataset.h"
#include<QtGui>
#include "datastructure/numeric_templates.h"
#include "mainwindow.h"
InterpreterGui::InterpreterGui(Dataset *settings, MainWindow* mainptr, QWidget *parent)
    :QWidget(parent)
{
    this->settings = settings;
    mainwindow = mainptr;
    QVBoxLayout* lay = new QVBoxLayout;
    run_b = new QPushButton("run");
    connect(run_b,SIGNAL(pressed()),this,SLOT(run()));
    lay->addWidget(run_b);
    available_cmd = new QComboBox;
    vector<string> avail_cmds;
    avail_cmds=settings->get("commandlist",avail_cmds).vs;
    for(unsigned int i = 0;i<avail_cmds.size();i++) available_cmd->addItem(avail_cmds[i].c_str());
    lay->addWidget(available_cmd);
    edit = new QTextEdit;
    lay->addWidget(edit);
    setLayout(lay);
}


void InterpreterGui::run()
{
    QStringList list=edit->toPlainText().split("\n");
    vector<string> commands;
    for(uint i = 0;i<list.size();i++) commands.push_back(list[i].toStdString());
    for(uint i = 0;i<commands.size();i++) cout<<commands[i]<<"\n";
    if(mainwindow != NULL) mainwindow->imgcommand_interpreter(commands);
}

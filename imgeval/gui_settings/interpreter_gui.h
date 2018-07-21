#ifndef INTERPRETER_GUI_H
#define INTERPRETER_GUI_H
#include<QWidget>
class Dataset;
class QPushButton;
class QTextEdit;
class QComboBox;
class MainWindow;
class InterpreterGui: public QWidget
{
    Q_OBJECT
public:
    InterpreterGui(Dataset* settings,MainWindow* mainptr, QWidget* parent =0);
public slots:
    void run();  ///< run the script
private:
    Dataset* settings;
    QPushButton* run_b;
    QTextEdit* edit;
    QComboBox* available_cmd;
    MainWindow* mainwindow;
};


#endif // INTERPRETER_GUI_H

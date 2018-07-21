#ifndef FILESAVEBOX
#define FILESAVEBOX
#include <QWidget>
#include <QString>
#include<iostream>
#include<QDir>
class Dataset;
class QLineEdit;
class FileSaveBox: public QWidget
{
Q_OBJECT
  public:
  FileSaveBox(Dataset* settings, QWidget* parent = 0);
  public slots:
  void selectFile();
  void toggleSavemode(int);
  void showExtendedDialog();   ///< show dialog for extended settings
  void killLastLine();   ///< removes Last Line from output file
 private:
  Dataset* settings;
  QLineEdit* ddir;
  QString dir;
};



#endif

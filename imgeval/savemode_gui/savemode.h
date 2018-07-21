#ifndef SAVEMODE_H
#define SAVEMODE_H
#include<QWidget>
class Dataset;
class QCheckBox;
class QLineEdit;
class Savemode: public QWidget
{
    Q_OBJECT
public:
    Savemode(Dataset* setting, QWidget* parent =0);
public slots:
    void commit();
private:
    Dataset* settings;
    QCheckBox* save_img;
    QCheckBox* delete_raw;
    QCheckBox* delete_mat;
    QCheckBox* replace_raw;
    QCheckBox* save_results;
    QCheckBox* save_bloc;
    QLineEdit* img_type;
};
#endif // SAVEMODE_H

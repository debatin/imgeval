/********************************************************************************
** Form generated from reading ui file 'savemode.ui'
**
** Created: Wed 9. Dec 13:11:09 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SAVEMODE_H
#define UI_SAVEMODE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_savemode
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QCheckBox *save_image;
    QLineEdit *lineEdit;
    QCheckBox *save_results;
    QCheckBox *delete_raw;
    QCheckBox *replace_raw;
    QLabel *label;
    QPushButton *pushButton;

    void setupUi(QDialog *savemode)
    {
        if (savemode->objectName().isEmpty())
            savemode->setObjectName(QString::fromUtf8("savemode"));
        savemode->resize(327, 215);
        verticalLayoutWidget = new QWidget(savemode);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(80, 40, 202, 118));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        save_image = new QCheckBox(verticalLayoutWidget);
        save_image->setObjectName(QString::fromUtf8("save_image"));

        verticalLayout->addWidget(save_image);

        lineEdit = new QLineEdit(verticalLayoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        save_results = new QCheckBox(verticalLayoutWidget);
        save_results->setObjectName(QString::fromUtf8("save_results"));

        verticalLayout->addWidget(save_results);

        delete_raw = new QCheckBox(verticalLayoutWidget);
        delete_raw->setObjectName(QString::fromUtf8("delete_raw"));

        verticalLayout->addWidget(delete_raw);

        replace_raw = new QCheckBox(verticalLayoutWidget);
        replace_raw->setObjectName(QString::fromUtf8("replace_raw"));

        verticalLayout->addWidget(replace_raw);

        label = new QLabel(savemode);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 10, 151, 16));
        QFont font;
        font.setFamily(QString::fromUtf8("MV Boli"));
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        pushButton = new QPushButton(savemode);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(230, 170, 75, 23));

        retranslateUi(savemode);

        QMetaObject::connectSlotsByName(savemode);
    } // setupUi

    void retranslateUi(QDialog *savemode)
    {
        savemode->setWindowTitle(QApplication::translate("savemode", "Dialog", 0, QApplication::UnicodeUTF8));
        save_image->setText(QApplication::translate("savemode", "save image as :", 0, QApplication::UnicodeUTF8));
        save_results->setText(QApplication::translate("savemode", "save results with each measurement", 0, QApplication::UnicodeUTF8));
        delete_raw->setText(QApplication::translate("savemode", "delete raw data", 0, QApplication::UnicodeUTF8));
        replace_raw->setText(QApplication::translate("savemode", "replace raw data", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("savemode", "Save Modes", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("savemode", "submit", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(savemode);
    } // retranslateUi

};

namespace Ui {
    class savemode: public Ui_savemode {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEMODE_H

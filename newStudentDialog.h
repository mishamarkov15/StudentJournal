//
// Created by Михаил Марков on 01/06/2023.
//

#ifndef TABLEPROJECT_NEWSTUDENTDIALOG_H
#define TABLEPROJECT_NEWSTUDENTDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <QMap>
#include <QErrorMessage>
#include <QString>
#include <QTableWidget>
#include <iostream>
#include <fstream>

class NewStudentDialog : public QDialog {
    Q_OBJECT;
public:
    explicit NewStudentDialog(QString  filepathToCSV, QTableWidget* parentTable, QWidget* parent = nullptr);

    void initLayout();

    void initConnections();

private slots:
    void writeToCSV();

    void closeWindowByRequest();

private:
    QTableWidget* parentTable;
    QGridLayout *layout;
    QPushButton *addButton;
    QPushButton *cancelButton;
    QLabel *title;
    std::string filepath;
    QMap<QString, QWidget*> editFields;
};


#endif //TABLEPROJECT_NEWSTUDENTDIALOG_H

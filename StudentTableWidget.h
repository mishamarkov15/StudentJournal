//
// Created by Михаил Марков on 01/06/2023.
//

#ifndef TABLEPROJECT_STUDENTTABLEWIDGET_H
#define TABLEPROJECT_STUDENTTABLEWIDGET_H

#include <QString>
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QTableWidget>
#include <fstream>
#include <QFileDialog>
#include <iostream>
#include <QDir>
#include <QHeaderView>

#include "utils/csv.h"
#include "TableAbstractWidget.h"
#include "newStudentDialog.h"

class StudentTableWidget : public QWidget, public TableAbstractWidget {
Q_OBJECT;
public:
    explicit StudentTableWidget(QWidget *parent = nullptr);

    void initLayout();

    void initConnect();

    void initTable(const QString &filepath = filepathToStudents);

    void saveAs() override;

    void reloadTableData(const QString &filepath = filepathToStudents);

private slots:

    void addNewStudent();

    void deleteRow();

    void activateDeleteRowButton();

    void autoSave();


private:

    QGridLayout *layout;
    QPushButton *addButton;
    QPushButton *deleteRowButton;
    QTableWidget *tableWidget;
};


#endif //TABLEPROJECT_STUDENTTABLEWIDGET_H

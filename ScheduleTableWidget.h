//
// Created by Михаил Марков on 01/06/2023.
//

#ifndef TABLEPROJECT_SCHEDULETABLEWIDGET_H
#define TABLEPROJECT_SCHEDULETABLEWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QTableWidget>
#include <fstream>
#include <iostream>
#include <QFileDialog>
#include <QString>
#include <QHeaderView>
#include <sstream>

#include "utils/csv.h"
#include "TableAbstractWidget.h"
#include "StudentTableWidget.h"
#include "NewScheduleDialog.h"

class ScheduleTableWidget : public QWidget, public TableAbstractWidget {
Q_OBJECT;
public:
    explicit ScheduleTableWidget(StudentTableWidget* studentTable, QWidget* parent = nullptr);

    void initLayout();

    void initConnect();

    void initTable(const QString& filepath = filepathToSchedule);

    void saveAs() override;


private slots:
    void showOnlyOnePersonLessons();

    void activateShowOnlyButton();

    void addNewRecord();

    void deleteRow();

    void activateDeleteRowButton();

    void updatePaidUnpaidStatus();

    void autoSave();

    void resetFiltersOnTable();

    void showOnlyPastLessons();

    void showOnlyFutureLessons();

private:
    bool filtered;
    StudentTableWidget* studentTable;

    QGridLayout *layout;
    QPushButton *addButton;
    QPushButton *deleteRowButton;
    QPushButton *showOnlyHisHerLessons;
    QPushButton *resetFilters;
    QPushButton *onlyPlannedLessonsBtn;
    QPushButton *onlyPastLessonsBtn;
    QTableWidget *tableWidget;

    void updateFile(const QString& name, bool increase);
};

// Check if dateString less than current time
bool dateLessThanToday(const QString& dateString, const QString& formatDate);

// Check if dateString greater or equal than current time
bool dateGreaterOrEqualThanToday(const QString& dateString, const QString& formatDate);

#endif //TABLEPROJECT_SCHEDULETABLEWIDGET_H

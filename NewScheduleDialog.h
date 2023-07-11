//
// Created by Михаил Марков on 01/06/2023.
//

#ifndef TABLEPROJECT_NEWSCHEDULEDIALOG_H
#define TABLEPROJECT_NEWSCHEDULEDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <QMap>
#include <QErrorMessage>
#include <iostream>
#include <QString>
#include <QCalendarWidget>
#include <fstream>
#include <QTableWidget>
#include <QVector>
#include <QRadioButton>
#include <QComboBox>
#include <QFile>

class NewScheduleDialog : public QDialog {
Q_OBJECT;
public:
    explicit NewScheduleDialog(QString  filepathToCSV, QTableWidget *parentTable, QWidget* parent = nullptr);

    void initLayout();

    void initConnections();

private slots:
    void writeToCSV();

    void closeWindowByRequest();

private:
    [[nodiscard]] std::pair<bool, QString> checkFieldsBeforeWriting() const;
    bool showErrorMsg();
    bool writeFile(const QStringList& list);
    QTableWidget* parentTable;
    QGridLayout *layout;
    QPushButton *addButton;
    QPushButton *cancelButton;
    QLabel *title;
    QString filepath;
    QMap<QString, QWidget*> editFields;
};


#endif //TABLEPROJECT_NEWSCHEDULEDIALOG_H

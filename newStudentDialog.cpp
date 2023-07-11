//
// Created by Михаил Марков on 01/06/2023.
//

#include "newStudentDialog.h"

#include <utility>

NewStudentDialog::NewStudentDialog(QString filepathToCSV, QTableWidget* parentTable, QWidget *parent) : QDialog(parent),
                                                                                 layout(new QGridLayout()),
                                                                                 parentTable(parentTable),
                                                                                 filepath(std::move(filepathToCSV.toStdString())),
                                                                                 addButton(new QPushButton("Add")),
                                                                                 cancelButton(
                                                                                         new QPushButton("Cancel")),
                                                                                 title(new QLabel(
                                                                                         "Provide info about student.")) {
    initLayout();
    initConnections();
}

void NewStudentDialog::initLayout() {
    auto *label_name = new QLabel("Input name");
    auto *label_grade = new QLabel("Select grade");
    auto *label_lessons = new QLabel("Input amount of unpaid lessons");
    auto *label_price = new QLabel("Input price per lesson ($)");

    editFields["name"] = new QLineEdit("Ivan Ivanov");
    editFields["grade"] = new QLineEdit("11");
    editFields["lessons"] = new QLineEdit("");
    editFields["price"] = new QLineEdit("");


    layout->addWidget(title, 0, 1, 1, 2);

    // Labels
    layout->addWidget(label_name, 1, 0, 1, 2);
    layout->addWidget(label_grade, 2, 0, 1, 2);
    layout->addWidget(label_lessons, 3, 0, 1, 2);
    layout->addWidget(label_price, 4, 0, 1, 2);

    // Inputs widgets
    layout->addWidget(editFields["name"], 1, 2, 1, 2);
    layout->addWidget(editFields["grade"], 2, 2, 1, 2);
    layout->addWidget(editFields["lessons"], 3, 2, 1, 2);
    layout->addWidget(editFields["price"], 4, 2, 1, 2);

    layout->addWidget(addButton, 5, 3, 1, 1);
    layout->addWidget(cancelButton, 5, 2, 1, 1);

    setLayout(layout);
}

void NewStudentDialog::initConnections() {
    connect(addButton, &QPushButton::clicked, this, &NewStudentDialog::writeToCSV);
    connect(cancelButton, &QPushButton::clicked, this, &NewStudentDialog::closeWindowByRequest);
}

void NewStudentDialog::writeToCSV() {
    // Тут будет проверка на корректность ввода
    auto name = dynamic_cast<QLineEdit *>(editFields["name"])->text();
    auto grade = dynamic_cast<QLineEdit *>(editFields["grade"])->text();
    auto lessons = dynamic_cast<QLineEdit *>(editFields["lessons"])->text();
    auto price = dynamic_cast<QLineEdit *>(editFields["price"])->text();

    if (!name.isEmpty() && !grade.isEmpty() && !lessons.isEmpty() && !price.isEmpty()) {
        QDebug d(QtMsgType::QtInfoMsg);
        d << "Adding: " << name << ' ' << grade << ' ' << lessons << ' ' << price << '\n';
        std::ofstream fout(filepath, std::ios::app | std::ios::out);
        if (!fout.is_open()) {
            auto *msg = new QErrorMessage(this);
            msg->setWindowTitle("Error");
            msg->showMessage("File can not be opened. Try later.");
            delete msg;
            return;
        }

        fout << '\n';
        fout << parentTable->rowCount() << ',' << name.toStdString() << ',' << grade.toStdString() << ',' <<
             lessons.toStdString() << ',' << price.toStdString();
        fout.close();
        d << "Successfully added.\n";

        parentTable->insertRow(parentTable->rowCount());
        QVector<QString> dataToAdd;
        dataToAdd << QString(std::to_string(parentTable->rowCount() - 1).c_str()) << name << grade << lessons << price;
        int j = 0;
        for (const auto &data: dataToAdd) {
            parentTable->setItem(parentTable->rowCount() - 1, j++, new QTableWidgetItem(data));
        }
    } else {
        QDebug d(QtMsgType::QtCriticalMsg);
        d << "Some fields aren't provided. So it is unable to write new record.\n";
    }
    close();
}

void NewStudentDialog::closeWindowByRequest() {
    close();
}

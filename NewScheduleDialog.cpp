//
// Created by Михаил Марков on 01/06/2023.
//

#include "NewScheduleDialog.h"

#include <utility>

NewScheduleDialog::NewScheduleDialog(QString filepathToCSV, QTableWidget *parentTable, QWidget *parent) :
        QDialog(parent),
        parentTable(parentTable),
        layout(new QGridLayout()),
        filepath(std::move(filepathToCSV)),
        addButton(new QPushButton("Add")),
        cancelButton(new QPushButton("Cancel")),
        title(new QLabel("Provide info about schedule record.")) {
    initLayout();
    initConnections();
}

void NewScheduleDialog::initLayout() {
    auto *label_calendar = new QLabel("Select date");
    auto *label_name = new QLabel("Input student's name");
    auto *label_paid = new QLabel("Paid or unpaid");
    auto *label_homework_link = new QLabel("Link to homework");
    auto *label_homework_done = new QLabel("Hw done / undone");

    // Applying styles to labels
    title->setWordWrap(true);
    title->setAlignment(Qt::AlignCenter);
    QFont font = title->font();
    font.setPointSize(16);
    font.setBold(true);
    title->setFont(font);

    font = label_calendar->font();
    font.setBold(true);
    label_calendar->setFont(font);

    font = label_name->font();
    font.setBold(true);
    label_name->setFont(font);

    font = label_paid->font();
    font.setBold(true);
    label_paid->setFont(font);

    font = label_homework_link->font();
    font.setBold(true);
    label_homework_link->setFont(font);

    font = label_homework_done->font();
    font.setBold(true);
    label_homework_done->setFont(font);

    auto *name = new QLineEdit();
    name->setPlaceholderText("Name here...");
    editFields["name"] = name;

    auto *link = new QLineEdit();
    link->setPlaceholderText("Link to hw...");
    editFields["homework_link"] = link;

    editFields["calendar"] = new QCalendarWidget();
    auto* hw_done = new QComboBox();
    hw_done->addItem("Done");
    hw_done->addItem("Undone");
    editFields["homework_done"] = hw_done;
    auto* paid = new QComboBox();
    paid->addItem("Paid");
    paid->addItem("Unpaid");
    editFields["paid"] = paid;


    layout->addWidget(title, 0, 0, 1, 6);

    // Labels
    layout->addWidget(label_name, 2, 0, 1, 2);
    layout->addWidget(label_homework_link, 4, 0, 1, 2);
    layout->addWidget(label_homework_done, 6, 0, 1, 2);
    layout->addWidget(label_paid, 8, 0, 1, 2);
    layout->addWidget(label_calendar, 2, 3, 1, 2);

    // Inputs widgets
    layout->addWidget(editFields["name"], 3, 0, 1, 2);
    layout->addWidget(editFields["homework_link"], 5, 0, 1, 2);
    layout->addWidget(editFields["homework_done"], 7, 0, 1, 2);
    layout->addWidget(editFields["paid"], 9, 0, 1, 2);
    layout->addWidget(editFields["calendar"], 3, 2, 7, 4);

    layout->addWidget(addButton, 10, 5, 1, 1);
    layout->addWidget(cancelButton, 10, 4, 1, 1);

    setLayout(layout);
}

void NewScheduleDialog::initConnections() {
    connect(addButton, &QPushButton::clicked, this, &NewScheduleDialog::writeToCSV);
    connect(cancelButton, &QPushButton::clicked, this, &NewScheduleDialog::closeWindowByRequest);
}

void NewScheduleDialog::writeToCSV() {
    if (showErrorMsg()) {
        return;
    }

    auto name = dynamic_cast<QLineEdit *>(editFields["name"])->text();
    auto homework_link = dynamic_cast<QLineEdit *>(editFields["homework_link"])->text();
    auto date = dynamic_cast<QCalendarWidget *>(editFields["calendar"])->selectedDate().toString("d-M-yyyy");
    QString homework_done = (dynamic_cast<QComboBox *>(editFields["homework_done"])->currentText() == "Done" ? "done" : "undone");
    QString paid = (dynamic_cast<QComboBox *>(editFields["paid"])->currentText() == "Paid" ? "1" : "0");

    if (!name.isEmpty() && !homework_link.isEmpty() && !date.isEmpty()) {
        QDebug d(QtMsgType::QtInfoMsg);
        d << "Adding: " << date << ' ' << name << ' ' << paid << ' ' << homework_link << ' ' << homework_done << '\n';

        QStringList dataToWrite = {QString::number(parentTable->rowCount()), date, name, paid, homework_link, homework_done};
        writeFile(dataToWrite);

        d << "Successfully added.\n";

        parentTable->insertRow(parentTable->rowCount());
        QVector<QString> dataToAdd;
        dataToAdd << QString(std::to_string(parentTable->rowCount() - 1).c_str()) << date << name << paid
                  << homework_link << homework_done;
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

void NewScheduleDialog::closeWindowByRequest() {
    close();
}

std::pair<bool, QString> NewScheduleDialog::checkFieldsBeforeWriting() const {
    auto name = dynamic_cast<QLineEdit *>(editFields["name"])->text();
    auto homework_link = dynamic_cast<QLineEdit *>(editFields["homework_link"])->text();
    auto date = dynamic_cast<QCalendarWidget *>(editFields["calendar"])->selectedDate().toString("d-M-yyyy");

    if (name.isEmpty()) {
        return {false, "Name"};
    }
    if (homework_link.isEmpty()) {
        return {false, "HomeWork Link"};
    }
    if (date.isEmpty()) {
        return {false, "Date"};
    }
    return {true, ""};
}

bool NewScheduleDialog::showErrorMsg() {
    auto [errors, field] = checkFieldsBeforeWriting();
    if (!errors) {
        QDebug d(QtMsgType::QtWarningMsg);
        d << "You must provide " << field << " field\n";
        QErrorMessage msg(this);
        msg.showMessage("You must provide " + field + " field.");
        msg.exec();
        return true;
    }
    return false;
}

bool NewScheduleDialog::writeFile(const QStringList& list) {
    QFile file(filepath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Append)) {
        auto *msg = new QErrorMessage(this);
        msg->setWindowTitle("Error");
        msg->showMessage("File can not be opened. Try later.");
        delete msg;
        return false;
    }
    QTextStream fout(&file);
    fout << list.join(",");
    file.close();
    return true;
}


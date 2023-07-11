//
// Created by Михаил Марков on 01/06/2023.
//

#include "ScheduleTableWidget.h"

ScheduleTableWidget::ScheduleTableWidget(StudentTableWidget *studentTableWidget,
                                         QWidget *parent) : QWidget(parent),
                                                            studentTable(studentTableWidget),
                                                            layout(new QGridLayout()),
                                                            addButton(new QPushButton("Add record")),
                                                            deleteRowButton(new QPushButton("Delete record")),
                                                            tableWidget(new QTableWidget()),
                                                            showOnlyHisHerLessons(new QPushButton("Show his/her lessons")),
                                                            resetFilters(new QPushButton("Reset filters")),
                                                            onlyPastLessonsBtn(new QPushButton("Show only past lessons")),
                                                            onlyPlannedLessonsBtn(new QPushButton("Show only future lessons")),
                                                            filtered(false) {
    initLayout();
    initTable();
    initConnect();
}

void ScheduleTableWidget::initLayout() {
    deleteRowButton->setDisabled(true);
    showOnlyHisHerLessons->setDisabled(true);

    layout->addWidget(tableWidget, 0, 0, 6, 6);
    layout->addWidget(addButton, 0, 6, 1, 1);
    layout->addWidget(showOnlyHisHerLessons, 1, 6, 1, 1);
    layout->addWidget(onlyPlannedLessonsBtn, 2, 6, 1, 1);
    layout->addWidget(onlyPastLessonsBtn, 3, 6, 1, 1);
    layout->addWidget(resetFilters, 4, 6, 1, 1);
    layout->addWidget(deleteRowButton, 5, 6, 1, 1);

    setLayout(layout);
}

void ScheduleTableWidget::initConnect() {
    connect(addButton, &QPushButton::clicked, this, &ScheduleTableWidget::addNewRecord);
    connect(deleteRowButton, &QPushButton::clicked, this, &ScheduleTableWidget::deleteRow);
    connect(tableWidget, &QTableWidget::itemChanged, this, &ScheduleTableWidget::autoSave);
    connect(tableWidget, &QTableWidget::itemChanged, this, &ScheduleTableWidget::updatePaidUnpaidStatus);

    // Activate Buttons
    connect(tableWidget, &QTableWidget::itemClicked, this, &ScheduleTableWidget::activateDeleteRowButton);
    connect(tableWidget->verticalHeader(), &QHeaderView::sectionPressed, this,
            &ScheduleTableWidget::activateDeleteRowButton);

    connect(tableWidget, &QTableWidget::itemClicked, this, &ScheduleTableWidget::activateShowOnlyButton);
    connect(tableWidget->verticalHeader(), &QHeaderView::sectionPressed, this,
            &ScheduleTableWidget::activateShowOnlyButton);

    connect(showOnlyHisHerLessons, &QPushButton::clicked, this, &ScheduleTableWidget::showOnlyOnePersonLessons);
    connect(resetFilters, &QPushButton::clicked, this, &ScheduleTableWidget::resetFiltersOnTable);
    connect(onlyPastLessonsBtn, &QPushButton::clicked, this, &ScheduleTableWidget::showOnlyPastLessons);
    connect(onlyPlannedLessonsBtn, &QPushButton::clicked, this, &ScheduleTableWidget::showOnlyFutureLessons);
}

void ScheduleTableWidget::addNewRecord() {
    auto *dialog = new NewScheduleDialog(filepathToSchedule, tableWidget, this);
    dialog->show();
}

void ScheduleTableWidget::initTable(const QString &filepath) {
    std::ifstream fin(filepath.toStdString());
    if (!fin.is_open()) {
        QDebug d(QtMsgType::QtCriticalMsg);
        d << "File can not be opened. (ScheduleTableWidget)\n";
        return;
    }
    size_t current_row = 0;
    CSVRow row;
    while (row.readNextRow(fin) != EOF) {
        tableWidget->insertRow(tableWidget->rowCount());
        if (current_row == 0) {
            for (size_t i = 0; i < row.size(); ++i) {
                tableWidget->insertColumn(tableWidget->columnCount());
            }
        }
        for (size_t i = 0; i < row.size(); ++i) {
            tableWidget->setItem(current_row, i, new QTableWidgetItem(std::string(row[i]).c_str()));
        }
        ++current_row;
    }
    fin.close();

}

void ScheduleTableWidget::autoSave() {
    updateCSV(filepathToSchedule, tableWidget->currentRow(), tableWidget->currentColumn(),
              tableWidget->currentItem()->text());
}

void ScheduleTableWidget::saveAs() {
    QByteArray rawData;
    for (int i = 0; i < tableWidget->rowCount(); ++i) {
        QString line;
        for (int j = 0; j < tableWidget->rowCount(); ++j) {
            line += (j > 0 ? "," : "") + tableWidget->item(i, j)->text();
        }
        line += '\n';
        rawData.push_back(QByteArray(line.toStdString().c_str(), line.length()));
    }
    QFileDialog::saveFileContent(rawData, "result.csv");
}

void ScheduleTableWidget::deleteRow() {
    deleteCSVRow(filepathToSchedule, tableWidget->currentRow());
    tableWidget->removeRow(tableWidget->currentRow());
    deleteRowButton->setDisabled(true);
}

void ScheduleTableWidget::activateDeleteRowButton() {
    deleteRowButton->setDisabled(false);
}

void ScheduleTableWidget::updatePaidUnpaidStatus() {
    if (tableWidget->currentColumn() != 3) {
        return;
    }
    auto name = tableWidget->item(tableWidget->currentRow(), 2)->text();
    auto increase = tableWidget->item(tableWidget->currentRow(), tableWidget->currentColumn())->text() == "0";
    updateFile(name, increase);
    studentTable->reloadTableData(filepathToStudents);
}

// Изменяем файл со студентами
void ScheduleTableWidget::updateFile(const QString &name, bool increase) {
    std::ifstream fin(filepathToStudents.toStdString());
    if (!fin.is_open()) {
        QDebug d(QtMsgType::QtCriticalMsg);
        d << "Can not open " << filepathToSchedule << " file\n";
        return;
    }
    CSVRow row;

    QFile tmp("tmp.txt");
    if (!tmp.open(QIODevice::WriteOnly)) {
        QDebug d(QtMsgType::QtCriticalMsg);
        d << "Can not open tmp.txt file\n";
        return;
    }
    QTextStream fout(&tmp);

    row.readNextRow(fin);
    QStringList data;
    for (size_t i = 0; i < row.size(); ++i) {
        data << QString(std::string(row[i]).c_str());
    }
    fout << data.join(",");

    while (row.readNextRow(fin) != EOF) {
        QStringList dataToWrite;
        dataToWrite << QString(std::string(row[0]).c_str())
                    << QString(std::string(row[1]).c_str())
                    << QString(std::string(row[2]).c_str());
        if (row[1] == name.toStdString()) {
            auto amount_of_unpaid_lessons = std::atoi(std::string(row[3]).c_str());
            if (increase) {
                ++amount_of_unpaid_lessons;
            } else {
                --amount_of_unpaid_lessons;
            }
            dataToWrite << QString::number(amount_of_unpaid_lessons);
        } else {
            dataToWrite << QString(std::string(row[3]).c_str());
        }
        dataToWrite << QString(std::string(row[4]).c_str());
        fout << '\n' << dataToWrite.join(",");
    }
    fin.close();
    tmp.close();

    // Теперь обновляем исходный файл
    QFile dest(filepathToStudents), src("tmp.txt");
    if (!dest.open(QIODevice::WriteOnly)) {
        QDebug d(QtMsgType::QtCriticalMsg);
        d << "Can not open " << filepathToSchedule << " file\n";
        return;
    }
    if (!src.open(QIODevice::ReadOnly)) {
        QDebug d(QtMsgType::QtCriticalMsg);
        d << "Can not open tmp.txt file\n";
        return;
    }

    QTextStream destOut(&dest), srcIn(&src);
    destOut << srcIn.readAll();
    dest.close();
    src.close();
}

void ScheduleTableWidget::showOnlyOnePersonLessons() {
    auto name = tableWidget->item(tableWidget->currentRow(), 2)->text();
    for (int i = 1; i < tableWidget->rowCount(); ++i) {
        if (tableWidget->item(i, 2)->text() != name) {
            tableWidget->setRowHidden(i, true);
        }
    }
    tableWidget->clearSelection();
    showOnlyHisHerLessons->setDisabled(true);
    filtered = true;
}

void ScheduleTableWidget::activateShowOnlyButton() {
    showOnlyHisHerLessons->setDisabled(false);
}

void ScheduleTableWidget::resetFiltersOnTable() {
    for (int i = 1; i < tableWidget->rowCount(); ++i) {
        tableWidget->setRowHidden(i, false);
    }
    filtered = false;
    onlyPastLessonsBtn->setDisabled(false);
    onlyPlannedLessonsBtn->setDisabled(false);
}

void ScheduleTableWidget::showOnlyPastLessons() {
    for (int i = 1; i < tableWidget->rowCount(); ++i) {
        tableWidget->setRowHidden(i, false);
    }
    for (int i = 1; i < tableWidget->rowCount(); ++i) {
        if (dateGreaterOrEqualThanToday(tableWidget->item(i, 1)->text(), "%d-%m-%Y")) {
            tableWidget->setRowHidden(i, true);
        }
    }
    onlyPastLessonsBtn->setDisabled(true);
    onlyPlannedLessonsBtn->setDisabled(false);
    filtered = true;
}

void ScheduleTableWidget::showOnlyFutureLessons() {
    for (int i = 1; i < tableWidget->rowCount(); ++i) {
        tableWidget->setRowHidden(i, false);
    }
    for (int i = 1; i < tableWidget->rowCount(); ++i) {
        if (dateLessThanToday(tableWidget->item(i, 1)->text(), "%d-%m-%Y")) {
            tableWidget->setRowHidden(i, true);
        }
    }
    onlyPastLessonsBtn->setDisabled(false);
    onlyPlannedLessonsBtn->setDisabled(true);
    filtered = true;
}

bool dateLessThanToday(const QString& dateString, const QString& formatDate) {
    std::tm t{};
    std::istringstream ss(dateString.toStdString());
    ss >> std::get_time(&t, formatDate.toStdString().c_str());
    if (ss.fail()) {
        QDebug err(QtMsgType::QtFatalMsg);
        err << "Can not parse " << dateString << " to format: " << formatDate << '\n';
        return false;
    }
    std::time_t time_stamp = mktime(&t);
    return time_stamp < std::time(nullptr);
}

bool dateGreaterOrEqualThanToday(const QString& dateString, const QString& formatDate) {
    return !dateLessThanToday(dateString, formatDate);
}
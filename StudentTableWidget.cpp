//
// Created by Михаил Марков on 01/06/2023.
//

#include "StudentTableWidget.h"

StudentTableWidget::StudentTableWidget(QWidget *parent) : QWidget(parent), layout(new QGridLayout()),
                                                          addButton(new QPushButton("Add student")),
                                                          deleteRowButton(new QPushButton("Delete student")),
                                                          tableWidget(new QTableWidget()) {
//    tableWidget->horizontalHeader() TODO: Resize last column
    initLayout();
    initTable();
    initConnect();
}

void StudentTableWidget::initLayout() {
    deleteRowButton->setDisabled(true);

    layout->addWidget(tableWidget, 0, 0, 6, 6);
    layout->addWidget(addButton, 0, 6, 1, 1);
    layout->addWidget(deleteRowButton, 1, 6, 1, 1);
    setLayout(layout);
}

void StudentTableWidget::initConnect() {
    connect(addButton, &QPushButton::clicked, this, &StudentTableWidget::addNewStudent);
    connect(deleteRowButton, &QPushButton::clicked, this, &StudentTableWidget::deleteRow);
    connect(tableWidget, &QTableWidget::itemChanged, this, &StudentTableWidget::autoSave);
    connect(tableWidget, &QTableWidget::itemClicked, this, &StudentTableWidget::activateDeleteRowButton);
    connect(tableWidget->verticalHeader(), &QHeaderView::sectionPressed, this, &StudentTableWidget::activateDeleteRowButton);
}

void StudentTableWidget::addNewStudent() {
    auto *dialog = new NewStudentDialog(filepathToStudents, tableWidget, this);
    dialog->show();
}

void StudentTableWidget::initTable(const QString &filepath) {
    std::ifstream fin(filepath.toStdString());
    if (!fin.is_open()) {
        std::cout << "File can not be opened.\n";
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

void StudentTableWidget::autoSave() {
    updateCSV(filepathToStudents, tableWidget->currentRow(), tableWidget->currentColumn(),
              tableWidget->currentItem()->text());
}

void StudentTableWidget::saveAs() {
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

void StudentTableWidget::deleteRow() {
    deleteCSVRow(filepathToStudents, tableWidget->currentRow());
    tableWidget->removeRow(tableWidget->currentRow());
    deleteRowButton->setDisabled(true);
}

void StudentTableWidget::activateDeleteRowButton() {
    deleteRowButton->setDisabled(false);
}

void StudentTableWidget::reloadTableData(const QString &filepath) {
    std::ifstream fin(filepath.toStdString());
    if (!fin.is_open()) {
        QDebug d(QtMsgType::QtCriticalMsg);
        d << "File " << filepath << " can not be opened.\n";
        return;
    }
    size_t current_row = 0;
    CSVRow row;
    while (row.readNextRow(fin) != EOF) {
        for (size_t i = 0; i < row.size(); ++i) {
            tableWidget->setItem(current_row, i, new QTableWidgetItem(std::string(row[i]).c_str()));
        }
        ++current_row;
    }
    fin.close();

}

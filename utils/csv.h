//
// Created by Михаил Марков on 01/06/2023.
//

#ifndef TABLEPROJECT_CSV_H
#define TABLEPROJECT_CSV_H

#include <iostream>
#include <fstream>
#include <QString>
#include <vector>

const QString filepathToStudents = "/Users/mikhaiil/CLionProjects/tableProject/data/students.csv";
const QString filepathToSchedule = "/Users/mikhaiil/CLionProjects/tableProject/data/schedule.csv";

class CSVRow {
public:
    std::string_view operator[](size_t index) const;

    size_t size() const;

    int readNextRow(std::istream &in);

private:
    std::string _line;
    std::vector<int> _data;
};

// id - индекс строки
// column_name - название колонки
// Например, (1, name) - поменяет имя первого студента
void updateCSV(const QString& filepath, int row, int col, const QString& data_to_update);

void deleteCSVRow(const QString& filepath, int row);


#endif //TABLEPROJECT_CSV_H

//
// Created by Михаил Марков on 01/06/2023.
//

#include "csv.h"

std::string_view CSVRow::operator[](size_t index) const {
    return std::string_view(&_line[_data[index] + 1], _data[index + 1] - (_data[index] + 1));
}

int CSVRow::readNextRow(std::istream &in) {
    std::getline(in, _line);
    if (!in) {
        return EOF;
    }
    _data.clear();
    _data.emplace_back(-1);
    std::string::size_type pos = 0;
    while ((pos = _line.find(',', pos)) != std::string::npos) {
        _data.emplace_back(pos);
        ++pos;
    }
    pos = _line.size();
    _data.emplace_back(pos);
    return 0;
}

size_t CSVRow::size() const {
    return _data.size() - 1;
}

std::fstream& operator<<(std::fstream& fout, const CSVRow& row) {
    for (int i = 0; i < row.size(); ++i) {
        fout << (i > 0 ? "," : "") << row[i];
    }
    fout << '\n';
    return fout;
}

void updateCSV(const QString& filepath, int rowInd, int colInd, const QString& data_to_update) {
    CSVRow row;
    std::fstream fin(filepath.toStdString(), std::ios::in), fout("tmp.csv", std::ios::out);

    int current_row = 0;
    while (row.readNextRow(fin) != EOF) {
        if (current_row++ == rowInd) {
            for (int i = 0; i < row.size(); ++i) {
                if (i > 0) fout << ',';
                if (i == colInd) {
                    fout << data_to_update.toStdString();
                    std::cout << "Found cell to update...\n";
                } else {
                    fout << row[i];
                }
            }
            fout << '\n';
        } else {
            fout << row;
        }
    }
    fin.close();
    fout.close();
    std::cout << "Success while saving...\n";
    fout.open(filepath.toStdString(), std::ios::out);
    fin.open("tmp.csv", std::ios::in);
    while (row.readNextRow(fin) != EOF) {
        fout << row;
    }
    fout.close();
    fin.close();
    std::cout << "Success!!!\n";
}

void deleteCSVRow(const QString& filepath, int rowInd) {
    CSVRow row;
    std::fstream fin(filepath.toStdString(), std::ios::in), fout("tmp.csv", std::ios::out);

    int current_row = 0;
    while (row.readNextRow(fin) != EOF) {
        if (current_row++ == rowInd) {
            continue;
        } else {
            fout << row;
        }
    }
    fin.close();
    fout.close();
    std::cout << "Success while saving...\n";
    fout.open(filepath.toStdString(), std::ios::out);
    fin.open("tmp.csv", std::ios::in);
    while (row.readNextRow(fin) != EOF) {
        fout << row;
    }
    fout.close();
    fin.close();
    std::cout << "Success!!!\n";
}
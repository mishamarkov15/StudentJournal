#ifndef TABLEPROJECT_MAINWINDOW_H
#define TABLEPROJECT_MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QTextEdit>
#include <fstream>
#include <QDebug>
#include <iostream>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTabWidget>

#include "newStudentDialog.h"
#include "StudentTableWidget.h"
#include "ScheduleTableWidget.h"
#include "AboutDialogWindow.h"
#include "utils/csv.h"

class MainWindow : public QMainWindow {
Q_OBJECT;
public:
    explicit MainWindow(QWidget *parent = nullptr);

    void initLayout();

private slots:
    void saveAs();

    void showAbout();

private:
    void initTabs();

    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *saveAsAction;
    QAction *aboutAction;

    QGridLayout *layout;
    QWidget *widget;
    QTabWidget *tabWidget;
};


#endif //TABLEPROJECT_MAINWINDOW_H

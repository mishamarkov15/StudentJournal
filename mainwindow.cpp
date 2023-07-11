#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          layout(new QGridLayout()), widget(new QWidget()),
                                          tabWidget(new QTabWidget()) {
    setCentralWidget(widget);
    createActions();
    createMenus();
    initLayout();
    setWindowTitle("Student Project");
}

void MainWindow::initTabs() {
    auto* studentTable = new StudentTableWidget();
    tabWidget->addTab(studentTable, "students");
    tabWidget->addTab(new ScheduleTableWidget(studentTable), "schedule");
}

void MainWindow::initLayout() {
    initTabs();
    layout->addWidget(tabWidget);
    widget->setLayout(layout);
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(saveAsAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createActions() {
    saveAsAction = new QAction(tr("&Save as"), this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save file as..."));
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);

    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setStatusTip(tr("Info about this application"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::saveAs() {
    dynamic_cast<TableAbstractWidget*>(tabWidget->currentWidget())->saveAs();
}

void MainWindow::showAbout() {
    auto* dialog = new AboutDialogWindow(this);
    dialog->show();
}

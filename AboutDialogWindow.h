//
// Created by Михаил Марков on 03/06/2023.
//

#ifndef TABLEPROJECT_ABOUTDIALOGWINDOW_H
#define TABLEPROJECT_ABOUTDIALOGWINDOW_H

#include <QDialog>
#include <QGridLayout>
#include <QPixmap>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>

#include "LogoWidget.h"

class AboutDialogWindow : public QDialog {
    Q_OBJECT;
public:
    explicit AboutDialogWindow(QWidget* parent = nullptr);

private slots:
    void closeWindow();
private:
    void initLayout();

    void initWidgets();

    void initConnections();

    QGridLayout* layout1;
    QPushButton* closeButton;
    QLabel* aboutText;
    LogoWidget* logo;
};


#endif //TABLEPROJECT_ABOUTDIALOGWINDOW_H

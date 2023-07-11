//
// Created by Михаил Марков on 03/06/2023.
//

#include "AboutDialogWindow.h"

AboutDialogWindow::AboutDialogWindow(QWidget *parent) : QDialog(parent), layout1(new QGridLayout()),
                                                        closeButton(new QPushButton("Ok")), aboutText(new QLabel()),
                                                        logo(new LogoWidget()) {
    initWidgets();
    initLayout();
    initConnections();
}

void AboutDialogWindow::initLayout() {
    setFixedSize(500, 300);
    layout1->addWidget(logo, 0, 0, 5, 2);
    layout1->addWidget(aboutText, 0, 2, 5, 4);
    layout1->addWidget(closeButton, 5, 4, 1, 2);
    setLayout(layout1);
}

void AboutDialogWindow::initWidgets() {
    aboutText->setText("The application is aimed at helping a tutor to monitor students progress,"
                       " prepare homework for a certain date and keep track of tuition payments.");
    aboutText->setAlignment(Qt::AlignCenter);
    aboutText->setFrameStyle(QFrame::NoFrame);
    aboutText->setWordWrap(true);
}

void AboutDialogWindow::initConnections() {
    connect(closeButton, &QPushButton::clicked, this, &AboutDialogWindow::closeWindow);
}

void AboutDialogWindow::closeWindow() {
    close();
}

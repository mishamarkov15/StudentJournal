//
// Created by Михаил Марков on 05/06/2023.
//

#ifndef TABLEPROJECT_LOGOWIDGET_H
#define TABLEPROJECT_LOGOWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QGridLayout>

class LogoWidget : public QWidget {
Q_OBJECT;
public:
    explicit LogoWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QGridLayout *layout;
};


#endif //TABLEPROJECT_LOGOWIDGET_H

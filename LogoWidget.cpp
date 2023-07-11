#include "LogoWidget.h"

LogoWidget::LogoWidget(QWidget *parent) : QWidget(parent), layout(new QGridLayout()) {
    setLayout(layout);
}

void LogoWidget::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    p.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    p.drawEllipse(0, 0, 100, 100);
    p.drawText(30, 50, "Tutor");
    p.drawText(35, 64, "Assistant");
}

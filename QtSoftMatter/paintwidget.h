#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QPainter>
#include "model.h"

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = 0);
    void setModel(Model &m);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void paintVisualisation(QPainter &painter);
    void drawEigenEllipse(QPainter &painter, Eigen::Vector2d q0, double radius=0.5);
    bool modelSet;
    Model model;
};

#endif // PAINTWIDGET_H

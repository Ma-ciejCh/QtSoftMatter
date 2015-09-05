#include "paintwidget.h"

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{
    modelSet = false;
}

void PaintWidget::setModel(Model &m)
{
    model = m;
    model.p = m.p;
    model.q = m.q;
    modelSet = true;
}

void PaintWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // brush and colors:
    static QPalette palette;
    painter.setBrush(palette.window());
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(QPen(Qt::black, 0.0));

    paintVisualisation(painter);
}

void PaintWidget::paintVisualisation(QPainter &painter)
{
    if (!modelSet)
        return;

    double scale = (width() - 1.) / model.boxW;
    if (model.boxW/model.boxH < (width() - 1.)/(height() - 1.))
        scale = (height() - 1.) / model.boxH;
    painter.scale(scale, scale);

    for (int i=0; i<model.N; ++i)
        drawEigenEllipse(painter, model.q.col(i));

    painter.drawLine(QPointF(0, 0), QPointF(model.boxW, 0));
    painter.drawLine(QPointF(0, 0), QPointF(0, model.boxH));
    painter.drawLine(QPointF(model.boxW, model.boxH), QPointF(model.boxW, 0));
    painter.drawLine(QPointF(model.boxW, model.boxH), QPointF(0, model.boxH));
}

void PaintWidget::drawEigenEllipse(QPainter &painter, Eigen::Vector2d q0, double radius)
{
    painter.drawEllipse(QPointF(q0[0], q0[1]), radius, radius);
}

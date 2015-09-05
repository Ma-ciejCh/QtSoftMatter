#ifndef QTSOFTMATTER_H
#define QTSOFTMATTER_H

#include <QWidget>
#include <QTimer>
#include <eigen3/Eigen/Core>
#include "model.h"

namespace Ui {
class QtSoftMatter;
}

class QtSoftMatter : public QWidget
{
    Q_OBJECT

public:
    explicit QtSoftMatter(QWidget *parent = 0);
    ~QtSoftMatter();

private:
    Ui::QtSoftMatter *ui;
    QTimer paintTimer, simTimer;
    Model model;
    bool simLock;

private slots:
    void updatePaintWidgets();
    void initModel();
    void computeRK4();
    void updateModel();
};

#endif // QTSOFTMATTER_H

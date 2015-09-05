#include "qtsoftmatter.h"
#include "ui_qtsoftmatter.h"
#include <boost/format.hpp>

QtSoftMatter::QtSoftMatter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QtSoftMatter)
{
    simLock = false;
    ui->setupUi(this);
    initModel();
    paintTimer.setInterval(50);
    simTimer.setInterval(50);
    connect(&paintTimer, SIGNAL(timeout()), this, SLOT(updatePaintWidgets()));
    connect(&simTimer, SIGNAL(timeout()), this, SLOT(computeRK4()));
    paintTimer.start();
    simTimer.start();

    connect(ui->nuDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(initModel()));
    connect(ui->nhSpinBox, SIGNAL(valueChanged(int)), this, SLOT(initModel()));
    connect(ui->dtDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateModel()));
    connect(ui->mSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateModel()));
    connect(ui->nSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateModel()));
}

QtSoftMatter::~QtSoftMatter()
{
    delete ui;
}

void QtSoftMatter::updatePaintWidgets()
{
    ui->paintWidget->setModel(model);
    ui->paintWidget->update();
}

void QtSoftMatter::initModel()
{
    model.init(ui->dtDoubleSpinBox->value(), ui->mSpinBox->value(), ui->nSpinBox->value(),
               ui->nhSpinBox->value(), 1/ui->nuDoubleSpinBox->value(),
               ui->sv0DoubleSpinBox->value());
    ui->paintWidget->setModel(model);
    ui->packingFractionLabel->setText(QString("%1 %").arg(100*model.packingFractionHCP));
    ui->cutoffLabel->setText(QString("%1 σ").arg(model.cutoff));
}

void QtSoftMatter::computeRK4()
{
    if (simLock)
        return;

    simLock = true;

    int steps = ui->spfSpinBox->value();

    for (int i=0; i<steps; ++i)
        model.rk4step();

    updatePaintWidgets();

    simLock = false;
}

void QtSoftMatter::updateModel()
{
    model.dt = pow(10, ui->dtDoubleSpinBox->value());
    model.m = ui->mSpinBox->value();
    model.n = ui->nSpinBox->value();
    model.update();
    ui->cutoffLabel->setText(QString("%1 σ").arg(model.cutoff));
}

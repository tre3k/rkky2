#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVector>
#include <QDebug>

#include "tplot.h"
#include "tplot2dcase.h"
#include "function_cuda.h"

#include "rkkyfunction.h"

#define T_N 257
#define T_FROM -0.3
#define T_TO 0.3

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionProcess_triggered();

    void on_actionQuit_triggered();

private:
    Ui::MainWindow *ui;

    tPlot *plotDispersion;
    tPlot *plotSphere;
    tPlot2DCase *plotIntencity;
    tPlot *plotCrossSection;

    void buildDispersion(rkkyFunction *rf);
    void buildSphere(rkkyFunction *rf);
    void buildIntencity(rkkyFunction *rf);
    void buildCrossSection(rkkyFunction *rf);

    void toCircle(double *x,double *y,double r,double phi);
    void crossAverage(double **data, double x0, double y0,
                      double angle, double o_angle, double offset,double len,
                      QVector<double> *vX, QVector<double> *vY);
    int doubleToInt(double var);

    void createMainSpinBoxes();
    struct s_SpinBoxes{
        QDoubleSpinBox *stiffness;
        QDoubleSpinBox *k_s;
        QDoubleSpinBox *DeltaH;
        QDoubleSpinBox *lambda;
    } mainSpinBoxes;

    struct s_SpinField{
        QDial *dialField;
        QDoubleSpinBox *spinBoxField;
    } spinFiled;

    struct s_DispersionWidget{
        QWidget *widget;
        QFormLayout *layout;
        QComboBox *comboBox;
    } dispersionWidget;

};

#endif // MAINWINDOW_H

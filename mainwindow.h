#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVector>

#include "tplot.h"
#include "tplot2dcase.h"
#include "function_cuda.h"

#include "rkkyfunction.h"

#define T_N 256
#define T_FROM -0.15
#define T_TO 0.15


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

    void buildDispersion(rkkyFunction *rf);
    void buildSphere(rkkyFunction *rf);
    void buildIntencity(rkkyFunction *rf);

    void createMainSpinBoxes();
    struct s_SpinBoxes{
        QDoubleSpinBox *stiffness;
        QDoubleSpinBox *k_s;
        QDoubleSpinBox *DeltaH;
        QDoubleSpinBox *lambda;
    } mainSpinBoxes;


};

#endif // MAINWINDOW_H

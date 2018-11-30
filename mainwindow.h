#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVector>

#include "tplot.h"
#include "tplot2dcase.h"
#include "function_cuda.h"


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

private:
    Ui::MainWindow *ui;

    tPlot *plotDispersion;

    void buildDispersion();

};

#endif // MAINWINDOW_H

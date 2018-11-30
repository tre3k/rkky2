#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QHBoxLayout *layoutTab1 = new QHBoxLayout();
    plotDispersion = new tPlot();
    layoutTab1->addWidget(plotDispersion);


    this->setCentralWidget(ui->tabWidget);
    ui->tab_1->setLayout(layoutTab1);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionProcess_triggered()
{
    buildDispersion();

}

void MainWindow::buildDispersion(){
    QVector <double> vX,vY;

    vX.clear();vY.clear();
    int N = 250;
    double *out; double *in = new double [N];

    for(int i=0;i<N;i++){
        in[i] = (double)(i-N/2)/10.0;
        vX.append(in[i]);
    }

    FunctionCuda *funcuda = new FunctionCuda();
    out = funcuda->test(in,N);

    for(int i=0;i<N;i++){
        vY.append(out[i]);
        //vY.append(in[i]*in[i]);
    }


    plotDispersion->clearGraphs();
    plotDispersion->graph(plotDispersion->createGraph())->setData(vX,vY);
    plotDispersion->rescaleAxes(true);
    plotDispersion->replot();

    qDebug () << sizeof(bool);

    return;
}

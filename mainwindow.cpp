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

    QHBoxLayout *layoutTab2 = new QHBoxLayout();
    plotSphere = new tPlot();
    layoutTab2->addWidget(plotSphere);

    QHBoxLayout *layoutTab3 = new QHBoxLayout();
    plotIntencity = new tPlot2DCase();
    layoutTab3->addWidget(plotIntencity);


    this->setCentralWidget(ui->tabWidget);
    ui->tab_1->setLayout(layoutTab1);
    ui->tab_2->setLayout(layoutTab2);
    ui->tab_3->setLayout(layoutTab3);


    createMainSpinBoxes();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMainSpinBoxes(){
    mainSpinBoxes.stiffness = new QDoubleSpinBox();
    mainSpinBoxes.DeltaH = new QDoubleSpinBox();
    mainSpinBoxes.k_s = new QDoubleSpinBox();
    mainSpinBoxes.lambda = new QDoubleSpinBox();

    mainSpinBoxes.stiffness->setMaximum(99999999.99);
    mainSpinBoxes.stiffness->setMinimum(0.0);
    mainSpinBoxes.stiffness->setValue(523.21);

    mainSpinBoxes.k_s->setValue(0.93);
    mainSpinBoxes.k_s->setSingleStep(0.01);

    mainSpinBoxes.lambda->setValue(5.13);


    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(new QLabel("Stiffness: "));
    ui->mainToolBar->addWidget(mainSpinBoxes.stiffness);
    ui->mainToolBar->addWidget(new QLabel("meVA<sup>4</sup>, k<sub>s</sub>: "));
    ui->mainToolBar->addWidget(mainSpinBoxes.k_s);
    ui->mainToolBar->addWidget(new QLabel("nm<sup>-1</sup>, H-H<sub>c2</sub>: "));
    ui->mainToolBar->addWidget(mainSpinBoxes.DeltaH);
    ui->mainToolBar->addWidget(new QLabel("T, wavelength "));
    ui->mainToolBar->addWidget(mainSpinBoxes.lambda);
    ui->mainToolBar->addWidget(new QLabel("A"));



    return;
}

void MainWindow::on_actionProcess_triggered()
{
    //buildDispersion();

    double dt = (T_TO-T_FROM)/T_N;
    QVector <double> vX,vY1,vY2;
    vX.clear();vY1.clear();vY2.clear();

    rkkyFunction *rf = new rkkyFunction();
    rf->setConstants(mainSpinBoxes.stiffness->value(),
                     mainSpinBoxes.k_s->value(),
                     mainSpinBoxes.DeltaH->value(),
                     mainSpinBoxes.lambda->value());

    rkkyFunction::s_functions funcs;



    for(double var = T_FROM; var < T_TO; var += dt){
        vX.append(var);
        funcs = rf->getFunction(var,0,0);
        vY1.append(funcs.func1);
        vY2.append(funcs.func2);
    }


    plotDispersion->clearGraphs();
    plotDispersion->graph(plotDispersion->createGraph("green"))->setData(vX,vY1);
    plotDispersion->graph(plotDispersion->createGraph("red"))->setData(vX,vY2);
    plotDispersion->rescaleAxes(true);
    plotDispersion->replot();
    delete rf;
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

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

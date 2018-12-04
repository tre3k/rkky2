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
    plotIntencity->setMinimumSize(QSize(10,10));


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
    mainSpinBoxes.stiffness->setValue(10000.21);

    mainSpinBoxes.k_s->setValue(0.1);
    mainSpinBoxes.k_s->setDecimals(3);
    mainSpinBoxes.k_s->setSingleStep(0.005);

    mainSpinBoxes.lambda->setValue(6.28);

    mainSpinBoxes.DeltaH->setSingleStep(0.25);


    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(new QLabel("Stiffness: "));
    ui->mainToolBar->addWidget(mainSpinBoxes.stiffness);
    ui->mainToolBar->addWidget(new QLabel("meVÅ<sup>4</sup>, k<sub>s</sub>: "));
    ui->mainToolBar->addWidget(mainSpinBoxes.k_s);
    ui->mainToolBar->addWidget(new QLabel("Å<sup>-1</sup>, H-H<sub>c2</sub>: "));
    //ui->mainToolBar->addWidget(new QLabel("rad, H-H<sub>c2</sub>: "));
    ui->mainToolBar->addWidget(mainSpinBoxes.DeltaH);
    ui->mainToolBar->addWidget(new QLabel("T, wavelenght "));
    ui->mainToolBar->addWidget(mainSpinBoxes.lambda);
    ui->mainToolBar->addWidget(new QLabel("Å"));


    spinFiled.dialField = new QDial();
    //spinFiled.dialField->setH1
    spinFiled.dialField->setMaximum(360);
    spinFiled.dialField->setMinimum(0);

    spinFiled.spinBoxField = new QDoubleSpinBox();
    spinFiled.spinBoxField->setMaximum(360);
    spinFiled.spinBoxField->setMinimum(-360);
    spinFiled.spinBoxField->setValue(90);
    spinFiled.spinBoxField->setSingleStep(1);

    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(new QLabel("filed angle x-z: "));
    ui->mainToolBar->addWidget(spinFiled.spinBoxField);

    return;
}

void MainWindow::on_actionProcess_triggered()
{

    rkkyFunction *rf = new rkkyFunction();
    rf->setConstants(mainSpinBoxes.stiffness->value(),
                     mainSpinBoxes.k_s->value(),
                     mainSpinBoxes.DeltaH->value(),
                     mainSpinBoxes.lambda->value());

    buildDispersion(rf);
    buildIntencity(rf);
    buildSphere(rf);


    delete rf;
}

void MainWindow::buildSphere(rkkyFunction *rf){

    plotSphere->xAxis->setLabel("theta_x");
    plotSphere->yAxis->setLabel("omega");
    plotSphere->clearGraphs();
    plotSphere->graph(plotSphere->createGraph("green"))->setData(rf->vRootThetaX1,rf->vRootOmega1);
    plotSphere->graph(plotSphere->createGraph("red"))->setData(rf->vRootThetaX2,rf->vRootOmega2);
    //plotSphere->rescaleAxes(true);
    plotSphere->xAxis->setRange(QCPRange(T_FROM,T_TO));
    plotSphere->yAxis->setRange(QCPRange(T_FROM,T_TO));
    plotSphere->replot();

    return;
}

void MainWindow::buildDispersion(rkkyFunction *rf){
    double dt = (T_TO-T_FROM)/T_N;
    QVector <double> vX,vY1,vY2;
    vX.clear();vY1.clear();vY2.clear();

    rkkyFunction::s_functions funcs;

    for(double var = T_FROM; var < T_TO; var += dt){
        vX.append(var);
        funcs = rf->getFunction(0.0,0.0,var);
        vY1.append(funcs.func1);
        vY2.append(funcs.func2);
    }

    plotDispersion->xAxis->setLabel("omega");
    plotDispersion->yAxis->setLabel("function");
    plotDispersion->clearGraphs();
    plotDispersion->graph(plotDispersion->createGraph("green"))->setData(vX,vY1);
    plotDispersion->graph(plotDispersion->createGraph("red"))->setData(vX,vY2);
    plotDispersion->rescaleAxes(true);
    plotDispersion->replot();
    return;
}

void MainWindow::buildIntencity(rkkyFunction *rf){
    rf->calculateMap2D(T_FROM,T_TO,T_N,spinFiled.spinBoxField->value()*M_PI/180);
    plotIntencity->plot2D->plot2DMap(rf->map2D,T_N,T_N);

    plotIntencity->plot2D->ColorMap->data()->setRange(QCPRange(T_FROM,T_TO),QCPRange(T_FROM,T_TO));
    if(!plotIntencity->checkBoxHold->isChecked()){
        plotIntencity->plot2D->ColorMap->rescaleDataRange(true);
        plotIntencity->plot2D->rescaleAxes(true);
    }
    plotIntencity->plot2D->replot();

    return;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

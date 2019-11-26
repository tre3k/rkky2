#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Tab 1 Dispersion */
    QHBoxLayout *layoutTab1 = new QHBoxLayout();
    plotDispersion = new tPlot();
    layoutTab1->addWidget(plotDispersion);

    dispersionWidget.widget = new QWidget;
    dispersionWidget.layout = new QFormLayout();
    dispersionWidget.comboBox = new QComboBox();
    dispersionWidget.widget->setLayout(dispersionWidget.layout);
    dispersionWidget.widget->setMaximumSize(200,99999);
    dispersionWidget.layout->addRow("axies: ",dispersionWidget.comboBox);
    dispersionWidget.comboBox->addItem("ꞷ");
    dispersionWidget.comboBox->addItem("θx");
    dispersionWidget.comboBox->addItem("θy");
    layoutTab1->addWidget(dispersionWidget.widget);


    /* Tab 2 Sphere */
    QHBoxLayout *layoutTab2 = new QHBoxLayout();
    plotSphere = new tPlot();
    layoutTab2->addWidget(plotSphere);

    /* Tab 3 Intencity */
    QHBoxLayout *layoutTab3 = new QHBoxLayout();
    plotIntencity = new tPlot2DCase();
    layoutTab3->addWidget(plotIntencity);
    plotIntencity->setMinimumSize(QSize(10,10));

     /* Tab 4 Cross-section */
    QHBoxLayout *layoutTab4 = new QHBoxLayout();
    plotCrossSection = new tPlot();
    layoutTab4->addWidget(plotCrossSection);

    /* Tab 5 I(H) */
    QHBoxLayout *layoutTab5 = new QHBoxLayout();
    plotIatH = new tPlot();
    layoutTab5->addWidget(plotIatH);
    plotIatH->xAxis->setLabel("deltaH, T");
    plotIatH->yAxis->setLabel("I");
    //plotIatH->yAxis->setScaleType(QCPAxis::stLogarithmic);

    /* Tab 6 Ks(H) */
    QHBoxLayout *layoutTab6 = new QHBoxLayout();
    plotKatH = new tPlot();
    layoutTab6->addWidget(plotKatH);
    plotKatH->xAxis->setLabel("deltaH, T");
    plotKatH->yAxis->setLabel("theta_S");


    this->setCentralWidget(ui->tabWidget);
    ui->tab_1->setLayout(layoutTab1);
    ui->tab_2->setLayout(layoutTab2);
    ui->tab_3->setLayout(layoutTab3);
    ui->tab_4->setLayout(layoutTab4);
    ui->tab_5->setLayout(layoutTab5);
    ui->tab_6->setLayout(layoutTab6);


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
    mainSpinBoxes.stiffness->setValue(1000.21);
    mainSpinBoxes.stiffness->setSingleStep(100);

    mainSpinBoxes.k_s->setValue(0.2);
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
    ui->mainToolBar->addWidget(new QLabel("field angle x-z: "));
    ui->mainToolBar->addWidget(spinFiled.spinBoxField);

    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(ui->action_at_H);

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
    buildCrossSection(rf);

    delete rf;
}

void MainWindow::buildSphere(rkkyFunction *rf){
    /* for theoretical */
    QVector<double> vX;
    QVector<double> vY11,vY12,vY13,vY14;
    QVector<double> vY21,vY22,vY23,vY24;

    double y11,y12,y13,y14,y21,y22,y23,y24;
    double t_s;

    vX.clear();
    vY11.clear(); vY12.clear(); vY13.clear(); vY14.clear();
    vY21.clear(); vY22.clear(); vY23.clear(); vY24.clear();

    double dt = (T_TO-T_FROM)/T_N;
    for(double var = T_FROM; var < T_TO; var += dt/5){
        vX.append(var);
        t_s = rf->k_s/rf->k_i;
        y11 = sqrt(t_s*t_s + sqrt((2*rf->E_i*var+rf->DeltaH)/rf->A0/rf->k_i/rf->k_i/rf->k_i/rf->k_i)-var*var);
        vY11.append(y11);
        y12 = -sqrt(t_s*t_s + sqrt((2*rf->E_i*var+rf->DeltaH)/rf->A0/rf->k_i/rf->k_i/rf->k_i/rf->k_i)-var*var);
        vY12.append(y12);
        y13 = sqrt(t_s*t_s - sqrt((2*rf->E_i*var+rf->DeltaH)/rf->A0/rf->k_i/rf->k_i/rf->k_i/rf->k_i)-var*var);
        vY13.append(y13);
        y14 = -sqrt(t_s*t_s - sqrt((2*rf->E_i*var+rf->DeltaH)/rf->A0/rf->k_i/rf->k_i/rf->k_i/rf->k_i)-var*var);
        vY14.append(y14);

        y21 = sqrt(t_s*t_s + sqrt(-(2*rf->E_i*var-rf->DeltaH)/rf->A0/rf->k_i/rf->k_i/rf->k_i/rf->k_i)-var*var);
        vY21.append(y21);
        y22 = -sqrt(t_s*t_s + sqrt(-(2*rf->E_i*var-rf->DeltaH)/rf->A0/rf->k_i/rf->k_i/rf->k_i/rf->k_i)-var*var);
        vY22.append(y22);
        y23 = sqrt(t_s*t_s - sqrt(-(2*rf->E_i*var-rf->DeltaH)/rf->A0/rf->k_i/rf->k_i/rf->k_i/rf->k_i)-var*var);
        vY23.append(y23);
        y24 = -sqrt(t_s*t_s - sqrt(-(2*rf->E_i*var-rf->DeltaH)/rf->A0/rf->k_i/rf->k_i/rf->k_i/rf->k_i)-var*var);
        vY24.append(y24);
    }

    plotSphere->xAxis->setLabel("omega");
    plotSphere->yAxis->setLabel("theta_x");
    plotSphere->clearGraphs();
    plotSphere->graph(plotSphere->createGraph("green"))->setData(rf->vRootOmega1, rf->vRootThetaX1);
    plotSphere->graph(plotSphere->createGraph("red"))->setData(rf->vRootOmega2, rf->vRootThetaX2);
    /*
    plotSphere->graph(plotSphere->createGraph("green"))->setData(rf->vRootThetaX1,rf->vRootOmega1);
    plotSphere->graph(plotSphere->createGraph("red"))->setData(rf->vRootThetaX2,rf->vRootOmega2);
    */

    plotSphere->graph(plotSphere->createGraph("green"))->setData(vX,vY11);
    plotSphere->graph(plotSphere->graphCount()-1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone,"green","green",5));
    plotSphere->graph(plotSphere->graphCount()-1)->setPen(QPen(QColor("green"),1.5,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
    plotSphere->graph(plotSphere->createGraph("green"))->setData(vX,vY12);
    plotSphere->graph(plotSphere->graphCount()-1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone,"green","green",5));
    plotSphere->graph(plotSphere->graphCount()-1)->setPen(QPen(QColor("green"),1.5,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
    plotSphere->graph(plotSphere->createGraph("green"))->setData(vX,vY13);
    plotSphere->graph(plotSphere->graphCount()-1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone,"green","green",5));
    plotSphere->graph(plotSphere->graphCount()-1)->setPen(QPen(QColor("green"),1.5,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
    plotSphere->graph(plotSphere->createGraph("green"))->setData(vX,vY14);
    plotSphere->graph(plotSphere->graphCount()-1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone,"green","green",5));
    plotSphere->graph(plotSphere->graphCount()-1)->setPen(QPen(QColor("green"),1.5,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));

    plotSphere->graph(plotSphere->createGraph("red"))->setData(vX,vY21);
    plotSphere->graph(plotSphere->graphCount()-1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone,"red","red",5));
    plotSphere->graph(plotSphere->graphCount()-1)->setPen(QPen(QColor("red"),1.5,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
    plotSphere->graph(plotSphere->createGraph("red"))->setData(vX,vY22);
    plotSphere->graph(plotSphere->graphCount()-1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone,"red","red",5));
    plotSphere->graph(plotSphere->graphCount()-1)->setPen(QPen(QColor("red"),1.5,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
    plotSphere->graph(plotSphere->createGraph("red"))->setData(vX,vY23);
    plotSphere->graph(plotSphere->graphCount()-1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone,"red","red",5));
    plotSphere->graph(plotSphere->graphCount()-1)->setPen(QPen(QColor("red"),1.5,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
    plotSphere->graph(plotSphere->createGraph("red"))->setData(vX,vY24);
    plotSphere->graph(plotSphere->graphCount()-1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone,"red","red",5));
    plotSphere->graph(plotSphere->graphCount()-1)->setPen(QPen(QColor("red"),1.5,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));


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
        switch(dispersionWidget.comboBox->currentIndex()){
        // omega
        case 0:
            funcs = rf->getFunction(0.0,0.0,var);
            break;
        // theta_x
        case 1:
            funcs = rf->getFunction(var,0.0,0.0);
            break;
        // theta_y
        case 2:
            funcs = rf->getFunction(0.0,var,0.0);
            break;
        }

        vY1.append(funcs.func1);
        vY2.append(funcs.func2);
    }

    switch(dispersionWidget.comboBox->currentIndex()){
    // omega
    case 0:
        plotDispersion->xAxis->setLabel("ꞷ");
        break;
    // theta_x
    case 1:
        plotDispersion->xAxis->setLabel("θx");
        break;
    // theta_y
    case 2:
        plotDispersion->xAxis->setLabel("θy");
        break;
    }
    plotDispersion->yAxis->setLabel("function");
    plotDispersion->clearGraphs();
    plotDispersion->graph(plotDispersion->createGraph("green"))->setData(vX,vY1);
    plotDispersion->graph(plotDispersion->graphCount()-1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone,"green","green",5));
    plotDispersion->graph(plotDispersion->graphCount()-1)->setPen(QPen(QColor("green"),3,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
    plotDispersion->graph(plotDispersion->graphCount()-1)->selectionDecorator()->setPen(QPen(QColor("green"),4,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
    plotDispersion->graph(plotDispersion->createGraph("red"))->setData(vX,vY2);
    plotDispersion->graph(plotDispersion->graphCount()-1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone,"red","red",5));
    plotDispersion->graph(plotDispersion->graphCount()-1)->setPen(QPen(QColor("red"),3,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
    plotDispersion->graph(plotDispersion->graphCount()-1)->selectionDecorator()->setPen(QPen(QColor("red"),4,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
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

void MainWindow::buildCrossSection(rkkyFunction *rf){

    vRy.clear(); vLy.clear(); vRx.clear(); vLx.clear(); vAx.clear(); vAy.clear();


    crossAverage(rf->map2D,T_N/2,T_N/2,0,
                 30,0,T_N/2-1,&vRx,&vRy);
    crossAverage(rf->map2D,T_N/2,T_N/2,180,
                 30,0,T_N/2-1,&vLx,&vLy);

    for(int i=0;i<vRx.count();i++){
        vAx.append(vRx.at(i));
        vAy.append(vRy.at(i)+vLy.at(i));
    }


    plotCrossSection->xAxis->setLabel("theta");
    plotCrossSection->yAxis->setLabel("I");
    plotCrossSection->clearGraphs();
    plotCrossSection->graph(plotCrossSection->createGraph("green"))->setData(vLx,vLy);  // Left
    plotCrossSection->graph(plotCrossSection->createGraph("red"))->setData(vRx,vRy);    // Right
    plotCrossSection->graph(plotCrossSection->createGraph("black"))->setData(vAx,vAy);  // Amount
    plotCrossSection->rescaleAxes(true);
    plotCrossSection->replot();

    /*
    qDebug () << "\n *** Cross-section *** \n";
    for(int i=0;i<vAx.count();i++){
        qDebug () << vAx.at(i) << "\t" << vAy.at(i);
    }
    */

}

void MainWindow::crossAverage(double **data, double x0, double y0,
                                       double angle, double o_angle, double offset,double len,
                                       QVector<double> *vX, QVector<double> *vY)
{
    const double dr = sqrt(2);
    double dphi = atan(1.0/len);
    double r,phi;
    double x,y;
    double z = 0.0;
    int n = 0;

    int ix,iy;
    int oldx = -1,oldy = -1;

    for(r=offset; r<len; r+=dr){
        for(phi=angle-o_angle/2; phi < angle+o_angle/2; phi+=dphi){
            toCircle(&x,&y,r,phi);

            ix = doubleToInt(x+x0);
            iy = doubleToInt(y+y0);

            if(!(oldx==ix && oldy==iy)){
                z += data[ix][iy];
                n++;
            }

            oldx = ix; oldy = iy;
        }
        vX->append(r*(T_TO-T_FROM)/T_N);
        vY->append(z/(double)n);
        z = 0.0;
        n = 0;
    }

    return;
}

void MainWindow::toCircle(double *x,double *y,double r,double phi){
    phi = 2*M_PI*phi/360;
    *x = r*cos(phi);
    *y = r*sin(phi);
    return;
}

int MainWindow::doubleToInt(double var){
   return (int)(var+0.5);
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_action_at_H_triggered()
{
    double Hmin = 0.0;
    double Hmax = 10;

    double dH = 0.25;


    /*
    double Hmin = 300;
    double Hmax = 10000;

    double dH = 100;
    */

    QVector<double> vH,vKatH,vIatH,vThetaCH;
    vH.clear(); vKatH.clear(); vIatH.clear(), vThetaCH.clear();

    retKI KandH;

    for(double H=Hmin;H<Hmax;H+=dH){
        mainSpinBoxes.DeltaH->setValue(H);
        //mainSpinBoxes.stiffness->setValue(H);
        on_actionProcess_triggered();
        vH.append(H);
        KandH = findMax(&vAx,&vAy);
        vKatH.append(KandH.Ks);
        vIatH.append(KandH.I);
        vThetaCH.append(KandH.Theta_C);
    }

    plotIatH->clearGraphs();
    plotIatH->graph(plotIatH->createGraph("green"))->setData(vH,vIatH);
    plotIatH->rescaleAxes(true);
    plotIatH->replot();
    plotKatH->clearGraphs();
    plotKatH->graph(plotKatH->createGraph("green"))->setData(vH,vKatH);
    plotKatH->graph(plotKatH->createGraph("red"))->setData(vH,vThetaCH);
    plotKatH->rescaleAxes(true);
    plotKatH->replot();
}

MainWindow::retKI MainWindow::findMax(QVector<double> *vx, QVector<double> *vy){
    retKI retval;

    retval.I = vy->at(0);
    retval.Ks = vx->at(0);
    retval.Theta_C = retval.I;

    for(int i=0;i<vy->size();i++){
        if(retval.I < vy->at(i)){
            retval.I = vy->at(i);
            retval.Ks = vx->at(i);
        }
    }

    for(int i=vy->size();i>0;i--){
        if(vy->at(i) > 0.0){
            retval.Theta_C = vx->at(i);
            break;
        }
    }

    return retval;
}



#include "tplot.h"

tPlot::tPlot(QWidget *parent) : QCustomPlot (parent){
    graph_count = 0;

    this->setInteractions(QCP::iRangeZoom |
                          QCP::iRangeDrag |
                          QCP::iMultiSelect |
                          QCP::iSelectLegend |
                          QCP::iSelectPlottables |
                          QCP::iSelectAxes);

    this->xAxis2->setVisible(true);
    this->yAxis2->setVisible(true);

    connect(this->xAxis, SIGNAL(rangeChanged(QCPRange)),this->xAxis2, SLOT(setRange(QCPRange)));
    connect(this->yAxis, SIGNAL(rangeChanged(QCPRange)), this->yAxis2, SLOT(setRange(QCPRange)));

    connect(this,SIGNAL(axisClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)),
            this,SLOT(slot_sAxies_drag_zoom(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)));
    connect(this,SIGNAL(mouseDoubleClick(QMouseEvent*)),
            this,SLOT(slot_full_drag_zoom(QMouseEvent*)));
    connect(this,SIGNAL(selectionChangedByUser()),
            this,SLOT(slot_selectionChanged()));

}

void tPlot::slot_sAxies_drag_zoom(QCPAxis* sAxis,QCPAxis::SelectablePart part,QMouseEvent* event){
    this->axisRect()->setRangeDrag(sAxis->orientation());
    this->axisRect()->setRangeZoom(sAxis->orientation());
    return;
}

void tPlot::slot_full_drag_zoom(QMouseEvent *mouseEvent){
    this->axisRect()->setRangeDrag(this->xAxis->orientation()|
                                   this->yAxis->orientation());
    this->axisRect()->setRangeZoom(this->xAxis->orientation()|
                                   this->yAxis->orientation());
    return;
}

void tPlot::slot_selectionChanged(){
    if (this->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || this->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
          this->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || this->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
      {
        this->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        this->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
      }
      // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
      if (this->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || this->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
          this->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || this->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
      {
        this->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        this->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
      }

      // synchronize selection of graphs with selection of corresponding legend items:
      for (int i=0; i<this->graphCount(); ++i)
      {
        QCPGraph *graph = this->graph(i);
        QCPPlottableLegendItem *item = this->legend->itemWithPlottable(graph);
        if (item->selected() || graph->selected())
        {
          item->setSelected(true);
          graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
        }
      }
    return;
}

void tPlot::addNewValue(double value1,double value2,int graph){
    dataX.clear();
    dataY.clear();
    dataX.append(value1);
    dataY.append(value2);
    this->graph(graph)->addData(dataX,dataY);
    this->rescaleAxes(true);
    this->replot();
    emit signal_ready();
}

void tPlot::slot_addNewValue(double value1,double value2,int graph){
    addNewValue(value1,value2,graph);
}

void tPlot::clearData(){
    this->clearGraphs();
}

void tPlot::slot_clearData(){
    clearData();
}

int tPlot::createGraph(QColor colorLine, QString title){
    graph_count = this->graphCount();
    this->addGraph();

    this->graph(graph_count)->setName(title);
    this->graph(graph_count)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,colorLine,colorLine,5));
    this->graph(graph_count)->setPen(QPen(QColor(colorLine),1,Qt::NoPen,Qt::SquareCap,Qt::BevelJoin));
    this->graph(graph_count)->selectionDecorator()->setPen(QPen(QColor(colorLine),2,Qt::DotLine,Qt::SquareCap,Qt::BevelJoin));

    graph_count ++;
    return graph_count-1;
}

void tPlot::clearGraph(int graph){
    this->graph(graph)->data()->clear();
    this->removeGraph(this->graph(graph));
}

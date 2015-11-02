#include "lds_graphicsview.h"

lds_graphicsview::lds_graphicsview(QWidget *parent):
    QGraphicsView(parent)
{

}

lds_graphicsview::~lds_graphicsview()
{

}

void lds_graphicsview::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    if(this->scene()){
        this->fitInView(this->scene()->itemsBoundingRect(), Qt::KeepAspectRatio);

//        QTransform matrix(1,0,0,0,1,0,0,0,1);
//        qreal xratio = this->viewport()->rect().width() / this->sceneRect().width();
//        qreal yratio = this->viewport()->rect().height() / this->sceneRect().height();
//        xratio = yratio = qMin(yratio, xratio);
//        matrix.scale(xratio, yratio);
//        this->setTransform(matrix);
    }
}


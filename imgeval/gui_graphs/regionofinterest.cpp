#include "regionofinterest.h"
#include<QtGui>
#include<iostream>
#include<QMenu>
#include "atomimg.h"
using namespace std;

/******************************* REGION OF INTEREST **************************/
/*! constructor for a Region of interest Rectangle
\author Markus Debatin
\date 10-2010
*************************************************************************/
ROI::ROI(QGraphicsItem* parent)
    :QGraphicsRectItem(parent)
{
    setPen(QPen(QColor(100,255,0)));
    setRect(QRect(0,0,10,10));
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    grabsize=20;
    // setAcceptDrops(true);
}

//! called whenever a mouse button is pressed
/*! Eventhandler for mouse events. Used for moving and resizeing the ROI
\author Markus Debatin
\date 10-2010
*************************************************************************/
void ROI::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    grabsize=rect().width()/8+2; //adjust grab region to size of rect
    QGraphicsItem::mousePressEvent(event);
    clickpos=event->pos();
    clicksize=rect().bottomRight()+pos();
    if((clickpos.x()>(rect().width())-grabsize)&&(clickpos.y()>(rect().height()-grabsize))){
        mode = 2;  //resize right bottom
        event->accept();
        setSelected(true);
        return;
    }
    if((clickpos.x()<grabsize)||(clickpos.y()<grabsize)){
        if (event->button()== Qt::MidButton)
            mode= 1;
        else
            mode = 3;  //resize left top
        event->accept();
        setSelected(true);
        return;
    }
} 

/*! event handler used to move and resize the ROI
\author Markus Debatin
\date 10-2010
*************************************************************************/
void ROI::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    QGraphicsItem::mouseMoveEvent(event);
    if(mode==1){
        if((event->pos().x()<rect().width())
            &&(event->pos().y()<rect().height())){
            setPos(event->scenePos()-clickpos);
            setRect(QRectF(QPointF(0,0),clicksize+clickpos-event->scenePos()));
            setPos(event->scenePos()-clickpos);
        }
    }
    if(mode==2){
        if((event->pos().x() >0)&& (event->pos().y()>0))
            setRect(QRectF(QPointF(0,0),event->pos()+QPointF(1,1)));
    }
    if(mode==3){
        setPos(event->scenePos()-clickpos);
    }
    event->accept();

} 

//! Eventhandler for moving and resizeing
void ROI::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    setSelected(false);
    cout<<"ROIrelease"<<event->pos().x()<<";"<<event->pos().y()<<"\n";
    QGraphicsItem::mouseMoveEvent(event);
    if(mode==2){
        QPointF pos1 = pos();
        QPointF pos2 = event->pos()+pos();
        if(pos1.x()>pos2.x()) {
            float f=pos1.x(); pos1.setX(pos2.x()); pos2.setX(f);}
        if(pos1.y()>pos2.y()) {
            float f=pos1.y(); pos1.setY(pos2.y()); pos2.setY(f);}
        setPos(pos1);
        pos2 = pos2-pos();
        setRect(QRectF(QPointF(0,0),pos2+QPointF(1,1)));
    }
    else if(mode==1){
    }
    mode=0;
} 


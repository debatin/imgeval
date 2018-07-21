#include<QtGui>
#include<iostream>
#include<QMenu>
#include "atomimg.h"
#include <iostream>
#include"imagemarker.h"
#include<math.h>
using namespace std;


void ImageMarker::testslot()
{
    tt+=1;
    cout<<"testgrw\n";
}

/*! constructs an ImageMarker for full functionality use other constructor
\author Markus Debatin
\date 10-2010
*************************************************************************/
ImageMarker::ImageMarker(QGraphicsItem* parent)
    :QGraphicsWidget(parent)
{
    qr = new QGraphicsRectItem(this);
    qr->setRect(QRect(0,0,10,10));
    setBrush(QBrush(QColor(100,255,0)));
    setGeometry(QRect(0,0,10,10));
    atomimg = NULL;
    createActions();
    tt=0;
}


/*! constructs an ImageMarker
\param name name of the marker (can be nything to identify it)
\param img  Atomimg class this marker belongs to
\author Markus Debatin
\date 10-2010
*************************************************************************/
ImageMarker::ImageMarker(string name,Atomimg* img, QGraphicsItem* parent)
    :QGraphicsWidget(parent)
{
    qr = new QGraphicsRectItem(this);
    qr->setRect(QRect(0,0,10,10));
    setBrush(QBrush(QColor(100,255,0)));
    setGeometry(QRect(0,0,10,10));
    this->name = name;
    this->atomimg=img;
    if(atomimg != NULL) atomimg->markerpos_changed(name, pos()); // aktuelle Position bekanntgeben
    t = new ImageMarker();
    tt=0;
    createActions();
}

//! creates actions for context menus
/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
void ImageMarker::createActions()
{
    size10Act = new QAction(tr("&Markersize 10"), this);
    sizeincAct = new QAction(tr("&Markersize +"), this);
    sizedecAct = new QAction(tr("&Markersize -"), this);
    lockAct = new QAction(tr("Lock"), this);
    lockAct->setCheckable(true);
    connect(size10Act, SIGNAL(triggered()), this, SLOT(size10()));
    connect(sizeincAct, SIGNAL(triggered()), this, SLOT(incsize()));
    connect(sizedecAct, SIGNAL(triggered()), this, SLOT(decsize()));
    connect(lockAct, SIGNAL(toggled(bool)),this, SLOT(lock(bool)));
}

/*! sets the brush of the marker
\param brush brush to be used
\author Markus Debatin
\date 10-2010
*************************************************************************/
void ImageMarker::setBrush(const QBrush brush)
{
    qr->setBrush(brush);
}

/*! Eventhandler um den Marker zu verschieben
\author Markus Debatin
\date 10-2010
*************************************************************************/
void ImageMarker::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    QGraphicsWidget::mousePressEvent(event);
    event->accept();
    cout<<"pressevent\n";
    clickpos=event->pos()*scale();
}

/*! event handler used to move the marker
\author Markus Debatin
\date 10-2010
*************************************************************************/
void ImageMarker::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    QGraphicsWidget::mouseMoveEvent(event);
    if(!locked){
        setPos(event->scenePos()-clickpos);
        event->accept();
        //emit moved(pos());
        if(atomimg != NULL) atomimg->markerpos_changed(name, pos());
        cout<<"move event\n";
    }

}

/*! creates a context menu
\author Markus Debatin
\date 10-2010
*************************************************************************/
void ImageMarker::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->accept();
    QMenu menu;
    menu.addAction(size10Act);
    menu.addAction(sizeincAct);
    menu.addAction(sizedecAct);
    menu.addAction(lockAct);
    menu.exec(event->screenPos());
}

/*! sets the size ofthe marke to 10 pixel
\author Markus Debatin
\date 10-2010
*************************************************************************/
void ImageMarker::size10()
{
    setPos(pos()+QPointF((scale()-1)*5,(scale()-1)*5));
    setScale(1);
}

/*! increases the markersize by a factor of sqrt(10)
\author Markus Debatin
\date 10-2010
*************************************************************************/
void ImageMarker::incsize()
{
    setPos(pos()+QPointF(-scale()*(sqrt(sqrt(10))-1)*5,-scale()*(sqrt(sqrt(10))-1)*5));
    setScale(scale()*sqrt(sqrt(10)));
//    setPos(Pos()+QPointF(scale()(sqrt(sqrt(10))-1)*5,scale()*5));
}

/*! decreases the markersize by a factor of sqrt(10)
\author Markus Debatin
\date 10-2010
*************************************************************************/
void ImageMarker::decsize()
{
    setScale(scale()/sqrt(sqrt(10)));
    setPos(pos()+QPointF(scale()*(sqrt(sqrt(10))-1)*5,scale()*(sqrt(sqrt(10))-1)*5));
}

/*! locks marker in place
\author Tetsu Takekoshi
\date 1-2013
*************************************************************************/
void ImageMarker::lock(bool freeze)
{
    locked = freeze;
}

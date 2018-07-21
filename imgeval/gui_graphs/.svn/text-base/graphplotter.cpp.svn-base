#include "graphplotter.h"
#include "datacurve.h"
#include <QtGui>
#include<vector>
#include<iostream>
#include<assert.h>
#include "graphaxis.h"

using namespace std;
/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
GraphPlotter::GraphPlotter(QWidget* parent)
    :QGraphicsScene(parent)
{
    scaling=0.005;
    hscaling=1;
    axis=new GraphAxis;
    addItem(axis);
    setSceneRect(-20,-200,540,230);
}

/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
GraphPlotter::~GraphPlotter()
{
   // LOG
}

/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
void GraphPlotter::addCurve(Datacurve* curve)
{
    assert(curve);
    curve->setScaling(scaling);
    curve->setHScaling(hscaling);
    curve->setZValue(2);
    curvelist.push_back(curve);
    QGraphicsScene::addItem(curve);
}

/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
void GraphPlotter::setScaling(float newscaling)
{
    scaling=newscaling;
    axis->setScaling(scaling);
    for(int i=0;i<(int)curvelist.size();i++)
        curvelist[i]->setScaling(scaling);
}
/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
void GraphPlotter::setHScaling(float newscaling)
{
    hscaling=newscaling;
    axis->setHScaling(hscaling);
    for(unsigned int i=0;i<curvelist.size();i++)
        curvelist[i]->setHScaling(hscaling);
}
/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
void GraphPlotter::clear()
{
    QList<QGraphicsItem *>it = items();
    for(int i = 0; i<it.size();i++){
        removeItem(it[i]);
        delete it[i];
    }
    curvelist.clear();
    axis = new GraphAxis;
    addItem(axis);
}


/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
void GraphPlotter::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    int numsteps =event->delta()/30;   //1° corresponds delta=8. Our mouse 1Step=15°=120delta
    float scalingfactor=1;
    if (numsteps>0)
        for(int i = 0;i<numsteps;i++)
            scalingfactor*=1.03;
    else
        for(int i = 0;i<-numsteps;i++)
            scalingfactor/=1.03;
    event->accept();
    //  if((event->buttons()&& Qt::MidButton)||(event->modifiers()&& Qt::ShiftModifier))
    setScaling(scaling*scalingfactor);
}



#include "graphaxis.h"
#include<QtGui>
#include<iostream>
using namespace std;

/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
GraphAxis::GraphAxis(QGraphicsItem* parent)
    :QGraphicsItem(parent)
{
    scaling=1;
    width=500;
    hscaling=1;
}

/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
void GraphAxis::setScaling(float newscaling)
{
    scaling=newscaling;
    if (scaling==0) scaling=1;
    update();

}
/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
void GraphAxis::setHScaling(float newscaling)
{
    hscaling=newscaling;
    if (hscaling==0) hscaling=1;

}
/*!  from a input it generates a limited number (lim)
  which is in boundaries between 1 and 2.5 and a 
  multiplier so that input*multiplier=lim
  also it is ensured that multiplier is either 2,5,1 times 10^x
*/
/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
void generateBoundNumber(float input,float &lim,float &multiplier)
{
    lim=input;
    multiplier=1;
    while(lim>1){lim/=10; multiplier/=10;}
    while(true){
        lim *= 2; multiplier*=2;
        if(lim>=1) break;
        lim *= 2.5;multiplier*=2.5;
        if(lim>=1) break;
        lim *= 2; multiplier*=2;
        if(lim>=1) break;
    }
} 

//! calculates and paints the axis shown on the graph
/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
void GraphAxis::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    float multiplier; float scale;
    generateBoundNumber(scaling,scale,multiplier);
    // painter->drawRoundRect(-100, -100, 200, 200);
    for(int i=0;i*scale<20;i++)
    {  painter->drawLine(0,(int)-i*scale*10,width,(int)-i*scale*10);
        painter->drawText(-20,-i*scale*10,QString::number(i*multiplier*10));
    }
    generateBoundNumber(hscaling,scale,multiplier);
    for(int i=0;i*scale*50<width;i++)
    {  painter->drawLine((int)(i*scale*100),-200,(int)(i*scale*100),0);
        painter->drawText((int)i*scale*100,20,QString::number(i*multiplier*100));
    }
}

/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
QRectF GraphAxis::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(-30 , -230 ,
                  width+60 + penWidth / 2, 360 );
}


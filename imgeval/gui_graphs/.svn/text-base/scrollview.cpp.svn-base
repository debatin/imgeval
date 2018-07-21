#include "scrollview.h"
#include <QtGui>
#include "atomimg.h"
#include "mainwindow.h"
#include <sstream>
#include <iostream>
ScrollView::ScrollView(QWidget* parent)
  :QGraphicsView(parent)
{

  noscroll=false;
}
void ScrollView::setNoScroll(bool nosc)
{
  noscroll=nosc;
}

void ScrollView::wheelEvent(QWheelEvent* event)
{
setTransformationAnchor (QGraphicsView::AnchorUnderMouse); //makes point under mouse not move

    if(noscroll){event->ignore(); return;}
  if(event->modifiers()&& Qt::AltModifier){
    QGraphicsView::wheelEvent(event); 
    return;}
  int numsteps =event->delta()/30;   //1° corresponds delta=8. Our mouse 1Step=15°=120delta
  float scalingfactor=1;
  if (numsteps>0)
    for(int i = 0;i<numsteps;i++)
      scalingfactor*=1.03;
  else
    for(int i = 0;i<-numsteps;i++)
      scalingfactor/=1.03;
  event->accept();
  if((event->buttons()&& Qt::MidButton)||(event->modifiers()&& Qt::ShiftModifier))
  scale(1,scalingfactor);
  else
  scale(scalingfactor,scalingfactor);
}

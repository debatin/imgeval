#include "datacurve.h"
#include <iostream>
#include <QtGui>
using namespace std;

/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
Datacurve::Datacurve(QGraphicsItem* parent)
        :QGraphicsPathItem(parent)
{
  scaling=1;
  hscaling=1;
}

/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
Datacurve::Datacurve(const std::vector<float> newdata,QGraphicsItem* parent)
        :QGraphicsPathItem(parent)
{
  scaling=1;
  hscaling=1;
    setData(newdata);
}

/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
Datacurve::Datacurve(const std::vector<int>& newdata,QGraphicsItem* parent)
        :QGraphicsPathItem(parent)
{
  scaling=1;
  hscaling=1;
    setData(newdata);
}

/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Datacurve::setScaling(float newscaling)
{
  scaling=newscaling;
  redraw();


}
/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Datacurve::setHScaling(float newscaling)
{
  hscaling=newscaling;
  redraw();


}
/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
float Datacurve::maxval()
{
  if (data.size()==0) return 0;
  float maxval = data[0];
  for(unsigned int i = 0;i<data.size();i++)
    if(maxval <data[i]) maxval=data[i];
  return maxval;
}
/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
float Datacurve::minval()
{
  if (data.size()==0) return 0;
  float minval = data[0];
  for(unsigned int i = 0;i<data.size();i++)
    if(minval >data[i]) minval=data[i];
  return minval;
}

/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Datacurve::redraw()
{
  prepareGeometryChange();
  pp=QPainterPath();
  if (data.size()>0){
  pp.moveTo(0,-data[0]*scaling);
  for(unsigned int i = 0;i<data.size();i++) 
    pp.lineTo(i*hscaling,-data[i]*scaling);
  }
  setPath(pp);
  update();
}

/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Datacurve::setData(const std::vector<float> newdata){
  data.clear();
  for(unsigned int i = 0;i<newdata.size();i++) 
    data.push_back(newdata[i]);
  redraw();

}

/*!
\author Markus Debatin
\date 10-2010
*************************************************************************/
void Datacurve::setData(const std::vector<int>& newdata){
  data.clear();
  for(unsigned int i = 0;i<newdata.size();i++) 
    data.push_back(newdata[i]);
  redraw();

}

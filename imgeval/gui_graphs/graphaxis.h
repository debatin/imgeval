#ifndef GRAPHAXIS
#define GRAPHAXIS
#include <QGraphicsItem>
#include "datastructure/numeric_templates.h"

//! Draws axis and background lines for the graphs 
/*! Draws graph axis and Background Lines
  a scaling of 1 means Values of the curves correspond to values 
in pixel. Axis will range from 0to 1000 and from 0 to 300 
*/
class testme
{
 public:
  testme(){i=0; i+=1;}
  int i;
};

class GraphAxis: public QGraphicsItem
{
 public:
  GraphAxis(QGraphicsItem* parent=0);
  ~GraphAxis(){LOG};
  void setWidth(int w){width=w;}
  QRectF boundingRect() const;
   void setScaling(float);
   void setHScaling(float);
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
  float scaling;
  float hscaling;
  int width;
};



#endif

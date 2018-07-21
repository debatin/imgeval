#ifndef GRAPHPLOTTER_H
#define GRAPHPLOTTER_H
#include<QGraphicsScene>
//#include "datacurve.h"
#include "datastructure/numeric_templates.h"
class Datacurve;
class GraphAxis;
class QGraphicsSceneWheelEvent;
class GraphPlotter: public QGraphicsScene
{
  Q_OBJECT
    public:
    GraphPlotter(QWidget* parent = 0);
    ~GraphPlotter();
    void addCurve(Datacurve* curve);
    void clear();
    void setScaling(float scaling);
    void setHScaling(float scaling);
    void wheelEvent(QGraphicsSceneWheelEvent* event);

 protected:
    std::vector<Datacurve*> curvelist;
    float scaling;
    float hscaling;
    GraphAxis* axis;
};


#endif // GRAPHPLOTTER_H

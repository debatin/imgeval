#ifndef DATACURV_H
#define DATACURV_H
#include<QGraphicsPathItem>
#include<vector>

//! graphical representation of a simple datacurve
/*!


  */
class Datacurve: public QGraphicsPathItem
{
 public:
  Datacurve(QGraphicsItem* parent = 0);
    Datacurve(const std::vector<float> newdata,QGraphicsItem* parent = 0);
    Datacurve(const std::vector<int>& newdata,QGraphicsItem* parent = 0);
    void setData(const std::vector<float> newdata);
    void setData(const std::vector<int>& newdata);
    void setScaling(float scaling);
    void setHScaling(float scaling);
    float maxval(); //returns maximum value
    float minval();  //returns minimum value
    void redraw();
    float scaling;
    float hscaling;  //horizontal scaling (datapoints/pixel)
    std::vector<float> data;
    QPainterPath pp;
};

#endif // DATACURVE_H

#ifndef SCROLLVIEW_H
#define SCROLLVIEW_H
#include <QGraphicsView>
class ScrollView: public QGraphicsView
{
  Q_OBJECT
    public:
    ScrollView(QWidget* parent =0 );
    void setNoScroll(bool nosc=true);
 protected:
  void wheelEvent(QWheelEvent* event);
  bool noscroll;
    public slots:
  void zoomIn(){scale(1.2,1.2);}
  void zoomOut(){  scale(1/1.2,1/1.2);}
};


#endif

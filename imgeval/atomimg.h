#ifndef ATOMIMG_H
#define ATOMIMG_H
#include<QWidget>
#include<iostream>
#include<QGraphicsView>
#include "imagemonitor.h"
#include "graphplotter.h"
#include "datastructure/imgdata.h"
#include "datastructure/dataset.h"
#include <vector>
#include <QPointF>
#include <QString>
using namespace std;
class imgmarker;
class QLabel;
class ROI;
class ScrollView;
class ParamBox;
class QAction;
class MainWindow;
//! Evaluate and display the images for one species
/*! This class consists of a frame containing the main picture, the two fitareas and the information box.
This class contains the calls to fitting functions, and manages loading and evalutaion of the images.  

In the constructor this class is given two datasets one to store the results, one to get (and also change) its settings. It is also given a name, which is used to identify the class in case of multiple instances. This name is prepended to each parameter(-name) the class reads or writes. 



  */

#define SAVEIMG_REPLACE_RAW 0x10  ///< the rawdata should be replace by a smaller fraction
#define SAVEIMG_DELETE_RAW 0x08   ///< the rawdata should be deleted
#define SAVEIMG_RESULTS 0x04      ///< the results shouldbe saved separately with the rawdata
#define SAVEIMG_IMG 0x01          ///<the img from the screen should be saved
#define SAVEIMG_DELETE_MAT 0x01          ///<the img from the screen should be saved
#define SAVEIMG_BACKGROUND 0x20   ///<the img should be added to the Background list

class Atomimg: public QWidget
{
    Q_OBJECT
    public:
    Atomimg(QWidget* parent = 0);
    Atomimg(Dataset* mresults,Dataset* settings, std::string s_name,
            MainWindow* central,QWidget* parent = 0);
    ~Atomimg();
    void init();
    void createActions();
    void updateParamBox();
    void setMeasurement(QString m){measindex=m.toStdString();}; ///< sets the Measurement number

    void reloadData();  ///< does image evaluation and calculations
    void standardAbsorptionCalcualtions();  ///< does standard calculations for absorption imaging
    void reloadBackground(); ///< loads or reloads background imgs

    void fitData();  ///<performs and plots fits
    void perform1DFit(vector<float> fitdaty, vector<float> &fitparams);
    vector<float> perform1DSpecialBimodal(vector<float> fitdaty, vector<float> fitdatx);

    void plotfunction(const vector<float>&fitparams,
                      int npoints,GraphPlotter* graph,string fitname="gaussian");
    void plotxy(const vector<float> &datax, const vector<float> &datay,  //< plots f curve with x,y coordinates given
                           GraphPlotter* graph);

    void storeGaussian(const vector<float>&fitparams,  //< stores the parameters with nice names
                       int offset,string pname);
    void storeBimodal(const vector<float>&fitparams,   //< creates BEC fration etc
                       int offset,string pname);
    void storeFitparams(const vector<float>&fitparams,  //< stores the fitted variables in a generic way
                       int offset,string pname);

    void savePNG();
    string getname(){return name;};
    void markerpos_changed(string name, QPointF pos);   //<called by the markers to indicate their position has changed
protected:
     void contextMenuEvent(QContextMenuEvent *event);
     void setImage();


public slots:
    void updateSidegraphs();
    void toggleSubtract(bool toggled);
    void selectImg();
    void markerchanged(QString name, QPointF pos);
 private:
    GraphPlotter* topgraph;   ///<fit and graph on top if image
    QGraphicsView* toptrace;  ///<GUI display fits on top
    GraphPlotter* sidegraph;  ///<fitarea at side containing fits
    QGraphicsView* sidetrace; ///<GUI to display fit at side
    ParamBox* parambox;       ///<informations box for fitted parameters
    QLabel* L_Atomnumber;     ///<abandonned !!!!!!!!!!!!1
    QLabel* L_Center;         ///< just a Label !!!!!!!!!!!!!11
    Imagemonitor* mainimg;    ///<GUI used to display the image 
    Imagedata img_atoms;      ///<Image taken with atoms
    Imagedata img_noatoms;    ///< Imaging beam without atoms
    Imagedata img_atoms_back; ///< Background light without image beam
    Imagedata img_noatoms_back; ///< Background light without image beam
    Imagedata img;            ///< image as result of computing
    std::string measindex;
    std::string oldmeasindex;
    std::string background_str;
    std::string name;         ///< name of this img (Rb or Cs)
    Dataset* results;         ///< central dataset stores results
    Dataset* settings;        ///< central program settings
    vector<string> background_names;
    imgmarker* marker;    ///< various coloured marker
    imgmarker* fitstart_marker;///< parameter to set fit start param
    ROI* roi_fit;             ///< Region of interest for the fits
    ROI* roi_background;      ///< ROI for additional Background Subtraction 
    ROI* roi_replace_raw;
    QAction* subtractregionAct;
    QAction* selectimgAct;
    QAction* autoscaleAct;
    string deletionbuffer;
public:
    MainWindow* mainapp;
private:
     int showimage; //0 data 1 imgatoms 2 noatoms 3 Background 4 backgroundn
};




#endif // ATOMIMG_H

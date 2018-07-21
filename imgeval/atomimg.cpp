#include "atomimg.h"
#include "imagemonitor.h"
#include<QtGui>
#include "graphplotter.h"
#include "datastructure/imgdata.h"
#include "datacurve.h"
#include "fits/perform_fits.h"
#include "scrollview.h"
#include "datastructure/dataset.h"
#include <vector>
#include <math.h>
#include <exception>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include "datastructure/numeric_templates.h"
#include "regionofinterest.h"
#include "parambox.h"
#include "mainwindow.h"
#include "constants.h"
#include "fits/fitfunctions.h"
#include "assert.h"
using namespace std;

//! constructor you probably don't want to use it
Atomimg::Atomimg(QWidget* parent)
    :QWidget(parent)
{
    showimage=0;
    marker=NULL;
    results=new Dataset;
    settings=new Dataset;
    init();
}
//! Constructor taking allnecessary parameters
/*! Use this consructor to creae a working insance of this object
\param mresults container to store measurement results
\param setings container o store general settings
\param s_name  name of the insance for ex. "Rb". Serves to
identify the corresponding arameters for ex "Rbdetuning"
\param center pointer to central Applet used for function calls
\param parent  see Qt QWidget
 */

Atomimg::Atomimg(Dataset* mresults,Dataset* settings, std::string s_name,
                 MainWindow* center,QWidget* parent)
  :QWidget(parent)
{
    marker=NULL;
    mainapp=center;
    results= mresults;
    this->settings= settings;
    name=s_name;
    init();
}


Atomimg::~Atomimg()
{
}

/*!called by the constructor. Mainly controlls Layout
 */
void Atomimg::init()
{
    settings->get("file_extensions").vs.push_back(settings->get(name+"ending_atoms").s);
    settings->get("file_extensions").vs.push_back(settings->get(name+"ending_noatoms","").s);

    topgraph = new GraphPlotter;              //graphs at top of the image
    toptrace= new QGraphicsView(this);
    toptrace->setScene(topgraph);

    sidegraph = new GraphPlotter;            //graphs at the side of the image
    sidetrace= new QGraphicsView(this);
    sidetrace->setScene(sidegraph);
    sidetrace->rotate(90);

    mainimg= new Imagemonitor(settings,mainapp,name,this);  //central image
    QGraphicsView* mainarea;
    mainarea= new ScrollView(this);
    mainarea->setScene(mainimg);

    marker=new imgmarker(name+"marker",this);              //Marker
    marker->setBrush(QBrush(QColor(0,0,150)));
    marker->setZValue(4);
    mainimg->addItem(marker);

    fitstart_marker = new imgmarker(name+"fitstart",this);
    fitstart_marker->setPos(settings->get(name+"_fitstart_x",0).i,settings->get(name+"_fitstart_y",0).i);
    fitstart_marker->setBrush(QBrush(QColor(0,250,50)));
    fitstart_marker->setZValue(4);
    mainimg->addItem(fitstart_marker);

    roi_fit = new ROI;
    roi_fit->setRect(QRectF(0,0,1000,1000));
    roi_fit->setZValue(2);
    mainimg->addItem(roi_fit);

    roi_background = new ROI;
    roi_background->setRect(QRectF(0,0,100,100));
    roi_background->setZValue(2);
    roi_background->setPen(QColor(236,0,255));
    mainimg->addItem(roi_background);

    roi_replace_raw = new ROI;
    roi_replace_raw->setRect(QRectF(0,0,1000,1000));
    roi_replace_raw->setZValue(1.9);
    roi_replace_raw->setPen(QColor(236,255,0));
    mainimg->addItem(roi_replace_raw);


    parambox = new ParamBox(results,name,this);

    QGridLayout* lay = new QGridLayout(this);
    lay->addWidget(toptrace,1,1);
    lay->addWidget(mainarea,2,1);
    lay->addWidget(parambox,1,2);
    lay->addWidget(sidetrace,2,2);
    lay->setColumnStretch(1,(int)((1-settings->get("fit_area_fraction",0.2).d)*100));
    lay->setColumnStretch(2,(int)((settings->get("fit_area_fraction").d)*100));
    lay->setRowStretch(1,(settings->get("fit_area_fraction").d)*100);
    lay->setRowStretch(2,(1-settings->get("fit_area_fraction").d)*100);
    setLayout(lay);
    connect(mainimg,SIGNAL(selectionChanged()),this,SLOT(updateSidegraphs()));
    createActions();
    reloadBackground();
}



//!
/*!reloads the image and performs image calcultaions
The images are loaded from files and the main image calculations are performed
The image with and without atoms are loaded. Next the images are devided and
the crossection is calculated using
\sigma= crossect*(lambda^2/(1+(2*detuning/linewidth)^2)
The images are calculated to represent the number of atoms per pixel
 */
void Atomimg::reloadData()
{
    if(!(background_names==settings->get("backgroundfiles").vs)){
        reloadBackground();
        return;    //reloadData() is called from reloadBackground.
                   //so we need not to continue this run of reloadData
    }

    // store the fitmarker. This enables to put the marker wehn you restart the program
    settings->store(name+"_fitstart_x",(int)fitstart_marker->pos().x());
    settings->store(name+"_fitstart_y",(int)fitstart_marker->pos().y());

    //check is everything is configured
    if(measindex.empty()) return ;
    try{
        img_atoms.load((measindex+settings->get(name+"ending_atoms").s).c_str());
        img_noatoms.load((measindex+settings->get(name+"ending_noatoms").s).c_str());
    }
    catch(...)
    {
        std::cout<<"loading err occured\n";
        img_atoms.clear();
        img_noatoms.clear();
        QMessageBox::warning(this,tr("opening error"),tr("error opening one of the images"));
        return;
    }

    //************************** CALCULATE IMG
    if(!(img_atoms.getArtist() == "MaD")){  //! if this is already modified raw data the background should not
        img_atoms-=img_atoms_back;         //be subtractet twice
        img_noatoms-=img_noatoms_back;

        for(int i=0;i<settings->get(name+"_rot90",0).i;i++){
            img_noatoms.rot90();
            img_atoms.rot90();
        }
        if(settings->get(name+"_flip_horizontal",0).i ==1){
            img_noatoms.flip_horizontal();
            img_atoms.flip_horizontal();
        }
    }
 //**********************************delete some files****************
    if((!(settings->get("take_as_background").i))&&(measindex != oldmeasindex)){
        try{
            if((settings->get("saveimg",(int)1).i & SAVEIMG_DELETE_RAW) != 0){
                remove((measindex+settings->get(name+"ending_atoms").s).c_str() );
                remove((measindex+settings->get(name+"ending_noatoms").s).c_str() );
            }
            if((settings->get("saveimg",(int)1).i & SAVEIMG_DELETE_MAT) != 0){
                remove((measindex+settings->get(name+".mat").s).c_str() );
            }
        }
        catch(...){}
    }
    oldmeasindex=measindex;   // this to enshure that we are not deleting a current measurement

    //*************** SAVE REDUCED SIZE RAW DATA WITH BACKGROUND SUBTRACTED ***************
    if(((settings->get("saveimg",(int)1).i & SAVEIMG_REPLACE_RAW) != 0)&&
       (!(settings->get("take_as_background").i))&&
       (!(img_atoms.getArtist() == "MaD"))){   //if image is already reduced, the artist is MaD

        Imagedata reduced_atoms;
        reduced_atoms = img_atoms;
        reduced_atoms.setROI(roi_replace_raw->pos(),roi_replace_raw->pos2());
        reduced_atoms.cutimg();
        reduced_atoms.save((measindex+settings->get(name+"ending_atoms").s).c_str());

        Imagedata reduced_noatoms;
        reduced_noatoms = img_noatoms;
        reduced_noatoms.setROI(roi_replace_raw->pos(),roi_replace_raw->pos2());
        reduced_noatoms.cutimg();
        reduced_noatoms.save((measindex+settings->get(name+"ending_noatoms").s).c_str());
    }

    standardAbsorptionCalcualtions();
    setImage();
    updateSidegraphs();
}


void Atomimg::standardAbsorptionCalcualtions()
{
    img_atoms.makePositive();    //just in case there are negative values due
    img_noatoms.makePositive();  //to background subtraction
    img=img_noatoms;
    img+=0.01;
    img_atoms += 0.1;  //in case something is 0 (black pixel)
    img/=img_atoms;
    img.ln();
    float cross=(*settings)[name+"crossection"].d;
    float detuning= (*settings)[name+"detuning"].d;
    float linewidth= (*settings)[name+"linewidth"].d;
    float wavelength=(*settings)[name+"wavelength"].d;

    float scatcross=cross*sq(wavelength)/(1+sq(2*detuning/linewidth));
    float scattcross_pixel = scatcross/sq((*settings)["m/pixel"].d);

    cout<<"cr"<<cross<<";"<<detuning<<";"<<linewidth<<";"<<wavelength
            <<";"<<scatcross<<":"<<scattcross_pixel<<"\n";
    img/=scattcross_pixel;  //gives atoms per pixel^2;

    if(settings->get(name+"_rot",0.0).d !=0){
        img.rot(settings->get(name+"_rot").d);
    }
    //background Area subtraction
    img.setROI(roi_background->pos(),roi_background->pos2());
    if((*settings)[name+"subtractRegion"].d==1)
        img-=img.sum()/(roi_background->width()*roi_background->height());
    //calculate sum over ROI
    img.setROI(roi_fit->pos(),roi_fit->pos2());
    results->store(name+"NROISum",img.sum());

}

///! internal function to display one of the images
void Atomimg::setImage()
{
    if(showimage==1) mainimg->setImage(&img_atoms);
    else if(showimage==2) mainimg->setImage(&img_noatoms);
    else if(showimage==3) mainimg->setImage(&img_atoms_back);
    else if(showimage==4) mainimg->setImage(&img_noatoms_back);
    else    mainimg->setImage(&img);
}


/*! called internally to reload background files
  loads the backgroundfiles for the atomimgs according to the settings.
  It also stores the names locally in order to be able to check whether the backgrounds have changed
  */
void Atomimg::reloadBackground()
{
    background_names=settings->get("backgroundfiles").vs;
    vector<string>bfiles = settings->get("backgroundfiles").vs; //filenames for backgroundimgs
    Imagedata a_back;  //temporal storage of background
    Imagedata noa_back; //...
    img_atoms_back.clear();
    img_noatoms_back.clear();
    if (bfiles.size()!=0)
    {
        for(unsigned int i =0;i<bfiles.size();i++){
            try{
                a_back.load((bfiles[i]+settings->get(name+"ending_atoms").s).c_str());
                noa_back.load((bfiles[i]+settings->get(name+"ending_noatoms").s).c_str());
                //logfile<<"loading: "<<(*settings)["backgroundfiles"].s+ending_noatoms<<endl;
            }
            catch(...)
            {
                std::cout<<"loading err occured\n";
                img_atoms_back.clear();
                img_noatoms_back.clear();
                QMessageBox::warning(this,tr("opening error"),tr("error opening background images"));
                return;
            }
            img_atoms_back += a_back;
            img_noatoms_back += noa_back;
        }
        img_atoms_back/=bfiles.size();
        img_noatoms_back/=bfiles.size();
    }
    reloadData();
}





/*! Calls fits and evaluates results
 */
void Atomimg::fitData()
{
    if(img.width()==0) return;
    if(img.height()==0) return;
    img.setROI(roi_fit->pos(),roi_fit->pos2());

    std::vector<float> fitparams;
    perform1DFit(img.colsum(),fitparams);
    std::vector<float> sidefitparams;
    perform1DFit(img.rowsum(),sidefitparams);

    float sx= results->get(name+"xRMS_radius_um").d/1000000;   //RMS-Gaussian Radius x-direction [m]
    float sy= results->get(name+"yRMS_radius_um").d/1000000;
    float N=((*results)[name+"xNfit"].d+(*results)[name+"yNfit"].d)/2; //Number of particles
    float m= settings->get(name+"mass_[amu]",0.001).d;
    m/=6.022e26;  //convert to SI mass

    float T= settings->get(name+"Temp[K]",3e-6).d; //Temperature SI units [K]
    float wx=sqrt(T*kb/m)/sx;
    float wy=sqrt(T*kb/m)/sy;
    assert(wx*wy>=0);
    float w=sqrt(wx*wy);
    results->store(name+"wx_calc[Hz]",wx/2/Pi);
    results->store(name+"wy_calc[Hz]",wy/2/Pi);

    results->store(name+"Nfit",N);
    float pd= N/(sx*sy*(sx+sy)/2)/exp(3/2.0*log(2*Pi))*1e-6;  //Peakdensity Atoms per cm^3
    results->store(name+"Peakdensity[cm-3]",pd);
    float psd=N*(hbar*w/kb/T)*(hbar*w/kb/T)*(hbar*w/kb/T);
    results->store(name+"PSDensity_sigmaT",psd);
    w=settings->get(name+"w_in[Hz]",100.0).d*2*Pi;
    psd=N*(hbar*w/kb/T)*(hbar*w/kb/T)*(hbar*w/kb/T);
    results->store(name+"PSDensity_wT",psd);
    updateParamBox();
    mainapp->dataeveluated();
}

/*! performs 1D fits on given y data
  the fit is selected according to the settings
   it also calls the drawing routines
*/
void Atomimg::perform1DFit(vector<float> fitdaty, vector<float> &fitparams)
{
    std:vector<float>  fitdatx(fitdaty.size(),0);  //generate x-values
    for(int i = 0;i<fitdatx.size();i++) fitdatx[i]=i;
    fitparams.clear();

    if(settings->get(name+"_fitfunction","gaussian").s == "bimodal"){
        fitparams= perform1DSpecialBimodal(fitdaty,fitdatx);
        plotfunction(fitparams,(int)roi_fit->width(),topgraph,"bgauss_fermi");
        if(fitparams[2]<=0) fitparams[2]*=(-1);  //fitfail
        storeBimodal(fitparams,(int)roi_fit->pos().x(),string("x"));
        //storeFitparams(fitparams,(int)roi_fit->pos().x(),string("x"));
    }
    else{
        fitparams.push_back(300);   // set nice starting values
        fitparams.push_back(fitdaty.size()/2);
        fitparams.push_back(fitdaty.size()/6);
        perform_fits(img.colsum(),fitparams);
        plotfunction(fitparams,(int)roi_fit->width(),topgraph,"gaussian");
        if(fitparams[2]<=0) fitparams[2]*=(-1);  //fitfail
        storeGaussian(fitparams,(int)roi_fit->pos().x(),string("x"));
    }
}

/*!  performs a special version of a bimodal fit according to a paper from 2009
  in Rev of Scientific instruments.
 returns the fitted params according to the following function(bose enhanced gaussian + thomas Fermi):
 bgauss+tf where:
 bgauss = x0*g_5/2(e^((x-x1)^2/x2))+x3
 tf=x4*max(0,(1-((i-x6)^2/x5^2)^(3/2))
  \param fitdaty y Valuues of the Data to be fitted
  \param fitdatx  x-Values of tzhe data to be fitted
  \param fitparams vector where the fitparams will be stored
*/
vector<float> Atomimg::perform1DSpecialBimodal(vector<float> fitdaty, vector<float> fitdatx)
{
    vector<float> fitparams;
    //first perform a gaussian fit to roughly find the cloud
    fitparams.push_back(300);
    fitparams.push_back(fitdaty.size()/2);
    fitparams.push_back(fitdaty.size()/6);
    perform_fits(fitdatx,fitdaty,fitparams,"gaussian");
    //perform a bimodal fit to find the bec and the pure thermal region
    //using a smoothened data to prevent the fit get stuck into a ripple
    fitparams[0]*=0.8;
    fitparams[2]*=1.44;
    fitparams.push_back(0);
    fitparams.push_back(fitparams[0]*0.3);
    assert(fitparams[2]>=0);
    fitparams.push_back(sqrt(fitparams[2])/1.2);
    perform_fits(fitdatx,smooth(fitdaty,10),fitparams,"bimodal");

    if(fitparams[5]<0) fitparams[5]=fitparams[5]*(-1);  //make it positive
    if(fitparams[1]<0) fitparams[1]=0;
    if(fitparams[1]>(float)fitdaty.size()) fitparams[1]=fitdaty.size();
    if(fitparams[5]>5000)fitparams[5]=fitdaty.size()/2;
    //now use only the outer parts of the data where is no BEC
    vector<float>thermalx(fitdatx);
    vector<float>thermaly(fitdaty);
    vector<float>thermalparams(fitparams);
    float cutscale=1.2;
    thermalparams.erase(thermalparams.begin()+4,thermalparams.end());

    int cutstart= (fitparams[1]-fitparams[5]*cutscale); //calculate the cutout region
    int cutend=   (fitparams[1]+fitparams[5]*cutscale);
    if(cutend  >(int)(fitdaty.size())) cutend  = fitdaty.size();
    if(cutstart>(int)(fitdaty.size())) cutstart= fitdaty.size();
    if(cutstart<0) cutstart=0;
    if(cutend  <0) cutend  =0;
    assert(fitparams[5]>=0);
    assert(cutend>=cutstart);
    thermaly.erase(thermaly.begin()+cutstart,thermaly.begin()+cutend);
    thermalx.erase(thermalx.begin()+cutstart,thermalx.begin()+cutend);
    perform_fits(thermalx,thermaly,thermalparams,"bose_gaussian");

    assert(fitparams.size()>=thermalparams.size());
    for(int i = 0;i<thermalparams.size();i++)
        fitparams[i]=thermalparams[i];

    plotfunction(thermalparams,(int)roi_fit->width(),topgraph,"bose_gaussian");

    //now that the thermal fraction is fitted it can be subtracted from the cloud
    vector<float>BECx(fitdatx);
    vector<float>BECy(fitdaty);
    for(int i =0;i<BECy.size();i++)
        BECy[i]-=   bose_gaussian((float)i,fitparams[0],fitparams[1],fitparams[2],fitparams[3]);
    //to reduce noise just throw away bad values and everythin in the thermal region

    float noisecut=0.25*max(BECy); //cutout (noise) data which is less than 25% of maximum

    for(int i = BECy.size()-1;i>=0;i--){
        if((BECy[i]<noisecut)||(i<cutstart)||(i>cutend)){
            BECy.erase(BECy.begin()+i);
            BECx.erase(BECx.begin()+i);
        }
    }
    vector<float>becparams(3,0);
    becparams[0]=fitparams[5]*0.7;  //the bimodal TF is too broad normally
    becparams[1]=fitparams[1];
    becparams[2]=fitparams[4];
    perform_fits(BECx,BECy,becparams,"thomas_fermi");

    fitparams[5]=becparams[0];
    fitparams[4]=becparams[2];
    fitparams.push_back(becparams[1]);
    return fitparams;
}




/*! plot a function according to the parameters resulting from the fit
\param p parameter of the function to plot
\param npoints number of points in x-Direction to plot
\param graph   plotting Engine
\param string fitname  lets you select the type of function to plot
*/
void Atomimg::plotfunction(const vector<float> &p,
                           int npoints,  GraphPlotter* graph,string fitname)
{
    double x0=p[0]; double x1=p[1]; double x2=p[2]; double x3=0;
    if(p.size()>3) x3=p[3];
    if((x0!=x0 )||(x1!=x1 )||(x2!=x2 )||(x3!=x3 ))return;  //check for NaN
    if(x1>npoints )return;
    if(x1<0 )return;
    if(x2==0)return;
    std::vector<float> fitdat;
    QColor color=QColor(255,0,0);
    if(fitname == "bose_gaussian"){
        for(int i = 0;i<npoints;i++)
            fitdat.push_back(bose_gaussian((float)i,x0,x1,x2,x3));
    }
    if(fitname == "bgauss_fermi"){
        color=QColor(0,0,255);
        assert(p.size()>=7);
        for(int i = 0;i<npoints;i++)
            fitdat.push_back(bose_gaussian((float)i,x0,x1,x2,x3)+thomas_fermi1d((float)i,p[5],p[6],p[4]));
    }
    else for(int i = 0;i<npoints;i++)   //plot a gaussian
        fitdat.push_back(x0 * exp(-((float)i-x1)*(i-x1)/x2)+x3);

    Datacurve* sidecurv =new Datacurve(fitdat);
    sidecurv->setPen(color);
    graph->addCurve(sidecurv);
}

/*! plots data given as two verctors
\param datax x Component of the data points
\param datay  y-component of data points
\param graph  Plotting Engine
*/
void Atomimg::plotxy(const vector<float> &datax,const vector<float> &datay,
                     GraphPlotter* graph)
{
    assert(datax.size()==datay.size());
    if(datax.size()==0) return;
    assert(max(datax)<4000);
    assert(max(datax)>=1);
    vector<float> dat((int)max(datax)+1,0);
    for(int i = 0; i<datax.size();i++){
        if(datax[i]>=0)
            dat[(int)datax[i]]=datay[i];
        assert((int)datax[i]<(int)dat.size());
        assert(datax[i]>=0);
    }
    Datacurve* sidecurv =new Datacurve(dat);
    sidecurv->setPen(QColor(0,255,0));
    graph->addCurve(sidecurv);
}



//store data
void Atomimg::storeGaussian(const vector<float>& fitparams,int roi_offset
                            ,string fitname)
{
    //fitfail
    assert(fitparams[2]>=0);
    results->store(name+fitname+"height_at/pix",fitparams[0]);
    results->store(name+fitname+"center_pix",fitparams[1]+roi_offset);
    results->store(name+fitname+"RMS_radius_pix",(sqrt(fitparams[2]/2)));
    results->store(name+fitname+"RMS_radius_um",sqrt(fitparams[2]/2)*(*settings)["m/pixel"].d*1.0e6);
    // results->store(name+fitname+"sigma_mum_sq2",(fitparams[2]/2)*(*settings)["m/pixel"].d*1.0e6*(2));
    results->store(name+fitname+"Nfit",sqrt(fitparams[2]/2)*fitparams[0]*sqrt(2*M_PI));

}

/*! interprete the fitting results and store the corresponding data
*/
void Atomimg::storeBimodal(const vector<float>& fitparams,int roi_offset
                           ,string fitname)
{
    assert(fitparams[2]>=0);
    float nbec= 16/15.0*fitparams[4]*fitparams[5];
    float nth= sqrt(2*Pi)*g_2_1*fitparams[0]*sqrt(fitparams[2]/2);
    float becfraction;
    results->store(name+fitname+"NBEC",nbec);
    results->store(name+fitname+"NTh",nth);
    results->store(name+fitname+"BEC_Fract",nbec/(nth+nbec));
    results->store(name+fitname+"center_pix",fitparams[1]+roi_offset);
    results->store(name+fitname+"RMS_radius_pix",sqrt(fitparams[2]/2));
    results->store(name+fitname+"RMS_radius_um",sqrt(fitparams[2]/2)*(*settings)["m/pixel"].d*1.0e6);
    results->store(name+fitname+"Nfit",(fitparams[2]/2)*fitparams[0]*(2*M_PI));
}

/*! just store the fitting parameters as they are  */
void Atomimg::storeFitparams(const vector<float>& fitparams,int roi_offset
                             ,string fitname)
{
    ostringstream os;
    for(int i = 0;i< fitparams.size();i++){
        os.str("");
        os<<i;
        assert(!(fitparams[i]!=fitparams[i]));
        results->store(name+fitname+os.str(),fitparams[i]);
    }
}

/*! save the displayed image as png or other image file */
void Atomimg::savePNG()
{
    mainimg->toFile(measindex+name+settings->get("saveimg_suffix",".png").s);
}

void Atomimg::updateParamBox()
{
    parambox->reload();
}

void Atomimg::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(subtractregionAct);
    menu.addAction(selectimgAct);
    menu.addAction(autoscaleAct);
    menu.exec(event->globalPos());
}

/*! creates actions for the context menu (called by init routine)*/
void Atomimg::createActions()
{
    subtractregionAct = new QAction(tr("&SubtractRegion"), this);
    subtractregionAct->setCheckable(true);
    subtractregionAct->setChecked((settings->get(name+"subtractRegion",0).i==1));
    subtractregionAct->setShortcut(tr("Ctrl+S"));
    subtractregionAct->setStatusTip(tr("Region to generate extra value, which is subtracted"));
    connect(subtractregionAct, SIGNAL(toggled(bool)), this, SLOT(toggleSubtract(bool)));


    selectimgAct = new QAction(tr("&select img to display"), this);
    selectimgAct->setStatusTip(tr("lets you display also background or one of the imgs"));
    connect(selectimgAct, SIGNAL(triggered()),
            this, SLOT(selectImg()));
    autoscaleAct = new QAction(tr("&autoscale the colors"), this);
    autoscaleAct->setStatusTip(tr("displays the data in a way the maximum color equals the max value"));
    connect(autoscaleAct, SIGNAL(triggered()),
            mainimg, SLOT(autoscaleColor()));
}


//************ SLOTS

//! called internally to update the addtional graphs
void Atomimg::updateSidegraphs()
{
    img.setROI(roi_fit->pos(),roi_fit->pos2());
    topgraph->clear();
    topgraph->setHScaling(500.0/(roi_fit->pos2().x()-roi_fit->pos().x()+1e-30));
    topgraph->addCurve(new Datacurve(img.colsum()));
    toptrace->fitInView(topgraph->sceneRect());

    sidegraph->clear();
    sidegraph->setHScaling(500.0/(roi_fit->pos2().y()-roi_fit->pos().y()+1e-30));
    sidegraph->addCurve(new Datacurve(img.rowsum()));
    sidetrace->rotate(-90);    //I don't know why but rotating back avoids
    QRectF r=sidegraph->sceneRect();   //getting wrong zooming parameters
    sidetrace->fitInView(r.y(),r.x(),r.height(),r.width());
    sidetrace->rotate(90);

    if(settings->get("noautofit",0).i==0) fitData();
}


//! subtract region should be used or not
void Atomimg::toggleSubtract(bool toggled)
{
    settings->store(name+"subtractRegion",toggled);
}
//! shows a dialog in which the user can select which Image to display
/*! this function is available by Mouseclick with the right button onto an image
*/
void Atomimg::selectImg()
{
    QStringList items;
    items << tr("standard data") << tr("image with atoms")
            << tr("image without atoms") << tr("background 'atoms'")
            << tr("background ' no atoms'");

    bool ok;
    QString item = QInputDialog::getItem(this, tr("QInputDialog::getItem()"),
                                         tr("Season:"), items, 0, false, &ok);
    if (ok && !item.isEmpty())
    {
        if(item == tr("standard data")) showimage=0;
        if(item == tr("image with atoms")) showimage=1;
        if(item == tr("image without atoms")) showimage=2;
        if(item == tr("background 'atoms'")) showimage=3;
        if(item == tr("background ' no atoms'")) showimage=4;
    }
    setImage();
}


//! synchronzation of markers
void Atomimg::markerpos_changed(string name,QPointF pos)
{
    mainapp->changemarker(QString(name.c_str()),pos);
}

/*! synchronization of markers
  \param name name of the marker that is changed
  \param pos  position the marker is moved to
  this function is connected to a signal with the same name emitted by the mainapplication
  the emission of the signal is triggered by calling mainapp->changemarker
  if the name of the marker if "...marker.." (not fitstart) then this marker is moved to the
  same/corresponding position
*/
void Atomimg::markerchanged(QString name, QPointF pos)
{
    if((marker != NULL)&&(name.toStdString().find("marker")!=string::npos))
        marker->setPos(pos);

}



/**
 *  Copyright (C) 2007 Lorenzo Bossi and Stefano Massera
 *
 *  This file is part of Fractals ;)
 *
 *  Fractals ;) is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published
 *  by the Free Software Foundation, either version 2 of the License,
 *  or (at your option) any later version.
 *
 *  Fractals ;) is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 *  See the GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Fractals ;)
 *  If not, see <http://www.gnu.org/licenses/>.
 */


#include "codice.h"

#include <math.h>
#include <complex>

using namespace immagine;

typedef std::complex<double> double_complex;

double power(double b,double e){
  return exp(e*log(b));
}


#ifdef _WIN32

long int lround  (double x){
  return (long) x; //TODO fixme
}

#endif

template <typename T>
class dyn_array{
  T *data;
public:
  dyn_array(int size){
    data = new T[size];
  }
  ~dyn_array(){
    delete[] data;
  }
  const T& operator[](int pos) const {
    return data[pos];
  }
  T& operator[](int pos) {
    return data[pos];
  }
};

template <typename T>
class matrix{
  int size;
  dyn_array< dyn_array<T>* > data;
public:
  matrix(int dimx, int dimy): size(dimx), data(dimx){
    for (int i = 0 ; i<dimx ; i++){
      data[i]=new dyn_array<T>(dimy);
    }
  }
  ~matrix(){
    for (int i = 0 ; i<size ; i++){
      delete data[i];
    }
  }
  const dyn_array<T>& operator[](int pos) const {
    return *data[pos];
  }
  dyn_array<T>& operator[](int pos) {
    return *data[pos];
  }

};


codice::codice(interfaces::input *ii, interfaces::output *oo){
    i=ii;
    o=oo;
    frattale=grigi=colore=proiezione=NULL;
}

void codice::generaFrattale(){
    double xwidth = i->getWidth();
    double yheight= i->getHeight();

    if (frattale){
      delete frattale;
      frattale=NULL;
    }
    if (grigi){
      delete grigi;
      grigi=NULL;
    }
    if (colore){
      delete colore;
      colore=NULL;
    }
    if (proiezione){
      delete proiezione;
      proiezione=NULL;
    }

    frattale=new image_2d ((int)xwidth,(int)yheight);

    o->msgGenera();
    o->progress (1);

    //creazione del frattale
    const int iteratelimit = 104; //500;
    const double modulelimit = 4;//20;
    
    double_complex min(i->getXMin(),i->getYMin());
    double_complex max(i->getXMax(),i->getYMax());
    double_complex c(i->getCR(),i->getCI());

    for(int a=0;a<frattale->get_x();a++){
      for(int b=0;b<frattale->get_y();b++){
	double_complex x(a*((max.real()-min.real())/xwidth)+min.real(),b*((max.imag()-min.imag())/yheight)+min.imag());
	if (!i->useParC())
	  c=x;
	int i;
	for( i=0; (abs(x)<modulelimit) && (i<iteratelimit) ;i++) {
	  x=x*x+c;
	}
	int col=pixel::maxcolor-lround((double)pixel::maxcolor*((double)i/(double)iteratelimit));
	/*
	if (i < iteratelimit)
	  col=(i%pixel::maxcolor);
	else
	  col=0;
	*/
	frattale->get_px((int)a,(int)b).set(col);
      }
      o->progress ((int)(100*a/xwidth));
    }
    o->progress (100);
    o->finishMsg();
}

void codice::correggiGrigi(){
  if (!frattale)
    return;

  if (grigi){
    delete grigi;
    grigi=NULL;
  }

  if (!i->correzioneGrigi())
    return;

  o->msgGrigi();
  o->progress (1);

  //sistemo i livelli di grigio applicando un filtro esponenziale
  /*
    y=a(e^(x/b)-1)
    esponenziale standard: a=1; b=255/ln(256);
    quindi
    double b = 2
    y= 255/exp(b*255)*exp(b*x)-(255/exp(b*255));
  */
 
  immagine::image_2d *img=frattale;
  
  grigi=new image_2d (img->get_x(),img->get_y());
  
  //virtual int correzioneGrigiVal() const =0;
  double aa,bb;
  
  aa=100-(i->correzioneGrigiVal()*90/100);
  bb=255.0/(exp(255.0/aa)-1);

  for(int a=0;a<img->get_x();a++){
    for(int b=0;b<img->get_y();b++){
      double x=img->get_px(a,b)[pixel::r];
      double y=bb*(exp(x/aa)-1);
      grigi->get_px(a,b).set((int)y);
    }
    o->progress ((int)(100*a/img->get_x()));
  }
  o->progress (100);
  o->finishMsg();
}

void codice::colora(){
  if (!frattale)
    return;
  if (colore){
    delete colore;
    colore=NULL;
  }

  if (!i->applicaColore())
    return;
  immagine::image_2d *img=frattale;
  if(grigi)
    img=grigi;
  
  o->msgColori();
  o->progress (1);
  colore=new image_2d (img->get_x(),img->get_y());
  int red[256],green[256],blue[256];
  i->getColori(red,green,blue);
  for(int a=0;a<img->get_x();a++){
    for(int b=0;b<img->get_y();b++){
      int gg=img->get_px(a,b)[pixel::r];
      colore->get_px(a,b).set(red[gg],green[gg],blue[gg]);
    }
    o->progress ((int)(100*a/img->get_x()));
  }
  o->progress (100);
  o->finishMsg();
}

void codice::proietta(){
  if (!frattale)
    return;
  
  if(proiezione){
    delete proiezione;
    proiezione=NULL;
  }

  if(!i->proiezione3d())
    return;

  o->msg3D();
  o->progress (1);

  const int altezzaScalino=i->altezzaScalini();        //30; //altezza immagine
  const int scalini=i->numeroScalini();             //4; //altezza immagine

  immagine::image_2d *base=frattale;
  immagine::image_2d *reale=frattale;
  if(grigi){
    base=grigi;
    reale=grigi;
  }
  if(colore)
    reale=colore;
  
  pixel parete(i->getPareteR(),i->getPareteG(),i->getPareteB());

  /*
j /\
  |
  |
  |
  +-------> i
       _
       /| y
z /\  /   
  |  /
  | /
  |/
  +-------> x
  

  i=x+sqrt(2)y/4;
  j=z+sqrt(2)y/4;
  */

  int dimx=base->get_x();
  int dimy=base->get_y();
  int dimz=scalini*altezzaScalino;
  int dimAdd=(int)ceil(dimy*sqrt(2.0)/4.0);
  proiezione=new image_2d(dimx+dimAdd,dimz+dimAdd);  
  
  matrix<int> altezze(dimx,dimy);
  for (int i=0;i<dimx;i++){
    for (int j=0;j<dimy;j++){
      altezze[i][j]=(pixel::maxcolor-base->get_px(i,j).gray_value())*scalini/pixel::maxcolor*altezzaScalino;
    }
  }

  //rilievo
  for (int i=0;i<proiezione->get_x();i++){
    for (int j=0;j<proiezione->get_y();j++){
      int x=-1;
      int y=-1;
      int z=-1;
      bool testa=true;
      for (y=0;y<dimy;y++){
	x=lround(i-(y*sqrt(2.0)/4.0));
	z=lround(j-(y*sqrt(2.0)/4.0));
	if (base->in_image(x,y) && z>=0 ){
	  int h=altezze[x][y];
	  if (h>z){
	    proiezione->get_px(i,j)=parete;
	    testa=false;
	    break;
	  }
	  if (h==z){
	    proiezione->get_px(i,j)=reale->get_px(x,y);
	    testa=true;
	    break;
	  }
	}
      }
      //a questo punto il pixel (x,y,z) e' stato proiettato su (i,j)
      //calcolo l'ombra
      int xx,yy,zz;
      if (!testa && base->in_image(x,y)){
	int h1=altezze[x+1][y];
	int h2=altezze[x][y-1];
	if (h1<z){//se non ha nulla a destra lo scurisco
	  proiezione->get_px(i,j).varia_luminosita(0.9);
	  if(h2<z){//ma se non ha nulla nemmeno davanti non troppo
	    proiezione->get_px(i,j).varia_luminosita(1.05);
	  } 
	}
      }
      else{
	if (base->in_image(x,y)){
	  for(xx=x-1,yy=y-1,zz=z+2;base->in_image(xx,yy) && zz<scalini*altezzaScalino; xx--,yy--,zz+=2 ){
	    int h=altezze[xx][yy];
	    if(h>z){
	      proiezione->get_px(i,j).varia_luminosita(0.9);
	      break;
	    }
	  }
	}
      }
    }
    o->progress ((int)(100*i/proiezione->get_x()));
  }
  o->progress (100);
  o->finishMsg();
  
}

void codice::stampa(){
  if (!frattale)
    return;
  immagine::image_2d *img=frattale;
  if(grigi)
    img=grigi;
  if(colore)
    img=colore;
  if(proiezione)
    img=proiezione;
  
  //stampa a video
  o->createImage(img->get_x(),img->get_y());
  int correzione=img->get_y()-1;
  for(int a=0;a<img->get_x();a++){
    for(int b=0;b<img->get_y();b++){
      o->setPixel(a,correzione-b,img->get_px(a,b)[pixel::r],img->get_px(a,b)[pixel::g],img->get_px(a,b)[pixel::b]);
    }
  }
  o->redrawImage();
}


int &pixel::operator[](int n){
  if(n>dim || n<0)
    return colore[0];
  return colore[n];
}
int pixel::operator[](int n) const{
  if(n>dim || n<0)
    return colore[0];
  return colore[n];
}
void pixel::set (int rr,int gg,int bb){
  colore[r]=rr;
  colore[g]=gg;
  colore[b]=bb;
}
void pixel::set (int gray){
  set(gray,gray,gray);
}
pixel::pixel(int rr,int gg,int bb){
  colore[0]=0;
  colore[r]=rr;
  colore[g]=gg;
  colore[b]=bb;
}
pixel::pixel(){
  for (int n=0;n<dim;n++)
    colore[n]=maxcolor;
}
pixel::pixel(const pixel &p){
  for (int n=0;n<dim;n++)
    colore[n]=p.colore[n];
}
pixel &pixel::operator=(const pixel &p){
  for (int n=0;n<dim;n++)
    colore[n]=p.colore[n];
  return *this;
}
int pixel::operator==(const pixel &p) const{
  for (int n=1;n<dim;n++)
    if (colore[n]!=p.colore[n])
      return 0;
  return 1;
}
int pixel::is_white() const{
  return (colore[r]>=white && colore[g]>=white && colore[b]>=white);
}
int pixel::gray_value() const{
  //Y = 0.3*R + 0.59*G + 0.11*B
  return lround(0.3*colore[r]+0.59*colore[b]+0.11*colore[g]);
}
void pixel::varia_luminosita(double percentuale){
  //Y = 0.3*R + 0.59*G + 0.11*B
  for (int i=1;i<=b;i++){
    colore[i]=lround(colore[i]*percentuale);
    if (colore[i]>maxcolor)
      colore[i]=maxcolor;
  }
}

image_2d::image_2d(int xx,int yy):dimx(xx),dimy(yy){
  img=new pixel*[dimx];
  for (xx=0;xx<dimx;xx++){
    img[xx]=new pixel[dimy];
  }
}
image_2d::~image_2d(){
  for (int xx=0;xx<dimx;xx++){
    delete [] img[xx];
  }
  delete [] img;
}
pixel &image_2d::get_px(int x,int y){
  if (in_image(x,y))
    return img[x][y];
  return fake_pixel;
}
const pixel &image_2d::get_px(int x,int y) const{
  if (in_image(x,y))
    return img[x][y];
  return fake_pixel;
}

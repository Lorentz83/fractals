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


#ifndef __INTERFACES_H
#define __INTERFACES_H

namespace interfaces{

  class input{
    //frattali
  public:
    virtual ~input(){}
    virtual int getWidth() const =0;
    virtual int getHeight() const =0;
    //frattali
    virtual double getXMax() const =0;
    virtual double getYMax() const =0;
    virtual double getXMin() const =0;
    virtual double getYMin() const =0;
    virtual bool useParC() const =0;
    virtual double getCR() const =0;
    virtual double getCI() const =0;
    //correzione grigi
    virtual bool correzioneGrigi() const =0;
    virtual int correzioneGrigiVal() const =0; // da 1 a 100
    //colore
    virtual bool applicaColore() const =0;
    virtual void getColori(int*,int*,int*) const =0; //modifica 3 array di 256 elementi precedentemente allocati
    //3d
    virtual bool proiezione3d() const =0;
    virtual int numeroScalini() const =0;
    virtual int altezzaScalini() const =0;
    virtual int getPareteR() const =0;
    virtual int getPareteG() const =0;
    virtual int getPareteB() const =0;
  };

  class output{
  public:

    virtual void msgGenera()=0;
    virtual void msgGrigi()=0;
    virtual void msg3D()=0;
    virtual void msgColori()=0;

    virtual void progress (int)=0;
    virtual void finishMsg()=0;
    //image
    virtual void createImage(int,int)=0;
    virtual void setPixel(int,int,int,int,int)=0;
    virtual void setPixel(int x,int y,int g){
      setPixel(x,y,g,g,g);
    }
    virtual void fillImage(int,int,int)=0;
    virtual void redrawImage()=0;
    
    virtual ~output(){}
  };

  class event{
  public:
    virtual void generaFrattale()=0;
    virtual void correggiGrigi()=0;
    virtual void colora()=0;
    virtual void proietta()=0;
    virtual void stampa()=0;

    virtual ~event(){}
  };
}

#endif

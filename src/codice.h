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


#ifndef __CODICE_H
#define __CODICE_H

#include "interfaces.h"

namespace immagine{

  class pixel{
  public:
    enum{r=1,g,b,dim};
  private:
    int colore[dim];
    static const int white=255;
  public:
    static const int maxcolor=255;
    int &operator[](int n);
    int operator[](int n) const;
    void set (int rr,int gg,int bb);
    void set (int gray);
    pixel(int rr,int gg,int bb);
    pixel();
    pixel(const pixel &p);
    pixel &operator=(const pixel &p);
    int operator==(const pixel &p) const;
    int is_white() const;
    int gray_value() const;
    void varia_luminosita(double percentuale);
  };
  
  class image_2d{
    const int dimx,dimy;
    pixel **img;
    pixel fake_pixel;//usato se gli indici sono fuori dall'immagine
  public:
    image_2d(int xx,int yy);
    ~image_2d();
    pixel &get_px(int x,int y);
    const pixel &get_px(int x,int y) const;
    bool in_image(int x,int y) const{
      return !(x<0 || y<0 || x>=dimx || y>=dimy );
    }
    int get_x() const{
      return dimx;
    }
    int get_y() const{
      return dimy;
    }
  };
    
}

class codice: public interfaces::event{
 protected:
  interfaces::input *i;
  interfaces::output *o;
  
  immagine::image_2d *frattale,*grigi,*colore,*proiezione;
  //immagine::image_3d *frattale3d;
  
 public:
  codice(interfaces::input *, interfaces::output *);
  void generaFrattale();
  void correggiGrigi();
  void colora();
  void proietta();

  void stampa();
};


#endif

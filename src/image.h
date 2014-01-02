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


#ifndef __IMAGE_H
#define __IMAGE_H

#include<QImage>

class imageFromFile{
 private:
  QImage img;
  
 public:
  static const int defPx=0;
  enum{r=1,g=2,b=3};
  imageFromFile(const char* filename):img(filename){
  }
  imageFromFile(const QString &filename):img(filename){
  }
  int getPixel(int x, int y, int col){
    if (img.isNull())
      return -1;
    if(x>img.width()-1 || x<0 || y>img.height()-1 || y<0)
      return defPx;
    if(col==r)
      return qRed(img.pixel(x,y));
    if(col==g)
      return qGreen(img.pixel(x,y));
    if(col==b)
      return qBlue(img.pixel(x,y));
    return defPx;
  }
};


#endif


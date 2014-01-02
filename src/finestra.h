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


#ifndef __FINESTRA_H
#define __FINESTRA_H

#include "ui_finestra.h"
#include <QtGui>

#include "interfaces.h"
#include "codice.h"

#include "image.h"

class datiFrattale{
 public:
  datiFrattale(){
    xMin=-1;
    xMax=1;
    yMin=-1;
    yMax=1;
    c=false;
    cR=0;
    cI=0;
  }
  QString nome;
  double xMin;
  double xMax;
  double yMin;
  double yMax;
  bool c;
  double cR;
  double cI;
};

QTextStream& operator>>(QTextStream& ,datiFrattale& );

class scrollableRasterImage: public QScrollArea{
  Q_OBJECT
private:
  QImage *image;
  QLabel imageLabel;
public:
  scrollableRasterImage(QWidget *parent):QScrollArea(parent){
    setMinimumSize(200,200);
    setAlignment(Qt::AlignCenter);
    image=NULL;
  }
  ~scrollableRasterImage(){
    if (image)
      delete image;
  }
  void createImage(int x,int y){
    if (image) 
      delete image;
    image=new QImage (x,y,QImage::Format_RGB32);
  }
  void fillImage(int r,int g,int b){
    image->fill(qRgb(r,g,b));
  }
  void setPixel(int x,int y,int r,int g,int b){
    image->setPixel(x,y,qRgb(r,g,b));
  }
  void redrawImage(){
    imageLabel.setPixmap(QPixmap::fromImage(*image));
    imageLabel.resize(imageLabel.pixmap()->size());
    setWidget(&imageLabel);
  }
  bool saveImage(const QString &fileName, const char *format=0, int quality=-1 ){
    if (image) 
      return image->save(fileName,format,quality);
    return false;
  }
};


class finestra : public QMainWindow, private Ui::MainWindow, public interfaces::input, public interfaces::output{
  Q_OBJECT  

public:
  static const QString colori;
  static const QString frattaliDef;

  finestra(QWidget *parent = 0);

  void setProgress(int);
  void resetProgress();
  void setStatusBar(const QString&,int=0);
  void resetStatusBar();

public slots:
  void generaFrattale();
  void applicaAggiunte();

private slots:
  void correggiFrattaleSpin(double);
  void valioriDefault(const QString &);
  void aboutWindow();
  void aboutQt();
  void saveAs();

protected:
  void closeEvent(QCloseEvent*);

  scrollableRasterImage *image;
  codice c;
  std::vector<datiFrattale> valoriFrattali;

public:
  //input
  int getWidth() const {
    return imgWidth->value();
  }
  int getHeight() const {
    return imgHeight->value();
  }
  double getXMax() const {
    return frattaliXMax->value();
  }
  double getYMax() const {
    return frattaliYMax->value();
  }
  double getXMin() const {
    return frattaliXMin->value();
  }
  double getYMin() const {
    return frattaliYMin->value();
  }
  bool useParC() const{
    return frattaliC->isChecked();
  }
  double getCR() const {
    return frattaliCR->value();
  }
  double getCI() const {
    return frattaliCI->value();
  }

  bool correzioneGrigi() const{
    return aggCorrezioneGrigi->isChecked();
  }
  int correzioneGrigiVal() const{
    return aggCorrezioneGrigiVal->value();
  }
  //colore
  bool applicaColore() const{
    return aggColore->isChecked();
  }
  void getColori(int *r,int *g,int *b) const{
    //modifica 3 array di 256 elementi precedentemente allocati
    imageFromFile iCol(QCoreApplication::applicationDirPath()+QDir::separator()+
                       colori+QDir::separator()+aggColoreSel->currentText());
    for (int gg=0;gg<256;gg++){    
      *r++=iCol.getPixel(gg,1,iCol.r);
      *g++=iCol.getPixel(gg,1,iCol.g);
      *b++=iCol.getPixel(gg,1,iCol.b);
    }      
  }
  bool proiezione3d() const{
     return treD->isChecked();
  }
  int numeroScalini() const{
    return treDScalini->value();
  }
  int altezzaScalini() const{
    return treDAltezza->value();
  }
  int getPareteR() const{
    return treDR->value();
  }
  int getPareteG() const{
    return treDG->value();
  }
  int getPareteB() const{
    return treDB->value();
  }

  //output
 public:
  
  void msgGenera(){
    setStatusBar(tr("Generating fractal"));
  }
  void msgGrigi(){
    setStatusBar(tr("Correcting gray scale"));
  }
  void msgColori(){
    setStatusBar(tr("Applying color"));
  }
  void msg3D(){
    setStatusBar(tr("Creating 3D image"));
  }

  void finishMsg(){
    //resetStatusBar();
    setStatusBar(tr("Finish"),1000);
    resetProgress();
  }
  virtual void progress (int n){
    setProgress(n);
  }
  virtual void createImage(int x,int y){
    image->createImage(x,y);
  }
  virtual void setPixel(int x,int y,int r,int g,int b){
    image->setPixel(x,y,r,g,b);
  }
  virtual void fillImage(int r,int g,int b){
    image->fillImage(r,g,b);
  }
  virtual void redrawImage(){
    image->redrawImage();
  }

};



#endif

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


#include "finestra.h"

const QString finestra::colori("colori");
const QString finestra::frattaliDef("frattali");

QTextStream& operator>>(QTextStream& in,datiFrattale& f){
  QString b;
  in>>f.xMin;
  in>>f.xMax;
  in>>f.yMin;
  in>>f.yMax;
  in>>b;
  if (b.compare("true")==0)
    f.c=true;
  else
    f.c=false;
  in>>f.cR;
  in>>f.cI;
  return in;
}

finestra::finestra(QWidget *):c(this,this){
  setupUi(this);
  resetProgress();
  showMaximized();

  //menu
  connect(action_Esci, SIGNAL(triggered()), this, SLOT( close() ));
  connect(action_About, SIGNAL(triggered()), this, SLOT( aboutWindow() ));
  connect(actionAbout_Qt, SIGNAL(triggered()), this, SLOT( aboutQt() ));
  connect( action_Salva_immagine, SIGNAL( triggered() ), this, SLOT( saveAs() ) );

  //spinbox
  connect( frattaliXMin, SIGNAL( valueChanged(double) ), this, SLOT( correggiFrattaleSpin(double) ) );
  connect( frattaliXMax, SIGNAL( valueChanged(double) ), this, SLOT( correggiFrattaleSpin(double) ) );
  connect( frattaliYMin, SIGNAL( valueChanged(double) ), this, SLOT( correggiFrattaleSpin(double) ) );
  connect( frattaliYMax, SIGNAL( valueChanged(double) ), this, SLOT( correggiFrattaleSpin(double) ) );
  
  //pulsanti
  connect( aggApplica, SIGNAL( clicked() ), this, SLOT( applicaAggiunte() ) );
  connect( frattaliOk, SIGNAL( clicked() ), this, SLOT( generaFrattale() ) );

  //combo
  connect( frattaliDefault, SIGNAL( currentIndexChanged (const QString&) ), this, SLOT( valioriDefault(const QString&) ) );


  imgWidth->setValue(800);
  imgHeight->setValue(600);

  //riempio combo box colori
  QDir dirColori(QCoreApplication::applicationDirPath()+QDir::separator()+colori);
  if (dirColori.exists()){
    dirColori.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
    dirColori.setSorting(QDir::Name);
    QFileInfoList list = dirColori.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
      QFileInfo fileInfo = list.at(i);
      if (fileInfo.suffix()=="png"){
	aggColoreSel->addItem(fileInfo.baseName());
      }
    }
  }
  
  //riempio combo box frattali
  QDir dirFrattali(QCoreApplication::applicationDirPath()+QDir::separator()+frattaliDef);
  if (dirFrattali.exists()){
    dirFrattali.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
    dirFrattali.setSorting(QDir::Name);
    QFileInfoList list=dirFrattali.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
      QFileInfo fileInfo = list.at(i);
      if (fileInfo.suffix()=="frt"){
	QFile data(fileInfo.filePath());
	if ( data.open(QIODevice::ReadOnly) ) {
	  QTextStream in(&data);
	  datiFrattale f;
	  in>>f;
	  f.nome=fileInfo.baseName();
	  frattaliDefault->addItem(f.nome);
	  valoriFrattali.push_back(f);
	  data.close();
	}
      }
    }
  }
  image=new scrollableRasterImage(frame);
  vboxLayout->addWidget(image);
}

void finestra::valioriDefault(const QString & nome){
  for (unsigned int i=0;i<valoriFrattali.size();i++){
    if (nome==valoriFrattali[i].nome){
      frattaliXMin->setValue(valoriFrattali[i].xMin);
      frattaliXMax->setValue(valoriFrattali[i].xMax);
      frattaliYMin->setValue(valoriFrattali[i].yMin);
      frattaliYMax->setValue(valoriFrattali[i].yMax);
      frattaliC->setChecked(valoriFrattali[i].c);
      frattaliCR->setValue(valoriFrattali[i].cR);
      frattaliCI->setValue(valoriFrattali[i].cI);
      return;
    }
  }
  //default
  frattaliXMin->setValue(-1);
  frattaliXMax->setValue(1);
  frattaliYMin->setValue(-1);
  frattaliYMax->setValue(1);
  frattaliC->setChecked(false);
  frattaliCR->setValue(0);
  frattaliCI->setValue(0);
}

void finestra::setProgress(int n){
  progressBar->setValue(n);
  setCursor(Qt::WaitCursor);
}

void finestra::resetProgress(){
  progressBar->reset();
  setCursor(Qt::ArrowCursor);
}

void finestra::setStatusBar(const QString &c,int time){
  statusbar->showMessage(c,time);
}
void finestra::resetStatusBar(){
  statusbar->clearMessage();
}

void finestra::correggiFrattaleSpin(double){
  frattaliXMin->setMaximum(frattaliXMax->value());
  frattaliXMax->setMinimum(frattaliXMin->value());

  frattaliYMin->setMaximum(frattaliYMax->value());
  frattaliYMax->setMinimum(frattaliYMin->value());
}

void finestra::generaFrattale(){
  c.generaFrattale();
  c.stampa();
}

void finestra::applicaAggiunte(){//controllare perche' non stampa ogni passo
  c.correggiGrigi();
  c.colora();
  c.proietta();
  c.stampa();
}

void finestra::aboutWindow(){
  QMessageBox::about(this, tr("About Fractals ;)"),
		     tr("<p><b>Fractals ;)</b>, Copyright (C) 2007 Lorenzo Bossi and Stefano Massera</p>"
			"<p>Fractals ;) is free software: you can redistribute it and/or modify"
			"it under the terms of the GNU General Public License as published"
			"by the Free Software Foundation, either version 2 of the License,"
			"or (at your option) any later version.</p>"
			"<p>Fractals ;) is distributed in the hope that it will be useful, but"
			"WITHOUT ANY WARRANTY; without even the implied warranty of"
			"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. "
			"See the <a href=\"http://www.gnu.org/licenses/gpl-2.0.html\">GNU General Public License</a> for more details.</p>"
			));
}

void finestra::aboutQt(){
  QMessageBox::aboutQt(this, tr("About Qt"));
}
void finestra::closeEvent(QCloseEvent* e){
  QWidget::closeEvent(e);
    
  // if (QMessageBox::question(this," ",tr("Are you sure?"),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
  //   QWidget::closeEvent(e);
  // else
  //   e->ignore();
}
void finestra::saveAs(){
  static QString dir(QDir::homePath());
  QString path;
  
  QFileDialog dialog(this);
  QStringList filters;

  QStringList exts;
  exts<<"png"<<"jpg"<<"bmp";

  for (int i=0;i<exts.size();i++){
    filters<<tr("%1 image").arg(exts.at(i).toUpper())+" (*."+exts.at(i)+")";
  }
  
  dialog.setWindowTitle(tr("Save as..."));
  dialog.setFilters(filters);
  dialog.setDirectory(dir);
  dialog.setViewMode(QFileDialog::Detail);
  dialog.setAcceptMode(QFileDialog::AcceptSave);

  if (dialog.exec()){
    dir=dialog.directory().path(); //ricordo l'ultima directory di salvataggio

    path=dialog.selectedFiles().first();
    QString filtro=dialog.selectedFilter();
    
    QString ext(".png");
    for (int i=0;i<exts.size();i++){
      if (filtro.endsWith( "(*."+exts.at(i)+")" ) ){
	ext=exts.at(i);
	break;
      }
    }
    ext="."+ext;
    if ( !path.endsWith(ext) )
      path+=ext;
    if (!image->saveImage(path))
      QMessageBox::critical(this,tr("Error"),tr("Can't save the image,\ncontrol write permission"));
  }
}

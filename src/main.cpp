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


#include <QApplication>
#include "finestra.h"


int main(int argc, char *argv[]){
  QApplication app(argc, argv);

  QString lang(QLocale::system().name());
 

  QTextStream out(stdout);
  out<<"Searching localization "<<lang<<"\n";


  QTranslator translator;
  QString mylocale("frattali_"+lang);
  if ( translator.load(mylocale, QCoreApplication::applicationDirPath()+QDir::separator()+"lang") ){
    app.installTranslator(&translator);
    out<<" installed frattali_"<<lang<<"\n";
  }

  QTranslator translator2;
  if(translator2.load("qt_"+lang, QLibraryInfo::location(QLibraryInfo::TranslationsPath))){
    app.installTranslator(&translator2);
    out<<" installed qt_"<<lang<<"\n";
  }

  finestra *mw=new finestra;
  mw->show();
  return app.exec();
}

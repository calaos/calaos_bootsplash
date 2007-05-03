//#########################################################################//
//                                                                         //
//      File     : MainWindow.h                                            //
//      Authors  : HECKY Raoul                                             //
//                                                                         //
//  Licence :                                                              //
//    This program is free software; you can redistribute it and/or modify //
//    it under the terms of the GNU General Public License as published by //
//    the Free Software Foundation; either version 2 of the License, or    //
//    (at your option) any later version.                                  //
//                                                                         //
//    This program is distributed in the hope that it will be useful,      //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of       //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        //
//    GNU General Public License for more details.                         //
//                                                                         //
//    You should have received a copy of the GNU General Public License    //
//    along with this program; if not, write to the Free Software          //
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.            //
//#########################################################################//
//-----------------------------------------------------------------------------
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//-----------------------------------------------------------------------------
#include <Evas.h>
#include <Emotion.h>
#include <Ecore.h>
#include <Ecore_X.h>
#include <Ecore_Evas.h>
#include <Edje.h>
#include <string>
#include <iostream>
#include <vector>
#include <EdjeObject.h>
#include <callbacks.h>
#include "../config.h"
//-----------------------------------------------------------------------------
// Default window size
#define WIDTH   1024
#define HEIGHT  768
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
typedef enum { FRAMEBUFFER, OPENGL, SOFTWARE } Engine;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class MainWindow
{
        private:
                string theme; //Edje theme filename
                Engine engine;
                bool fullscreen;

                //EFL
                Ecore_Evas *ee;
                Ecore_X_Window ewin;
                Evas *evas;
                EdjeObject *edje_back; //The edje background object
                EdjeObject *edje_control; //The edje menu object

        public:
                MainWindow(Engine &_engine, string &_theme);
                ~MainWindow();

                void set_fullscreen(bool en);
                void Resize(int w, int h);
                void SignalControl(void *data, Evas_Object *obj, const char *sig, const char *src);

                Ecore_Evas *get_ecore_evas() { return ee; }
};
//-----------------------------------------------------------------------------
#endif

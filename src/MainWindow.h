/******************************************************************************
**  Copyright (c) 2007-2008, Calaos. All Rights Reserved.
**
**  This file is part of Calaos Home.
**
**  Calaos Home is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 3 of the License, or
**  (at your option) any later version.
**
**  Calaos Home is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Foobar; if not, write to the Free Software
**  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
******************************************************************************/
//-----------------------------------------------------------------------------
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//-----------------------------------------------------------------------------
#include <Evas.h>
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

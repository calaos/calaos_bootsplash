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
#ifndef CEDJEOBJ_H
#define CEDJEOBJ_H
//-----------------------------------------------------------------------------
#include <Evas.h>
#include <Edje.h>
#include <string>
#include <iostream>
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class EdjeObject
{
        protected:
                string theme; //Edje theme filename

                //EFL
                Evas *evas;
                Evas_Object *edje; //The evas object

        public:
                EdjeObject(string &_theme, Evas *_evas);
                ~EdjeObject();

                //load the edje file
                bool LoadEdje(string collection);

                virtual void Show() { evas_object_show(edje); }
                virtual void Hide() { evas_object_hide(edje); }
                void Move(int x, int y) { evas_object_move(edje, x, y); }
                void Resize(int w, int h) { evas_object_resize(edje, w, h); }

                void set_layer(int i) { evas_object_layer_set(edje, i); }
                int get_layer() { evas_object_layer_get(edje); }

                Evas_Object *get_edje() { return edje; }

                void set_theme(string &_theme) { theme = _theme; }

                void signal_emit(string emission, string source)
                        { edje_object_signal_emit(edje, emission.c_str(), source.c_str()); }
};
//-----------------------------------------------------------------------------
#endif

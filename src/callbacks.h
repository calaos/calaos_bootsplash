//#########################################################################//
//                                                                         //
//      File     : callbacks.h                                             //
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
#ifndef CALLBACKS_H
#define CALLBACKS_H
//-----------------------------------------------------------------------------
#include <Evas.h>
#include <Emotion.h>
#include <Ecore.h>
#include <Ecore_X.h>
#include <Ecore_Evas.h>
#include <Edje.h>
#include <string>
#include <iostream>
//-----------------------------------------------------------------------------
int app_signal_exit(void *data, int type, void *event);
void resize_cb(Ecore_Evas *ee);
void signalcontrol_cb(void *data, Evas_Object *o, const char *sig, const char *src);
//-----------------------------------------------------------------------------
#endif

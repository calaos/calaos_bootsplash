//#########################################################################//
//                                                                         //
//      File     : EApplication.h                                          //
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
#ifndef EApplication_H
#define EApplication_H
//-----------------------------------------------------------------------------
#include <Evas.h>
#include <Ecore.h>
#include <Ecore_X.h>
#include <Ecore_Evas.h>
#include <Edje.h>
#include <string>
#include <iostream>
#include <MainWindow.h>
#include <Utils.h>
#include "../config.h"
//-----------------------------------------------------------------------------
#define DEFAULT_THEME           "default.edj"
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
class EApplication
{
        private:
                //the window
                MainWindow *window;
                EApplication(int argc, char **argv);

        public:
                static EApplication &Instance(int argc = -1, char **argv = NULL)
                {
                        static EApplication app(argc, argv);

                        return app;
                }

                void Run();
                void Stop();

                MainWindow *get_window() { return window; }
};
//-----------------------------------------------------------------------------
#endif

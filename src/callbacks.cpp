//#########################################################################//
//                                                                         //
//      File     : callbacks.cpp                                           //
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
#include <callbacks.h>
#include <EApplication.h>
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int app_signal_exit(void *data, int type, void *event)
{
        cout << "Exit called, shutting down..." << endl;
        EApplication::Instance().Stop();
        return 1;
}
//-----------------------------------------------------------------------------
void resize_cb(Ecore_Evas *ee)
{
        Evas_Coord w, h;
        evas_output_viewport_get(ecore_evas_get(ee), NULL, NULL, &w, &h);
        EApplication::Instance().get_window()->Resize(w, h);
}
//-----------------------------------------------------------------------------
void signalcontrol_cb(void *data, Evas_Object *o, const char *sig, const char *src)
{
        EApplication::Instance().get_window()->SignalControl(data, o, sig, src);
}
//-----------------------------------------------------------------------------

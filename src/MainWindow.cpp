//#########################################################################//
//                                                                         //
//      File     : MainWindow.cpp                                          //
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
//-----------------------------------------------------------------------------
#include <MainWindow.h>
//-----------------------------------------------------------------------------
MainWindow::MainWindow(Engine &_engine, string &_theme) : engine(_engine), theme(_theme), fullscreen(false)
{
        //Init EFL
        ecore_init();
        ecore_evas_init();
        edje_init();

        ecore_event_handler_add(ECORE_EVENT_SIGNAL_EXIT, app_signal_exit, NULL);

        //create the ecore_evas object with the specified engine.
        switch (engine)
        {
                case OPENGL: ee = ecore_evas_gl_x11_new(NULL, 0, 0, 0, WIDTH, HEIGHT); break;
                case FRAMEBUFFER: ee = ecore_evas_fb_new(NULL, 0, WIDTH, HEIGHT); break;
                default:
                case SOFTWARE: ee = ecore_evas_software_x11_new(NULL, 0, 0, 0, WIDTH, HEIGHT);
        }
        //Fall back to SOFTWARE X11 if any other fail.
        if (!ee)
        {
                if (engine == OPENGL) cout << "Failed to create the X11 opengl engine..." << endl;
                if (engine == FRAMEBUFFER) cout << "Failed to create the framebuffer engine..." << endl;
                cout << "Falling back to software X11." << endl;
                ee = ecore_evas_software_x11_new(NULL, 0, 0, 0, WIDTH, HEIGHT);
                engine = SOFTWARE;

                if (!ee)
                {
                        cout << "FATAL ERROR: Can't create a rendering engine !" << endl << "Exiting..." << endl;
                        exit(1);
                }
        }

        ecore_evas_title_set(ee, "Calaos::Bootsplash");
        ecore_evas_shaped_set(ee, 0);
        ecore_evas_move_resize(ee, 0, 0, WIDTH, HEIGHT);
        ecore_evas_show(ee);

        //get the evas object
        evas = ecore_evas_get(ee);
        edje_frametime_set(1.0 / 30.0);

        //create the background
        edje_back = new EdjeObject(theme, evas);
        if (!edje_back->LoadEdje("background"))
        {
                cout << "FATAL ERROR: Can't load the default theme!" << endl << "Exiting..." << endl;
                exit(1);
        }

        Resize(WIDTH, HEIGHT);
        edje_back->Show();

        ecore_evas_callback_resize_set(ee, resize_cb);
}
//-----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
        delete edje_back;
        delete edje_control;

        edje_shutdown();
        ecore_evas_shutdown();
        ecore_shutdown();
}
//-----------------------------------------------------------------------------
void MainWindow::set_fullscreen(bool en)
{
        ecore_evas_borderless_set(ee, en);
        ecore_evas_fullscreen_set(ee, en);
}
//-----------------------------------------------------------------------------
void MainWindow::Resize(int w, int h)
{
        edje_back->Move(0, 0);
        edje_back->Resize(w, h);
}
//-----------------------------------------------------------------------------
void MainWindow::SignalControl(void *data, Evas_Object *obj, const char *sig, const char *src)
{

}
//-----------------------------------------------------------------------------

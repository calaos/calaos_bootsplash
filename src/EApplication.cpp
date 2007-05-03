//#########################################################################//
//                                                                         //
//      File     : EApplication.cpp                                        //
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
#include <EApplication.h>
//-----------------------------------------------------------------------------
EApplication::EApplication(int argc, char **argv)
{
        string theme = PACKAGE_DATA_DIR"/"DEFAULT_THEME;
        Engine eng = SOFTWARE;

        for (int i = 1; i < argc; i++)
        {
                if (!strcmp(argv[i], "-gl"))
                        eng = OPENGL;
                else if (!strcmp(argv[i], "-fb"))
                        eng = FRAMEBUFFER;
                else if (!strcmp(argv[i], "-t"))
                {
                        if (i + 1 < argc)
                                theme = argv[i + 1];
                }
                else if ( (!strcmp(argv[i], "-h")) || (!strcmp(argv[i], "--help")) )
                {
                        cout << "ePresent v" << VERSION << endl;
                        cout << "Usage:\n\t" << argv[0] << " [options]" << endl;
                        cout << endl << "\tOptions:\n\t";
                        cout << "-gl\tUse opengl engine. (if available)\n\t";
                        cout << "-fb\tUse framebuffer engine. (if available)\n\t";
                        cout << "-t <file>\tSpecify the main theme.\n\t";
                        cout << "-h, --help\tDisplay this help.\n\n";

                        exit(1);
                }
        }

        window = new MainWindow(eng, theme);
}
//-----------------------------------------------------------------------------
void EApplication::Run()
{
        ecore_main_loop_begin();
}
//-----------------------------------------------------------------------------
void EApplication::Stop()
{
        ecore_main_loop_quit();
}
//-----------------------------------------------------------------------------

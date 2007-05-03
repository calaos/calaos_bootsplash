//#########################################################################//
//                                                                         //
//      File     : EdjeObject.cpp                                          //
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
#include <EdjeObject.h>
//-----------------------------------------------------------------------------
EdjeObject::EdjeObject(string &_theme, Evas *_evas): theme(_theme), evas(_evas)
{
        //create the edje object
        edje = edje_object_add(evas);
}
//-----------------------------------------------------------------------------
EdjeObject::~EdjeObject()
{
        evas_object_del(edje);
}
//-----------------------------------------------------------------------------
bool EdjeObject::LoadEdje(string collection)
{
        if (edje_object_file_set(edje, theme.c_str(), collection.c_str()) == 0)
        {
                cerr << "Error loading edje(\"" << theme << "\", \"" << collection << "\")!" << endl;
                int err = edje_object_load_error_get(edje);
                cerr << "Error #" << err << endl;
                switch (err)
                {
                  case EDJE_LOAD_ERROR_NONE: cerr << "No Error..." << endl; break;
                  default:
                  case EDJE_LOAD_ERROR_GENERIC: cerr << "Unknown Error !" << endl; break;
                  case EDJE_LOAD_ERROR_DOES_NOT_EXIST: cerr << "File doesn't exist !" << endl; break;
                  case EDJE_LOAD_ERROR_PERMISSION_DENIED: cerr << "Permission denied !" << endl; break;
                  case EDJE_LOAD_ERROR_RESOURCE_ALLOCATION_FAILED: cerr << "Allocation failed !" << endl; break;
                  case EDJE_LOAD_ERROR_CORRUPT_FILE: cerr << "File is corrupted !" << endl; break;
                  case EDJE_LOAD_ERROR_UNKNOWN_FORMAT: cerr << "Unknown file format !" << endl; break;
                  case EDJE_LOAD_ERROR_INCOMPATIBLE_FILE: cerr << "Incompatible file !" << endl; break;
                }

                return false;
        }

        return true;
}
//-----------------------------------------------------------------------------

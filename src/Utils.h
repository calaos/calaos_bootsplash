//#########################################################################//
//                                                                         //
//      File     : Utils.h                                                 //
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
#ifndef CUTILS_H
#define CUTILS_H
//-----------------------------------------------------------------------------
#include <string>
#include <sstream>
#include <iostream>
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
namespace Utils
{
static string itostr(int i);
static string url_encode(string str);
static string url_decode(string str);
static int htoi(char *s);
static string time2string(long s);
//-----------------------------------------------------------------------------
template<typename T>
bool is_of_type(const std::string &str)
{
        std::istringstream iss(str);
        T tmp;
        return (iss >> tmp) && (iss.eof());
}
template<typename T>
bool from_string(const std::string &str, T &dest)
{
        std::istringstream iss(str);
        return iss >> dest != 0;
}
template<typename T>
std::string to_string( const T & Value )
{
        std::ostringstream oss;
        oss << Value;
        return oss.str();
}
//-----------------------------------------------------------------------------
};
//-----------------------------------------------------------------------------
#endif

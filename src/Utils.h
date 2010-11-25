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
#ifndef CUTILS_H
#define CUTILS_H
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <string.h>
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

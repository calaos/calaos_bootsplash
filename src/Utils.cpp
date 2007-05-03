//#########################################################################//
//                                                                         //
//      File     : Utils.cpp                                               //
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
#include <Utils.h>
//-----------------------------------------------------------------------------
string Utils::itostr(int i)
{
        std::ostringstream output;
        output << i;
        return output.str();
}
//-----------------------------------------------------------------------------
string Utils::url_encode(string str)
{
        string ret = "";
        char tmp[10];

        for (int i = 0;i < str.length();i++)
        {
                if (str[i] >= 'a' && str[i] <= 'z' ||
                    str[i] >= 'A' && str[i] <= 'Z' ||
                    str[i] >= '0' && str[i] <= '9' ||
                    str[i] == '_')
                    ret += str[i];
                else
                {
                        memset(tmp, '\0', 5);
                        sprintf(tmp, "%%%02X", (unsigned char)str[i]);
                        ret += tmp;
                }
        }

        return ret;
}
//-----------------------------------------------------------------------------
string Utils::url_decode(string str)
{
        string ret = "";

        for (int i = 0;i < str.length();i++)
        {
                if (str[i] == '%' && isxdigit((int)str[i + 1]) && isxdigit((int)str[i + 2]))
                {
                        ret += (char) htoi((char *)str.c_str() + i + 1);
                        i += 2;
                }
                else
                        ret += str[i];
        }

        return ret;
}
//-----------------------------------------------------------------------------
int Utils::htoi(char *s)
{
        int value;
        int c;

        c = ((unsigned char *)s)[0];
        if (isupper(c))
                c = tolower(c);
        value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

        c = ((unsigned char *)s)[1];
        if (isupper(c))
                c = tolower(c);
        value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

        return (value);
}
//-----------------------------------------------------------------------------
string Utils::time2string(long s)
{
        double sec = s;
        int hours = (int)(sec / 3600.0);
        sec -= hours * 3600;
        int min = (int)(sec / 60.0);
        sec -= min * 60;

        stringstream str;

        if (hours == 1)
                str << hours << " " << "heure" << " ";
        if (hours > 1)
                str << hours << " " << "heures" << " ";
        if (min == 1)
                str << min << " " << "minute" << " ";
        if (min > 1)
                str << min << " " << "minutes" << " ";
        if (sec == 1)
                str << sec << " " << "seconde";
        if (sec > 1)
                str << sec << " " << "secondes";

        return str.str();
}
//-----------------------------------------------------------------------------

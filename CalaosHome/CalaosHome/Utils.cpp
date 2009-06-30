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
//-----------------------------------------------------------------------------
#include "Utils.h"
//-----------------------------------------------------------------------------
using namespace Utils;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
string Utils::itostr(int i)
{
        std::ostringstream output;
        //         output.imbue(std::locale(""));
        output << i;
        return output.str();
}
//-----------------------------------------------------------------------------
string Utils::url_encode(string str)
{
        string ret = "";
        char tmp[10];

        for (uint i = 0;i < str.length();i++)
        {
                if ((str[i] >= 'a' && str[i] <= 'z') ||
                                (str[i] >= 'A' && str[i] <= 'Z') ||
                                (str[i] >= '0' && str[i] <= '9') ||
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

        for (uint i = 0;i < str.length();i++)
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
std::string Utils::url_decode2(std::string str)
{
        return url_decode(url_decode(str));
}
//-----------------------------------------------------------------------------
std::string Utils::Base64_decode(std::string &str)
{
        std::string ret;
        ret = base64_decode(str.c_str());

        return ret;
}
//---------------------------------------------------------------------------
void *Utils::Base64_decode_data(std::string &str)
{
        char *ret = base64_decode(str.c_str());

        return ret;
}
//---------------------------------------------------------------------------
std::string Utils::Base64_encode(std::string &str)
{
        std::string ret;
        ret = base64_encode(str.c_str(), str.length());

        return ret;
}
//---------------------------------------------------------------------------
std::string Utils::Base64_encode(void *data, int size)
{
        std::string ret;
        ret = base64_encode((char *)data, size);

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
string Utils::time2string_digit(long s)
{
        double sec = s;
        int hours = (int)(sec / 3600.0);
        sec -= hours * 3600;
        int min = (int)(sec / 60.0);
        sec -= min * 60;

        stringstream str;

        if (hours > 0)
                str << setw(2) << setfill('0') << hours << ":";

        str << setw(2) << setfill('0') << min << ":";
        str << setw(2) << setfill('0') << sec;

        return str.str();
}
//-----------------------------------------------------------------------------
void Utils::split(const string &str, vector<string> &tokens, const string &delimiters, int max /* = 0 */)
{
        // Skip delimiters at beginning.
        string::size_type lastPos = str.find_first_not_of(delimiters, 0);
        // Find first "non-delimiter".
        string::size_type pos     = str.find_first_of(delimiters, lastPos);

        int counter = 0;

        while (string::npos != pos || string::npos != lastPos)
        {
                if (counter + 1 >= max && max > 0)
                {
                        tokens.push_back(str.substr(lastPos, string::npos));
                        break;
                }

                // Found a token, add it to the vector.
                tokens.push_back(str.substr(lastPos, pos - lastPos));
                // Skip delimiters.  Note the "not_of"
                lastPos = str.find_first_not_of(delimiters, pos);
                // Find next "non-delimiter"
                pos = str.find_first_of(delimiters, lastPos);

                counter++;
        }

        while (tokens.size() < (uint)max) tokens.push_back("");
}
//-----------------------------------------------------------------------------
void Utils::remove_tag(string &html, const string begin_tag, const string end_tag)
{
        string::size_type start_pos = html.find(begin_tag, 0);

        while (start_pos != string::npos)
        {
                string::size_type end_pos = html.find(end_tag, start_pos);
                if (end_pos == string::npos)
                {
                        break;
                }
                else
                {
                        end_pos += end_tag.length();
                        html.erase(start_pos, end_pos - start_pos);
                        start_pos = html.find(begin_tag, 0);
                }
        }
}
//-----------------------------------------------------------------------------
void Utils::replace_str(string &source, const string searchstr, const string replacestr)
{
        string::size_type pos = 0;
        while((pos = source.find(searchstr, pos)) != string::npos)
        {
                source.erase(pos, searchstr.length());
                source.insert(pos, replacestr);
                pos += replacestr.length();
        }
}
//-----------------------------------------------------------------------------
void Utils::trim_right(std::string &source, const std::string &t)
{
        source.erase(source.find_last_not_of(t) + 1);
}
//-----------------------------------------------------------------------------
void Utils::trim_left(std::string &source, const std::string &t)
{
        source.erase(0, source.find_first_not_of(t));
}
//-----------------------------------------------------------------------------
static Params ParamsConfig;
std::string Utils::get_config_option(std::string key)
{
		return ParamsConfig[key];
}
//-----------------------------------------------------------------------------
void Utils::set_config_option(std::string key, std::string value)
{
		ParamsConfig.Add(key, value);
}
//-----------------------------------------------------------------------------

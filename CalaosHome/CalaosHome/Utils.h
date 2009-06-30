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
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "Params.h"
#include "base64.h"
#import "ObjCWrapper.h"

//-----------------------------------------------------------------------------
using namespace std;

//-----------------------------------------------------------------------------
// Some common defines
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#define PI 3.14159265358979323846
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifndef WAGO_LISTEN_PORT
#define WAGO_LISTEN_PORT        4646
#endif
#ifndef TCP_LISTEN_PORT
#define TCP_LISTEN_PORT         4456
#endif
#ifndef HTTP_PORT
#define HTTP_PORT               4460
#endif

//Sleep time for SocketManager thread
#ifndef THREAD_SLEEP
#define THREAD_SLEEP            60 / 2
#endif

#ifndef BCAST_UDP_PORT
#define BCAST_UDP_PORT          4545
#endif

#define NETWORK_TIMEOUT         30000

#define SPECIAL_ROOM_TYPE "Internal"
#define SPECIAL_ROOM_SIMPLESCENARIO "SimpleScenario"

#define SPECIAL_RULES_SIMPLESCENARIO "SimpleScenarioRules"

typedef unsigned int uint;

//Curl callback
//int CURL_write_callback(void *buffer, size_t size, size_t nmemb, void *stream);
//-----------------------------------------------------------------------------
//class EdjeObject;
//-----------------------------------------------------------------------------
namespace Utils
{
        string itostr(int i);
        string url_encode(string str);
        string url_decode(string str);
        std::string url_decode2(std::string str); //decode 2 times
        int htoi(char *s);
        string time2string(long s);
        string time2string_digit(long s);

        /* usefull string utilities */
        void split(const string &str, vector<string> &tokens, const string &delimiters = " ", int max = 0);
        void remove_tag(string &source, const string begin_tag, const string end_tag);
        void replace_str(string &source, const string searchstr, const string replacestr);
        void trim_right(std::string &source, const std::string &t);
        void trim_left(std::string &source, const std::string &t);

        //!decode a BASE64 string
        std::string Base64_decode(std::string &str);
        void *Base64_decode_data(std::string &str);
        //!encode a BASE64 string
        std::string Base64_encode(std::string &str);
        std::string Base64_encode(void *data, int size);
	
		std::string get_config_option(std::string key);
	    void set_config_option(std::string key, std::string value);	

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
        //Some usefull fonctors
        struct UrlDecode
        {
                template <class T> void operator ()(T &str) const
                {
                        str = Utils::url_decode2(str);
                }
        };
        struct Delete
        {
                template <class T> void operator ()(T *&p) const
                {
                        delete p;
                        p = NULL;
                }
        };
        class to_lower
        {
                public:
                        char operator() (char c) const
                        {
                                return tolower(c);
                        }
        };
        class to_upper
        {
                public:
                        char operator() (char c) const
                        {
                                return toupper(c);
                        }
        };

        class DeletorBase
        {
                public:
                virtual void operator() (void *b) const
                {
                        b = NULL;
                        cerr << "DeletorBase() called, this is an error. It should never happen"
                             << ", because it means the application leaks memory!" << endl;
                }
        };

        //Fonctor to delete a void * with a specified type
        template<typename T>
        class DeletorT: public DeletorBase
        {
                public:
                virtual void operator() (void *b) const
                {
                        T base = reinterpret_cast<T>(b);
                        if (base) delete base;
                }
        };
        //-----------------------------------------------------------------------------
        //-----------------------------------------------------------------------------
        //Used by the CURL callback
        typedef struct file_curl
        {
                char *fname;
                FILE *fp;
        } File_CURL;
        //-----------------------------------------------------------------------------
        class line_exception : public std::exception
        {
                private:
                        std::string msg;

                public:
                        line_exception( const char * Msg, int Line )
                        {
                                std::ostringstream oss;
                                oss << "Error line " << Line << " : " << Msg;
                                msg = oss.str();
                        }

                        virtual ~line_exception() throw()
                        { }

                        virtual const char * what() const throw()
                        {
                                return msg.c_str();
                        }
        };


        //-----------------------------------------------------------------------------
        typedef enum { TBOOL, TINT, TSTRING } DATA_TYPE;
        typedef enum { PLAY, PAUSE, STOP, ERROR, SONG_CHANGE } PSTATUS;
        typedef enum { UNKNOWN, SLIMSERVER, IRTRANS, CALAOS } SOCKET_TYPE;
        //-----------------------------------------------------------------------------
        enum { VUP, VDOWN, VSTOP, VNONE };
        //-----------------------------------------------------------------------------
        typedef unsigned short UWord;
        //-----------------------------------------------------------------------------
};
//-----------------------------------------------------------------------------
#endif

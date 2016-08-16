#ifndef SANTIAGO_AUTHENTICATION_SERVERDATA_H
#define SANTIAGO_AUTHENTICATION_SERVERDATA_H

#include "RequestHandlerBase.h"

namespace Santiago{ namespace Authentication
{
    struct ServerData
    {
        std::map<std::string,CookieData>           _cookieCookieDataMap;
        std::map<std::string,UserIdData>           _userIdUserIdDataMap;
        std::map<RequestId,RequestHandlerBasePtr>  _activeRequestHandlersList;
    };
}}

#endif

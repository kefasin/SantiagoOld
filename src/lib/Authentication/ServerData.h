#ifndef SANTIAGO_AUTHENTICATION_SERVERDATA_H
#define SANTIAGO_AUTHENTICATION_SERVERDATA_H

#include "RequestHandlerBase.h"

namespace Santiago{ namespace Authentication
{
    struct ResourceData
    {
        std::vector<std::pair<unsigned,std::string> >  _connectionIdCookiePairList;
    };

    struct ServerData
    {
        std::map<std::string,CookieData>                 _cookieCookieDataMap;
        std::map<std::string,UserIdData>                 _userIdUserIdDataMap;
        std::map<RequestId,RequestHandlerBasePtr>        _activeRequestHandlersList;
        std::map<std::string,ResourceData>               _resourceIdResourceDataMap;
        std::map<std::string,std::vector<std::string> >  _cookieIdResourceIdMap;
    };
}}

#endif

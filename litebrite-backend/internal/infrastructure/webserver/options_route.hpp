//
// Created by broosegoose on 7/30/23.
//

#ifndef INFRASTRUCTURE_WEBSERVER_OPTIONS_ROUTE_HPP
#define INFRASTRUCTURE_WEBSERVER_OPTIONS_ROUTE_HPP

#include <crow.h>

inline crow::response OptionsResponse(const crow::request &req) {
    return crow::response(crow::status::OK);
}

#define CROW_OPTIONS_ROUTE(app, url) \
    CROW_ROUTE(app, url).methods("OPTIONS"_method)(OptionsResponse), \
    CROW_ROUTE(app, url)

#define CROW_BP_OPTIONS_ROUTE(bp, url) \
    CROW_BP_ROUTE(bp, url).methods("OPTIONS"_method)(OptionsResponse), \
    CROW_BP_ROUTE(bp, url)


#endif //INFRASTRUCTURE_WEBSERVER_OPTIONS_ROUTE_HPP
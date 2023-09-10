//
// Created by broosegoose on 9/9/23.
//

#ifndef INFRASTRUCTURE_WEBSERVER_AUTHORIZE_MIDDLEWARE_HPP
#define INFRASTRUCTURE_WEBSERVER_AUTHORIZE_MIDDLEWARE_HPP

#include "./authorize_middleware.hpp"

#include "domain/user.hpp"
#include "domain/site.hpp"

#include "infrastructure/db/db.hpp"


namespace crow
{
    struct AuthorizeSiteHandler
    {
        struct context
        {
            nlohmann::json params;
            domain::SitePtr site = nullptr;
        };

        template <typename AllContext>
        void before_handle(crow::request& req, crow::response& res, context& ctx, AllContext& all_ctx)
        {
            if (req.method == "OPTIONS"_method || req.url.substr(0, _site_prefix.size()) != _site_prefix) {
                // No authZ needed on preflight
                return;
            }
            if (!middlewareIsConfigured(res)) {
                return;
            }
            if (!parseParamsFetchSite(req, res, ctx)) {
                return;
            }
            AuthenticateHandler::context &auth_ctx = all_ctx.template get<AuthenticateHandler>();
            auto &user = auth_ctx.user;
            if (user->is_admin) {
                // admin always gets through
                return;
            } else if (user->site_id != ctx.site->id) {
                // non admin calling route on site they aren't allocated
                res.code = crow::status::UNAUTHORIZED;
                res.body = "crow::AuthorizeHandler unauthorized access";
                res.end();
                return;
            }
        }

        void after_handle(crow::request& /*req*/, crow::response& /*res*/, context& /*ctx*/)
        {}

        AuthorizeSiteHandler& SetDb(infrastructure::DbPtr db) {
            _db = std::move(db);
            return *this;
        }

    private:

        bool middlewareIsConfigured(crow::response& res) {
            if (_db == nullptr) {
                res.code = crow::status::INTERNAL_SERVER_ERROR;
                res.body = "crow::AuthenticateMiddleware is not properly configured";
                res.end();
                return false;
            }
            return true;
        }

        [[nodiscard]] bool parseParamsFetchSite(crow::request &req, crow::response &res, context& ctx) {
            try {
                ctx.params = nlohmann::json::parse(req.body);
                const auto wants_site_id = ctx.params.value("site_id", -1);
                if (wants_site_id == -1) {
                    res.code = crow::status::BAD_REQUEST;
                    res.body = "crow::AuthorizeHandler invalid body payload";
                    res.end();
                    return false;
                }
                auto site = _db->GetSite(wants_site_id);
                if (site == nullptr) {
                    res.code = crow::status::UNAUTHORIZED;
                    res.body = "crow::AuthorizeHandler invalid site";
                    res.end();
                    return false;
                }
                ctx.site = std::move(site);
                return true;
            } catch (nlohmann::json::exception& e) {
                std::cerr << e.what() << std::endl;
                res.code = crow::status::BAD_REQUEST;
                res.body = "crow::AuthorizeHandler invalid body payload";
            } catch(std::runtime_error &e) {
                std::cerr << e.what() << std::endl;
                res.code = crow::status::INTERNAL_SERVER_ERROR;
                res.body = "crow::AuthorizeHandler server failed to parse token";
            } catch (...) {
                res.code = crow::status::INTERNAL_SERVER_ERROR;
                res.body = "crow::AuthorizeHandler server super failed to parse token";
            }
            res.end();
            return false;
        }

        const std::string _site_prefix = "/site/";

        infrastructure::DbPtr _db = nullptr;
    };

    struct AuthorizeAdminHandler
    {
        struct context
        {
            nlohmann::json params;
        };

        template <typename AllContext>
        void before_handle(crow::request& req, crow::response& res, context& ctx, AllContext& all_ctx)
        {
            if (req.method == "OPTIONS"_method || req.url.substr(0, _admin_prefix.size()) != _admin_prefix) {
                // No authZ needed on preflight
                return;
            }
            if (!parseParams(req, res, ctx)) {
                return;
            }
            AuthenticateHandler::context &auth_ctx = all_ctx.template get<AuthenticateHandler>();
            auto &user = auth_ctx.user;
            if (!user->is_admin) {
                res.code = crow::status::UNAUTHORIZED;
                res.body = "crow::AuthorizeAdminHandler unauthorized access";
                res.end();
                return;
            }
        }

        void after_handle(crow::request& /*req*/, crow::response& /*res*/, context& /*ctx*/)
        {}

    private:

        [[nodiscard]] bool parseParams(crow::request &req, crow::response &res, context& ctx) {
            try {
                ctx.params = nlohmann::json::parse(req.body);
                return true;
            } catch (nlohmann::json::exception& e) {
                std::cerr << e.what() << std::endl;
                res.code = crow::status::BAD_REQUEST;
                res.body = "crow::AuthorizeAdminHandler invalid body payload";
            } catch(std::runtime_error &e) {
                std::cerr << e.what() << std::endl;
                res.code = crow::status::INTERNAL_SERVER_ERROR;
                res.body = "crow::AuthorizeAdminHandler server failed to parse token";
            } catch (...) {
                res.code = crow::status::INTERNAL_SERVER_ERROR;
                res.body = "crow::AuthorizeAdminHandler server super failed to parse token";
            }
            res.end();
            return false;
        }

        const std::string _admin_prefix = "/admin/";
    };
}

#endif //INFRASTRUCTURE_WEBSERVER_AUTHORIZE_MIDDLEWARE_HPP

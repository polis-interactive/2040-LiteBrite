//
// Created by broosegoose on 7/30/23.
//

#ifndef INFRASTRUCTURE_WEBSERVER_SUBDOMAIN_CORS_HPP
#define INFRASTRUCTURE_WEBSERVER_SUBDOMAIN_CORS_HPP

#include "crow/http_request.h"
#include "crow/http_response.h"
#include "crow/routing.h"

namespace crow
{
    struct SubdomainCORSHandler;

    /// Used for tuning CORS policies
    struct SubdomainCORSRules
    {
        friend struct SubdomainCORSHandler;

        /// Set Access-Control-Allow-Origin. Default is "*"
        SubdomainCORSRules& domainOrigin(const std::string& origin)
        {
            domain_origin_ = origin;
            return *this;
        }

        /// Set Access-Control-Allow-Methods. Default is "*"
        SubdomainCORSRules& methods(crow::HTTPMethod method)
        {
            add_list_item(methods_, crow::method_name(method));
            return *this;
        }

        /// Set Access-Control-Allow-Methods. Default is "*"
        template<typename... Methods>
        SubdomainCORSRules& methods(crow::HTTPMethod method, Methods... method_list)
        {
            add_list_item(methods_, crow::method_name(method));
            methods(method_list...);
            return *this;
        }

        /// Set Access-Control-Allow-Headers. Default is "*"
        SubdomainCORSRules& headers(const std::string& header)
        {
            add_list_item(headers_, header);
            return *this;
        }

        /// Set Access-Control-Allow-Headers. Default is "*"
        template<typename... Headers>
        SubdomainCORSRules& headers(const std::string& header, Headers... header_list)
        {
            add_list_item(headers_, header);
            headers(header_list...);
            return *this;
        }

        /// Set Access-Control-Max-Age. Default is none
        SubdomainCORSRules& max_age(int max_age)
        {
            max_age_ = std::to_string(max_age);
            return *this;
        }

        /// Enable Access-Control-Allow-Credentials
        SubdomainCORSRules& allow_credentials()
        {
            allow_credentials_ = true;
            return *this;
        }

        /// Ignore CORS and don't send any headers
        void ignore()
        {
            ignore_ = true;
        }

        /// Handle CORS on specific prefix path
        SubdomainCORSRules& prefix(const std::string& prefix);


        /// Global CORS policy
        SubdomainCORSRules& global();

    private:
        SubdomainCORSRules() = delete;
        SubdomainCORSRules(SubdomainCORSHandler* handler):
                handler_(handler) {}

        /// build comma separated list
        void add_list_item(std::string& list, const std::string& val)
        {
            if (list == "*") list = "";
            if (list.size() > 0) list += ", ";
            list += val;
        }

        /// Set header `key` to `value` if it is not set
        void set_header_no_override(const std::string& key, const std::string& value, crow::response& res)
        {
            if (value.size() == 0) return;
            if (!get_header_value(res.headers, key).empty()) return;
            res.add_header(key, value);
        }

        bool isSubdomain(const std::string &maybe_sub_domain) {
            auto pos = maybe_sub_domain.rfind(domain_origin_);
            return pos != std::string::npos && pos > 0 && maybe_sub_domain[pos - 1] == '.';
        }

        /// Set response headers
        void apply(const std::string &req_origin, crow::response& res)
        {
            if (ignore_) return;
            if (req_origin.empty() || !isSubdomain(req_origin)) {
                set_header_no_override("Access-Control-Allow-Origin", domain_origin_, res);
            } else {
                set_header_no_override("Access-Control-Allow-Origin", req_origin, res);
            }
            set_header_no_override("Access-Control-Allow-Methods", methods_, res);
            set_header_no_override("Access-Control-Allow-Headers", headers_, res);
            set_header_no_override("Access-Control-Max-Age", max_age_, res);
            if (allow_credentials_) set_header_no_override("Access-Control-Allow-Credentials", "true", res);
        }

        bool ignore_ = false;
        std::string domain_origin_ = "*";
        std::string methods_ = "*";
        std::string headers_ = "*";
        std::string max_age_;
        bool allow_credentials_ = false;

        SubdomainCORSHandler* handler_;
    };

    /// CORSHandler is a global middleware for setting CORS headers.

    ///
    /// By default, it sets Access-Control-Allow-Origin/Methods/Headers to "*".
    /// The default behaviour can be changed with the `global()` cors rule.
    /// Additional rules for prexies can be added with `prefix()`.
    struct SubdomainCORSHandler
    {
        struct context
        {};

        void before_handle(crow::request& /*req*/, crow::response& /*res*/, context& /*ctx*/)
        {}

        void after_handle(crow::request& req, crow::response& res, context& /*ctx*/)
        {
            auto& rule = find_rule(req.url);
            auto origin = req.get_header_value("Origin");
            rule.apply(origin, res);
        }

        /// Handle CORS on a specific prefix path
        SubdomainCORSRules& prefix(const std::string& prefix)
        {
            rules.emplace_back(prefix, SubdomainCORSRules(this));
            return rules.back().second;
        }


        /// Get the global CORS policy
        SubdomainCORSRules& global()
        {
            return default_;
        }

    private:
        SubdomainCORSRules& find_rule(const std::string& path)
        {
            // TODO: use a trie in case of many rules
            for (auto& rule : rules)
            {
                // Check if path starts with a rules prefix
                if (path.rfind(rule.first, 0) == 0)
                {
                    return rule.second;
                }
            }
            return default_;
        }

        std::vector<std::pair<std::string, SubdomainCORSRules>> rules;
        SubdomainCORSRules default_ = SubdomainCORSRules(this);
    };

    inline SubdomainCORSRules& SubdomainCORSRules::prefix(const std::string& prefix)
    {
        return handler_->prefix(prefix);
    }


    inline SubdomainCORSRules& SubdomainCORSRules::global()
    {
        return handler_->global();
    }

} // namespace crow

#endif //INFRASTRUCTURE_WEBSERVER_SUBDOMAIN_CORS_HPP

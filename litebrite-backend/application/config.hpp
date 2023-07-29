//
// Created by broosegoose on 7/23/23.
//

#ifndef APPLICATION_CONFIG_HPP
#define APPLICATION_CONFIG_HPP

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <cpp-dotenv/include/dotenv.h>

namespace application {

    enum class AppType {
        EMBEDDED
    };

    std::pair<dotenv::dotenv&, nlohmann::json> get_config(const AppType &app_type, int argc, char * argv[]) {
        std::string json_name = "";
        std::string env_name = "";
        switch (app_type) {
            case AppType::EMBEDDED:
                json_name = "embedded.";
                break;
        }
        if (argc > 1) {
            json_name += argv[1];
        } else {
            json_name += "default";
        }
        env_name = json_name + ".env";
        json_name += ".json";
        std::filesystem::path root_dir = ROOT_DIR;
        auto conf_dir = root_dir / "config";
        auto env_file_path = conf_dir / env_name;
        auto json_file_path = conf_dir / json_name;
        if (!std::filesystem::exists(env_file_path)) {
            throw std::runtime_error("Couldn't find env file at path: " + env_file_path.string());
        }
        if (!std::filesystem::exists(json_file_path)) {
            throw std::runtime_error("Couldn't find json file at path: " + json_file_path.string());
        }

        /* load env */
        auto& dotenv = dotenv::env.load_dotenv(env_file_path.string(), true, true);

        /* load json */
        std::ifstream config_file(json_file_path);
        nlohmann::json config;
        config_file >> config;

        return { dotenv, std::move(config) };
    }
}

#endif //APPLICATION_CONFIG_HPP

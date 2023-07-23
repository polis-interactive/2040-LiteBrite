//
// Created by broosegoose on 7/23/23.
//

#ifndef APPLICATION_CONFIG_HPP
#define APPLICATION_CONFIG_HPP

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace application {

    enum class AppType {
        EMBEDDED
    };

    nlohmann::json get_json_config(const AppType &app_type, int argc, char * argv[]) {
        std::string config_name = "";
        switch (app_type) {
            case AppType::EMBEDDED:
                config_name = "embedded.";
                break;
        }
        if (argc > 1) {
            config_name += argv[1];
        } else {
            config_name += "default";
        }
        config_name += ".json";
        std::filesystem::path app_dir = APPLICATION_DIR;
        auto normalcy_dir = app_dir.parent_path();
        auto conf_dir = normalcy_dir / "config";
        auto conf_file_path = conf_dir / config_name;
        if (!std::filesystem::exists(conf_file_path)) {
            throw std::runtime_error("Couldn't find config file at path: " + conf_file_path.string());
        }

        std::ifstream config_file(conf_file_path);
        nlohmann::json config;
        config_file >> config;
        return config;
    }
}

#endif //APPLICATION_CONFIG_HPP

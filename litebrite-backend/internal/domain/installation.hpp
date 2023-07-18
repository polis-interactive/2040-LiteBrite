//
// Created by broosegoose on 7/18/23.
//

#ifndef DOMAIN_INSTALLATION_HPP
#define DOMAIN_UNIVSERSE_HPP

#include <vector>
#include <map>
#include <string>

namespace domain {

    struct Universe {
        unsigned int number;
        unsigned int pixel_count;
    };

    struct Installation {
        std::map<unsigned int, unsigned int> universes;
        std::map<std::string, std::vector<unsigned int>> controllers;
    };

}

#endif //DOMAIN_INSTALLATION_HPP

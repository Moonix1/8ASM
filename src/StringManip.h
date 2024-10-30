#pragma once
#include <string>
#include <algorithm>

namespace StringManip {

static std::string Trim(std::string &str) {
	str.erase(str.begin(), std::find_if_not(str.begin(), str.end(), [](unsigned char c) {
        return std::isspace(c);
    }));

    str.erase(std::find_if_not(str.rbegin(), str.rend(), [](unsigned char c) {
        return std::isspace(c);
    }).base(), str.end());

    return str;
}

}
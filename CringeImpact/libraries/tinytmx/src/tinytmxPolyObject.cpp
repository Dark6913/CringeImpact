#pragma warning(disable : 4996)
#include "tinyxml2.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "tinytmxPolyObject.hpp"

namespace tinytmx {

    void PolyObject::Parse(tinyxml2::XMLNode const *polyObjectNode) {
        char *pointsLine = strdup(polyObjectNode->ToElement()->Attribute("points"));

        char *token = std::strtok(pointsLine, " ");
        while (token) {
            tinytmx::Vector2f point;
            std::sscanf(token, "%f,%f", &point.x, &point.y);

            points.push_back(point);

            token = std::strtok(nullptr, " ");
        }

        free(pointsLine);
    }
}

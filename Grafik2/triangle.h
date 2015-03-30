#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/integer.hpp"

#include "edge.h"

class triangle_rasterizer {
public:
    triangle_rasterizer(int x1, int y1, int x2, int y2, int x3, int y3);
    virtual ~triangle_rasterizer();

    std::vector<glm::vec3> all_pixels();

    bool more_fragments() const;
    void next_fragment();

    int x() const;
    int y() const;

private:
    void initialize_triangle(int x1, int y1, int x2, int y2, int x3, int y3);

    int LowerLeft();
    int UpperLeft();

    glm::ivec2 ivertex[3];

    // Indices into the vertex table
    int lower_left;
    int upper_left;
    int the_other;

    edge_rasterizer leftedge;
    edge_rasterizer rightedge;

    // Screen coordinates
    int       x_start;
    int       y_start;

    int       x_stop;
    int       y_stop;

    int       x_current;
    int       y_current;

    bool valid;
};

#endif

#include "triangle.h"

triangle_rasterizer::triangle_rasterizer(int x1, int y1, int x2, int y2, int x3, int y3) : valid(false)
{
    this->initialize_triangle(x1, y1, x2, y2, x3, y3);
}

triangle_rasterizer::~triangle_rasterizer()
{}

std::vector<glm::vec3> triangle_rasterizer::all_pixels()
{
    std::vector<glm::vec3> points;

    while (this->leftedge.more_fragments()) {
	for (int x = this->leftedge.x(); x < this->rightedge.x(); ++x) {
	    points.push_back(glm::vec3(float(x), float(this->leftedge.y()), 0.0f));
	}
	this->leftedge.next_fragment();
	this->rightedge.next_fragment();
    }
    return points;
}

bool triangle_rasterizer::more_fragments() const
{
    return this->valid;
}

void triangle_rasterizer::next_fragment()
{
    if (this->x_current < this->x_stop) {
	this->x_current += 1;
    }
    else {
	// this->x_current >= this->x_stop, so find the next NonEmptyScanline
	// this->valid = this->SearchForNonEmptyScanline();

	this->leftedge.next_fragment();
	this->rightedge.next_fragment();
	while (this->leftedge.more_fragments() && (leftedge.x() >= rightedge.x())) {
	    leftedge.next_fragment();
	    rightedge.next_fragment();
	}
	this->valid = this->leftedge.more_fragments();
	if (this->valid) {
	    this->x_start   = leftedge.x();
	    this->x_current = this->x_start;
	    this->x_stop    = rightedge.x() - 1;
	    this->y_current = leftedge.y();
	}
    }
}

int triangle_rasterizer::x() const
{
    if (!this->valid) {
	throw std::runtime_error("triangle_rasterizer::x(): Invalid State/Not Initialized");
    }
    return this->x_current;
}

int triangle_rasterizer::y() const
{
    if (!this->valid) {
	throw std::runtime_error("triangle_rasterizer::y(): Invalid State/Not Initialized");
    }
    return this->y_current;
}

void triangle_rasterizer::initialize_triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
    this->ivertex[0] = glm::ivec2(x1, y1);
    this->ivertex[1] = glm::ivec2(x2, y2);
    this->ivertex[2] = glm::ivec2(x3, y3);

    this->lower_left = this->LowerLeft();
    this->upper_left = this->UpperLeft();
    this->the_other  = 3 - lower_left - upper_left;

    glm::ivec2 ll = this->ivertex[this->lower_left];
    glm::ivec2 ul = this->ivertex[this->upper_left];
    glm::ivec2 ot = this->ivertex[this->the_other];
            
    // Let u be the vector from 'lower_left' to 'upper_left' vertices.
    glm::ivec2 u(ul - ll);
	    
    // Let v be the vector from 'lower_left' to 'the_other'.
    glm::ivec2 v(ot - ll);

    // If the cross product (u x v) has a positive 
    // z-component then the point 'the_other' is to the left of u, else it is to the
    // right of u.
    int z_component_of_the_cross_product = u.x * v.y- u.y * v.x;
	    
    if (z_component_of_the_cross_product != 0) {
	if (z_component_of_the_cross_product > 0) {
	    // The vertex the_other is to the left of the longest vector u.
	    // Therefore, the leftedge has two edges associated to it
	    // (lower_left -> the_other), and (the_other -> upper_left),
	    // while the right edge has only one (lower_left -> upper_left).
	    this->leftedge.init(ll.x, ll.y, ot.x, ot.y, ul.x, ul.y);
	    this->rightedge.init(ll.x, ll.y, ul.x, ul.y);
	}
	else {
	    // The vertex the_other is to the right of the longest vector u.
	    // Therefore, the leftedge has only one edge assigned to it
	    // (lower_left -> upper_left), while the  rightedge has two edges 
	    // associated to it (lower_left -> the_other), and (the_other -> upper_left).
	    this->leftedge.init(ll.x, ll.y, ul.x, ul.y);
	    this->rightedge.init(ll.x, ll.y, ot.x, ot.y, ul.x, ul.y);
	}
	
	// Now the leftedge and rightedge `edge_rasterizers' are initialized, so they are
	// ready for use.

	this->x_start   = this->leftedge.x();
	this->y_start   = this->leftedge.y();

	this->x_current = this->x_start;
	this->y_current = this->y_start;

	this->x_stop    = this->rightedge.x() - 1;
	this->y_stop    = this->ivertex[this->upper_left].y;

	this->valid = (this->x_current <= this->x_stop);
	if (!(this->valid)) {
	    this->next_fragment();
	}
    }
}

int triangle_rasterizer::LowerLeft()
{
    int ll = 0;
    for (int i = ll + 1; i < 3; ++i) {
	if ((this->ivertex[i].y < this->ivertex[ll].y)
	    ||
	    ((this->ivertex[i].y == this->ivertex[ll].y)
	     &&
	     (this->ivertex[i].x < this->ivertex[ll].x)
		)
	    )
	{ ll = i; }
    }
    return ll;
}

int triangle_rasterizer::UpperLeft()
{
    int ul = 0;
    for (int i = ul + 1; i < 3; ++i) {
	if ((this->ivertex[i].y > this->ivertex[ul].y)
	    ||
	    ((this->ivertex[i].y == this->ivertex[ul].y)
	     &&
	     (this->ivertex[i].x < this->ivertex[ul].x)
		)
	    )
	{ ul = i; }
    }
    return ul;
}

#ifndef __GLM_UTILS_H__
#define __GLM_UTILS_H__

/**
 * \file glmutils.h
 * \brief Utility Functions and Operators
 */

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <string>
#include <cctype>

// Use radians by default
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


/**
 * \namespace glm
 * Make some utility functions and make it look like these functions are part of the glm library
 */
namespace glm {
    /**
     * Creates a 4x4 matrix which performs a translation
     * \param dx - the x-component of the translation vector
     * \param dy - the y-component of the translation vector
     * \param dz - the z-component of the translation vector
     * \return a 4x4 matrix performing a translation
     */
    glm::mat4x4 translate(float dx, float dy, float dz);

    /**
     * Creates a 4x4 matrix which performs a scaling
     * \param s_x - scale factor in the x-direction
     * \param s_y - scale factor in the y-dirextion
     * \param s_z - scale factor in the z-dirextion
     * \return a 4x4 matrix performing a scaling
     */
    glm::mat4x4 scale(float s_x, float s_y, float s_z);

    /**
     * Create a 4x4 matrix which performs a rotation around the x-axis
     * \param angle - the rotation angle
     * \return a 4x4 matrix performing a translation
     */
    glm::mat4x4 rotateX(float angle);

    /**
     * Create a 4x4 matrix which performs a rotation around the y-axis
     * \param angle - the rotation angle
     * \return a 4x4 matrix performing a translation
     */
    glm::mat4x4 rotateY(float angle);

    /**
     * Create a 4x4 matrix which performs a rotation around the z-axis
     * \param angle - the rotation angle
     * \return a 4x4 matrix performing a translation
     */
    glm::mat4x4 rotateZ(float angle);

    /**
     * Creates a 4x4 matrix which performs an XY-shear
     * \param sh_x - the shear factor in the x-direction
     * \param sh_y - the shear factor in the y-direction
     * \return a 4x4 matrix performing an XY shear
     */
    glm::mat4x4 shearXY(float sh_x, float sh_y);

    /**
     * Creates a 4x4 matrix which performs an XZ-shear
     * \param sh_x - the shear factor in the x-direction
     * \param sh_z - the shear factor in the z-direction
     * \return a 4x4 matrix performing an XZ shear
     */
    glm::mat4x4 shearXZ(float sh_x, float sh_z);

    /**
     * Creates a 4x4 matrix which performs an YZ-shear
     * \param sh_y - the shear factor in the y-direction
     * \param sh_z - the shear factor in the z-direction
     * \return a 4x4 matrix performing an YZ shear
     */
    glm::mat4x4 shearYZ(float sh_y, float sh_z);
}


/**
 * \file glmutils.h
 * \brief Utility operators
 */

/**
 * Prints a vec2 to std::cout
 * \param s - a std::ostream& where the output should be written.
 * \param v - a glm::vec2& which should be written.
 * \return the parameter s.
 */
std::ostream& operator<<(std::ostream& s, glm::vec2 const& v);
    

/**
 * Prints a vec3 to std::cout
 * \param s - a std::ostream& where the output should be written.
 * \param v - a glm::vec3& which should be written.
 * \return the parameter s.
 */
std::ostream& operator<<(std::ostream& s, glm::vec3 const& v);


/**
 * Prints a vec4 to std::cout
 * \param s - a std::ostream& where the output should be written.
 * \param v - a glm::vec4& which should be written.
 * \return the parameter s.
 */
std::ostream& operator<<(std::ostream& s, glm::vec4 const& v);


/**
 * Prints a mat4x4t o std::cout
 * \param s - a std::ostream& where the output should be written.
 * \param m - a glm::mat4x4& which should be written.
 * \return the parameter s.
 */
std::ostream& operator<<(std::ostream& s, glm::mat4x4 const& m);

#endif

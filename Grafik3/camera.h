#ifndef __RASTERMAN_CAMERA_H__
#define __RASTERMAN_CAMERA_H__

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <string>
#include <cctype>

#include "glmutils.h"


/**
 * \class Camera
 * A class which implements a virtual camera
 */ 
class Camera {
public:
    /**
     * Default constructor, creates a camera with default values of the defining parameters.
     */
    Camera();

    /** 
     * Parameterized constructor, creates a camere with the parameteres given.
     * \param vrp - a vector which specifies the View Reference Point in world coordinates.
     * \param vpn - a vector which specifies the View Plane Normal in world coordinates.
     * \param vup - a vector which specifies the View Up vector in world coordinates.
     * \param prp - a vector which specifies the Projection Reference Point in eye coordinates.
     * \param lower_left_window - a vector which specifies the lower left corner of the window in eye coordinates.
     * \param upper_right_window - a vector which specifies the upper right corner of the window in eye coordinates.
     * \param front_plane - the z-coordinate of the front clipping plane, relative to vrp, i.e. in eye coordinates.
     * \param back_plane - the z-coordinate of the back clipping plane, relative to vrp, i.e. in eye coordinates.
     * \param window_width - the width of the window on the screen.
     * \param window_height - the height of the window on the screen.
     */
    Camera(glm::vec3 const& vrp, glm::vec3  const& vpn, glm::vec3  const& vup, glm::vec3  const& prp,
	   glm::vec2  const& lower_left_window, glm::vec2  const& upper_right_window,
	   float front_plane, float back_plane,
	   int window_width, int window_height);
 
    /**
     * Copy constructor, creates a copy of its parameter.
     * \param camera - the camera object which shuld be copied.
     */
   Camera(Camera const& camera);

    /**
     * Destructor, cleans up the camere and destroys the object.
     */
    virtual ~Camera();

    /**
     * Assignment operator, assigns its parameter to the current instance of class Camera
     * \param camera - the new value of class Camera.
     */
    Camera const& operator=(Camera const& camera);

    /**
     * \return the current ViewOrientation matrix.
     */
    glm::mat4x4 ViewOrientation();

    /**
     * \return the inverse of the current ViewOrientation matrix.
     */
    glm::mat4x4 InvViewOrientation();

    /**
     * \return the current ViewProjection matrix.
     */
    glm::mat4x4 ViewProjection();

    /**
     * \return the inverse of the current ViewProjection matrix.
     */
    glm::mat4x4 InvViewProjection();

    /**
     * \return the current WindowViewport matrix.
     */
    glm::mat4x4 WindowViewport();

    /**
     * \return the inverse of the current WindowViewport matrix.
     */
    glm::mat4x4 InvWindowViewport();

    /**
     * \return the current transformation matrix = WindowViewport() * ViewProjection() * ViewOrientation().
     */
    glm::mat4x4 CurrentTransformationMatrix();

    /**
     * \return the inverse of the current transformation 
     * matrix = InvViewOrientation() * InvViewProjection() * InvWindowViewport().
     */
    glm::mat4x4 InvCurrentTransformationMatrix();

    /**
     * \return the current value of the View Reference Point.
     */
    glm::vec3 const& VRP() const;

    /**
     * Changes the value of the View Reference Point.
     * \param vrp - the new View Reference Point
     */
    void VRP(glm::vec3 const& vrp);

    /**
     * \return the current value of the View Plane Normal.
     */
    glm::vec3 const& VPN() const;

    /**
     * Changes the value of the View Plane Normal.
     * \param vpn - the new View Plane Normal.
     */
    void VPN(glm::vec3 const& vpn);

    /**
     * \return the current value of the View Up vector.
     */
    glm::vec3 const& VUP() const;

    /**
     * Changes the value of the View Up vector.
     * \param vup - the new View Up vector.
     */
    void VUP(glm::vec3 const& vup);

    /**
     * \return the current value of the Projection Reference Point.
     */
    glm::vec3 const& PRP() const;

    /**
     * Changes the value of the Projection Reference Point.
     * \param prp - the new Projection Reference Point.
     */
    void PRP(glm::vec3 const& prp);

    /**
     * \return the lower left corner of the current window.
     */
    glm::vec2 const& WinLowerLeft() const;

    /**
     * Changes the lower left corner of the window.
     * \param lower_left_window - the new value of the lower left corner of the window.
     */
    void WinLowerLeft(glm::vec2 const& lower_left_window);

    /**
     * \return the upper right corner of the current window.
     */
    glm::vec2 const& WinUpperRight() const;

    /**
     * Changes the upper right corner of the window.
     * \param upper_right_window - the new value of the upper right corner of the window.
     */
    void WinUpperRight(glm::vec2 const& upper_right_window);

    /**
     * \return the value of the z-coordinate of the Back Clipping Plane.
     */
    float FrontClippingPlane() const;

    /**
     * Changes the z-value of the Front Clipping Plane.
     * \param front_plane - the new value of the z-coordinate of the Front Clipping Plane.
     */
    void FrontClippingPlane(float const front_plane);

    /**
     * \return the value of the z-coordinate of the Back Clipping Plane.
     */
    float BackClippingPlane() const;

    /**
     * Changes the z-value of the Back Clipping Plane.
     * \param back_plane - the new value of the z-coordinate of the Back Clipping Plane.
     */
    void BackClippingPlane(float const back_plane);

    /**
     * \return the value of the Window width
     */
    int WindowWidth();

    /**
     * Changes the Window width
     * \param new_window_width - the new value of the Window width
     */
    void WindowWidth(int new_window_width);

    /**
     * \return the value of the Window height
     */
    int WindowHeight();

    /**
     * Changes the Window height
     * \param new_window_height - the new value of the Window height
     */
    void WindowHeight(int new_window_height);

protected:

private:
    // Compute the View Orientation Matrix
    void ComputeViewOrientation(glm::vec3& vrp, glm::vec3& vpn, glm::vec3& vup);

    // Compute the Projection Orientation Matrix
    void ComputeViewProjection(glm::vec3& prp, 
			       glm::vec2& lower_left_window, glm::vec2& upper_right_window,
			       float front_clipping_plane, float back_clipping_plane);

    // Compute the Window Viewport Matrix
    void ComputeWindowViewport(int window_width, int window_height);
 
    // The View Orientation Matrix
    glm::mat4x4 vieworientationmatrix;
    glm::mat4x4 invvieworientationmatrix;
    
    
    // The View Projection Matrix
    glm::mat4x4 viewprojectionmatrix;
    glm::mat4x4 invviewprojectionmatrix;

    // The Window Viewport Matrix
    glm::mat4x4 windowviewportmatrix;
    glm::mat4x4 invwindowviewportmatrix;

    // The Current Transformation Matrix;
    glm::mat4x4 currenttransformationmatrix;
    glm::mat4x4 invcurrenttransformationmatrix;

    // The View Reference Point
    glm::vec3 vrp;

    // The View Plane Normal
    glm::vec3 vpn;

    // The View Up vector
    glm::vec3 vup;

    // The Projection Reference Point
    glm::vec3 prp;

    // The lower left corner of the window
    glm::vec2 lower_left_window;

    // The upper right corner of the window
    glm::vec2 upper_right_window;

    // The front clipping plane
    float front_plane;

    // The back clipping plane
    float back_plane;

    // Window width
    int window_width;

    // Window height
    int window_height;
};

#endif

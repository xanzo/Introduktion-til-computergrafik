#include "camera.h"

std::string horizline(60, '=');

void TestCam1()
{
#if 0
    float window_width  = 500.0f;
    float window_height = 300.0f;
#else
    float window_width  =   2.0f;
    float window_height =   2.0f;
#endif

    glm::vec3 vrp(0.0f, 0.0f, 0.0f);
    glm::vec3 vpn(0.0f, 0.0f, 1.0f);
    glm::vec3 vup(0.0f, 1.0f, 0.0f);
    glm::vec3 prp(0.0f, 0.0f, 5.0f);

    glm::vec2 lower_left(-4.0f, -3.0f);
    glm::vec2 upper_right(4.0f,  3.0f);

    float front_plane =  -5.0f;
    float back_plane  = -15.0f;

    float x_viewport = -1.0f;
    float y_viewport = -1.0f;
    float viewport_width  = window_width;
    float viewport_height = window_height;

    Camera cam(vrp, vpn, vup, prp,
	       lower_left, upper_right, front_plane, back_plane,
	       x_viewport, y_viewport,
	       viewport_width, viewport_height);

    glm::mat4x4 CTM = cam.CurrentTransformationMatrix();
    std::cout << "CurrentTransformationMatrix" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << CTM << std::endl;

    std::cout << "Test of the extreme points" << std::endl;
    std::cout << horizline << std::endl;

    glm::vec4 Pxback(16.0f, 0.0f, -15.0f, 1.0f);
    glm::vec4 TPxback = CTM * Pxback;
    std::cout << "Pxback        = [" << Pxback  << "] --> [" << TPxback << "] --> ["
	      << TPxback / TPxback.w << "]" << std::endl;

    glm::vec4 Pyback(0.0f, 12.0f, -15.0f, 1.0f);
    glm::vec4 TPyback = CTM * Pyback;
    std::cout << "Pyback        = [" << Pyback   << "] --> [" << TPyback << "] --> ["
	      << TPyback / TPyback.w << "]" << std::endl;

    glm::vec4 Pbackcorner(16.0f, 12.0f, -15.0f, 1.0f);
    glm::vec4 TPbackcorner = CTM * Pbackcorner;
    std::cout << "Pbackcorner   = [" << Pbackcorner  << "] --> [" << TPbackcorner << "] --> ["
	      << TPbackcorner / TPbackcorner.w << "]" << std::endl;

    std::cout << std::endl;

    glm::vec4 Pxfront(8.0f, 0.0f, -5.0f, 1.0f);
    glm::vec4 TPxfront = CTM * Pxfront;
    std::cout << "Pxfront       = [" << Pxfront  << "] --> [" << TPxfront << "] --> ["
	      << TPxfront / TPxfront.w << "]" << std::endl;

    glm::vec4 Pyfront(0.0f, 6.0f, -5.0f, 1.0f);
    glm::vec4 TPyfront = CTM * Pyfront;
    std::cout << "Pyfront       = [" << Pyfront  << "] --> [" << TPyfront << "] --> [" 
	      << TPyfront / TPyfront.w << "]" << std::endl;

    glm::vec4 Pfrontcorner(8.0f, 6.0f, -5.0f, 1.0f);
    glm::vec4 TPfrontcorner = CTM * Pfrontcorner;
    std::cout << "Pfrontcorner  = [" << Pfrontcorner  << "] --> [" << TPfrontcorner << "] --> ["
	      << TPfrontcorner / TPfrontcorner.w << "]" << std::endl;
}

void TestCam2()
{
    int window_width  = 500;
    int window_height = 500;

    glm::vec3 vrp(0.0f, 0.0f, 0.0f);
    glm::vec3 vpn(0.0f, 0.0f, 1.0f);
    glm::vec3 vup(0.0f, 1.0f, 0.0f);
    glm::vec3 prp(8.0f, 6.0f, 84.0f);
    
    glm::vec2 lower_left( -50.0f, -50.0f);
    glm::vec2 upper_right( 50.0f,  50.0f);
    
    float front_plane = 60.0f;
    float back_plane  = 25.0f;

    int x_viewport = 0;
    int y_viewport = 0;
    int viewport_width = window_width;
    int viewport_height = window_height;

    Camera cam(vrp, vpn, vup, prp,
	       lower_left, upper_right, front_plane, back_plane,
	       x_viewport, y_viewport,
	       viewport_width, viewport_height);

    glm::mat4x4 CTM = cam.CurrentTransformationMatrix();
    std::cout << "CurrentTransformationMatrix" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << CTM << std::endl;
}

void TestCam3()
{
    int window_width  = 500;
    int window_height = 500;

    glm::vec3 vrp(0.0f, 0.0f, 0.0f);
    glm::vec3 vpn(0.0f, 0.0f, 1.0f);
    glm::vec3 vup(0.0f, 1.0f, 0.0f);
    glm::vec3 prp(8.0f, 6.0f, 84.0f);
    
    glm::vec2 lower_left( -50.0f, -50.0f);
    glm::vec2 upper_right( 50.0f,  50.0f);
    
    float front_plane = 60.0f;
    float back_plane  = 25.0f;

    int x_viewport = 0;
    int y_viewport = 0;
    int viewport_width = window_width;
    int viewport_height = window_height;

    Camera cam(vrp, vpn, vup, prp,
	       lower_left, upper_right, front_plane, back_plane,
	       x_viewport, y_viewport,
	       viewport_width, viewport_height);

    glm::mat4x4 CTM = cam.CurrentTransformationMatrix();
    std::cout << "CurrentTransformationMatrix" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << CTM << std::endl;

    glm::mat4x4 invCTM = cam.InvCurrentTransformationMatrix();
    std::cout << "InvCurrentTransformationMatrix" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << invCTM << std::endl;

    std::cout << "CTM * invCTM" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << CTM * invCTM << std::endl;

    std::cout << "invCTM * CTM" << std::endl;
    std::cout << horizline << std::endl;
    std::cout << invCTM * CTM << std::endl;
}


int main()
{
    try {
	TestCam1();
	//TestCam2();
	//TestCam3();
    }
    catch (std::exception const& Exception) {
	std::cout << "Exception: " << Exception.what() << std::endl;
    }

    return 0;
}

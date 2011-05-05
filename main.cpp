#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/glfw.h>
#include <iostream>

#include "Entity.h"
#include "AseReader.h"
#include "Material.h"
#include "Error.h"
#include "Camera.h"
#include "SceneHandler.h"

using std::cout;
using std::endl;

void updateLighting(Camera& camera)
{
    float vector4f[4]= {camera.pos.x, camera.pos.y, camera.pos.z, 1.0f}; // Origin, in hom. coords
    glLightfv(GL_LIGHT0, GL_POSITION, vector4f); // Set light position
}

int main(int argc, char *argv[])
{
    int running = GL_TRUE; // Main loop exits when this is set to GL_FALSE
    int mousebtn, lastmousebtn;

    //Did the init not succeed?
    if(!SceneHandler::Init())
    {
        //quit.
        return 0;
    }

    //glEnable(GL_LIGHT0);

    Light *light0 = SceneHandler::CreateLight();

    light0->setPosition(0.0,-100.0,200.0);
    light0->setDirection(0.0,10.0,-10.0);
    light0->setDiffuse(1.0,0.7,0.7);
    light0->setAmbient(0.1,0.1,0.1);
    light0->setSpecular(1.0,1.0,1.0);


//    Light *light1 = SceneHandler::CreateLight();
//
//    light1->setPosition(0.0,200.0,200.0);
//    light1->setDirection(0.0,-10.0,-10.0);
//    light1->setDiffuse(0.7,1.0,0.7);
//    light1->setAmbient(0.1,0.1,0.1);
//    light1->setSpecular(1.0,1.0,1.0);


    mousebtn = lastmousebtn = GLFW_RELEASE;

    Camera camera;
    camera.setRotation(90.0,0.0,0.0);
    camera.setPosition(0.0,0.0,100.0);

    PlayerEntity *player;

    player = SceneHandler::CreatePlayerEntity();
    player->SetPosition(0.0,-100.0,50.0);
    player->minBox=glm::vec4(-12.0,-12.0,-30.0,1.0f);
    player->maxBox=glm::vec4(12.0,12.0,30.0,1.0f);
    player->eyeHeight=25.0;

    PlayerEntity::SetCamera(&camera);

    StaticEntity *scene;
    StaticEntity *beast;
    StaticEntity *box;

    scene = SceneHandler::CreateStaticEntity("media/testscene/testscene.ase",10.0);
    beast = SceneHandler::CreateStaticEntity("media/beast/beast.ase",0.5);
    box = SceneHandler::CreateStaticEntity("media/box/box.ase",1.0);

    beast->SetPosition(0.0,100.0,50.0);
    beast->SetRotation(45.0,0.0,0.0);

    scene->SetPosition(0.0,0.0,0.0);
    scene->SetRotation(180.0,0.0,0.0);

    box->SetPosition(0.0,100.0,150.0);
    //box->SetRotation(0.0,0.0,-20.0);

    SceneHandler::CreateBBoxCollision(box,10.0);
    SceneHandler::CreateBBoxCollision(beast,10.0);
    SceneHandler::CreateMeshCollision(scene);

    SceneHandler::CreatePlayerCollision(player);

    GLint tex_units;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &tex_units);
    cout<<"Max Texture units: "<<tex_units<<endl;

    glfwSwapInterval(0); // Do not wait for screen refresh between frames

    GLuint errorID = glGetError();
    if(errorID != GL_NO_ERROR) {
        printf("\nOpenGL error: %s\n", gluErrorString(errorID));
        printf("Attempting to proceed anyway. Expect rendering errors or a crash.\n");
    }

    // Main loop
    while(running)
    {

        mousebtn=glfwGetMouseButton( GLFW_MOUSE_BUTTON_1 );

        if(mousebtn == GLFW_PRESS)
        {
            glfwEnable( GLFW_MOUSE_CURSOR );
            PlayerEntity::followMouse=false;
        }

        mousebtn=glfwGetMouseButton( GLFW_MOUSE_BUTTON_2 );
        if(mousebtn == GLFW_PRESS)
        {
            glfwDisable( GLFW_MOUSE_CURSOR );
            PlayerEntity::followMouse=true;
        }


        // Update the scene.
        SceneHandler::Update();

        //updateLighting(camera);

        camera.setUp();

        // Finally, draw the scene.
        SceneHandler::Render();

        // Swap buffers, i.e. display the image and prepare for next frame.
        glfwSwapBuffers();

        // Check if the ESC key was pressed or the window was closed.
        if(glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
            running = GL_FALSE;
    }

    // Free the allocated resources
    SceneHandler::Destroy();

    return(0);
}


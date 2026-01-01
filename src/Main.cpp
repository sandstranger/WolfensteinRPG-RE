#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>
#include "SDLGL.h"
#include "ZipFile.h"

#include "CAppContainer.h"
#include "App.h"
#include "Image.h"
#include "Resource.h"
#include "Render.h"
#include "GLES.h"

#include "Canvas.h"
#include "Graphics.h"
#include "Player.h"
#include "Game.h"
#include "Graphics.h"
#include "Utils.h"
#include "TinyGL.h"
#include "Input.h"
#ifdef ANDROID
#include <SDL_main.h>
#endif

void drawView(SDLGL* sdlGL);

#ifdef ANDROID
int SDL_main(int argc, char **argv) {
#else
int main(int argc, char* args[]) {
#endif

#ifdef ANDROID
    chdir(SDL_AndroidGetExternalStoragePath());
#endif
    int		UpTime = 0;

    if (UpTime == 0) {
        UpTime = CAppContainer::getInstance()->getTimeMS();
    }
    
    ZipFile zipFile;
#ifdef ANDROID
    zipFile.openZipFile(getenv("PATH_TO_RESOURCES"));
#else
    zipFile.openZipFile("Wolfenstein RPG.ipa");
#endif
    SDL_SetHint(SDL_HINT_TV_REMOTE_AS_JOYSTICK, "0");
    SDL_SetHint(SDL_HINT_ACCELEROMETER_AS_JOYSTICK, "0");
    SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_RAWINPUT_CORRELATE_XINPUT, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS3, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_STEAMDECK, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_WII, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_COMBINE_JOY_CONS, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS4, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_SWITCH, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_JOY_CONS, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS4_RUMBLE, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5_RUMBLE, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_STEAM, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_GAMECUBE, "1");
    SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI_PS5, "1");

    SDLGL sdlGL;
	sdlGL.Initialize();

    Input input;
    input.init(); // [GEC] Port: set default Binds

    CAppContainer::getInstance()->Construct(&sdlGL, &zipFile);
    sdlGL.updateVideo(); // [GEC]

    while (CAppContainer::getInstance()->app->closeApplet != true) {
        int currentTimeMillis = CAppContainer::getInstance()->getTimeMS();
        if (currentTimeMillis > UpTime) {
            input.handleEvents();
            UpTime = currentTimeMillis + 15;
            drawView(&sdlGL);
            input.consumeEvents();
        }
    }

    printf("APP_QUIT\n");
    CAppContainer::getInstance()->~CAppContainer();
    zipFile.closeZipFile();
    sdlGL.~SDLGL();
    input.~Input();
	return 0;
}


static uint32_t lastTimems = 0;

void drawView(SDLGL *sdlGL) {

    int cx, cy;
    int w = sdlGL->vidWidth;
    int h = sdlGL->vidHeight;

    if (lastTimems == 0) {
        lastTimems = CAppContainer::getInstance()->getTimeMS();
    }

    SDL_GL_GetDrawableSize(sdlGL->window, &cx, &cy);
    if (w != cx || h != cy) {
        w = cx; h = cy;
    }

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, Applet::IOS_WIDTH, Applet::IOS_HEIGHT, 0.0, -1.0, 1.0);
    //glRotatef(90.0, 0.0, 0.0, 1.0);
    //glTranslatef(0.0, -320.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    

    uint32_t startTime = CAppContainer::getInstance()->getTimeMS();
    uint32_t passedTime = startTime - lastTimems;
    lastTimems = startTime;

    if (passedTime >= 125) {
        passedTime = 125;
    }
    //printf("passedTime %d\n", passedTime);

    CAppContainer::getInstance()->DoLoop(passedTime);

    SDL_GL_SwapWindow(sdlGL->window);  // Swap the window/pBmp to display the result.
    
}

#ifdef ANDROID
extern "C" {
void resumeSound() {
    CAppContainer::getInstance()->resumeOpenAL();
}

 void pauseSound() {
    CAppContainer::getInstance()->suspendOpenAL();
}

bool needToShowScreenControls() {
    CAppContainer *appContainer = CAppContainer::getInstance();
    if (!appContainer || !appContainer->app || !appContainer->app->canvas){
        return true;
    }
    int currentCanvasState = appContainer->app->canvas->state;
    return currentCanvasState ==Canvas::ST_PLAYING || currentCanvasState ==Canvas::ST_COMBAT;
}

bool needToInvokeMouseButtonsEvents(){
    return true;
}
}

#endif


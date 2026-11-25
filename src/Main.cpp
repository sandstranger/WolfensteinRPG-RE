#include <stdexcept>
#include <cstdio>
#include <cstdlib>

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
#include <string>
#include "SwappyController.h"

using namespace std;
#endif

#include <atomic>
#include <mutex>
#include <string>

#if ANDROID
// The paths the launcher gives through JNA (setPathsToResources, setPathToSDLControllerDB): set on the java side's thread and
// read on the game's, so guarded by a mutex and read through the functions below (copies)
static std::mutex g_jnaPathsMutex;
static std::string g_pathToIPA;
static std::string g_pathToUserFolder;
static std::string g_pathToSDLControllerDB;

static std::string jnaPath(const std::string &path) {
    std::lock_guard<std::mutex> lock(g_jnaPathsMutex);
    return path;
}

static void setJnaPath(std::string &path, const char *value) {
    std::lock_guard<std::mutex> lock(g_jnaPathsMutex);
    path = (value) ? value : "";    // (JNA passes null for a null String: a std::string of null is undefined)
}

std::string sdlControllerDBPath() {
    return jnaPath(g_pathToSDLControllerDB);
}

// needToShowScreenControls() for the java side, worked out on the game's thread every frame (drawView): the java side's
// thread read the canvas itself, while the game changed it (or destroyed it, at exit)
static std::atomic<int> g_javaShowScreenControls(1);
#endif

//! The folder of the user's files (the font): the launcher's on android, the working directory elsewhere
std::string userFolderPath() {
#if ANDROID
    return jnaPath(g_pathToUserFolder);
#else
    return ".";
#endif
}

void drawView(SDLGL* sdlGL);

#ifdef ANDROID
int SDL_main(int argc, char **argv) {
#else
int main(int argc, char* args[]) {
#endif

#ifdef ANDROID
    chdir(jnaPath(g_pathToUserFolder).c_str());
#endif
    int		UpTime = 0;

    if (UpTime == 0) {
        UpTime = CAppContainer::getInstance()->getTimeMS();
    }
    
    ZipFile zipFile;
#ifdef ANDROID
    zipFile.openZipFile(jnaPath(g_pathToIPA).c_str());
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
    CAppContainer::getInstance()->shutdown();
    zipFile.closeZipFile();
    sdlGL.shutdown();
#ifdef ANDROID
    DestroySwappy();
#endif
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
#ifdef ANDROID
    {
        // (for needToShowScreenControls(), see g_javaShowScreenControls)
        const Canvas *canvas = CAppContainer::getInstance()->app ? CAppContainer::getInstance()->app->canvas : nullptr;
        const int state = canvas ? canvas->state : -1;
        g_javaShowScreenControls = (!canvas || state == Canvas::ST_PLAYING || state == Canvas::ST_COMBAT) ? 1 : 0;
    }
#endif
#ifdef ANDROID
    if (SwappySwapBuffers()){
        return;
    }
#endif
    SDL_GL_SwapWindow(sdlGL->window);  // Swap the window/pBmp to display the result.
}

#ifdef ANDROID
extern "C" {
__attribute__((used)) __attribute__((visibility("default")))
void onNativeResume() {
}
__attribute__((used)) __attribute__((visibility("default")))
 void onNativePause() {
}
// The queries return an int: JNA reads a java 'boolean' result as a 32 bit int, and of a returned C/C++ 'bool' only the
// low byte is defined on arm64 (AAPCS64 leaves the rest of the register unspecified).
__attribute__((used)) __attribute__((visibility("default")))
int needToShowScreenControls() {
    return g_javaShowScreenControls;
}

__attribute__((used)) __attribute__((visibility("default")))
int needToInvokeMouseButtonsEvents(){
    return 1;
}

__attribute__((used)) __attribute__((visibility("default")))
int needToReInitGameControllers (){
    return 0;
}
__attribute__((used)) __attribute__((visibility("default")))
void setPathsToResources (const char *pathToArchive, const char *pathToUserFolder) {
    setJnaPath(g_pathToUserFolder, pathToUserFolder);
    setJnaPath(g_pathToIPA, pathToArchive);
}

__attribute__((used)) __attribute__((visibility("default")))
void setPathToSDLControllerDB (const char *pathToSDLControllerDB){
    setJnaPath(g_pathToSDLControllerDB, pathToSDLControllerDB);
}
}
#endif


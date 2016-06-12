//
//  GameManager.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/23/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "GameManager.hpp"

#ifdef __APPLE__
#include "SDL2/SDL.h"
#include <unistd.h>
#endif
#ifdef _WIN32
#include "SDL.h"
#include <windows.h>
#endif

#include "Logger.hpp"
#include "GraphicsManager.hpp"
#include "Queue.hpp"
#include "Coordinate.hpp"
#include "KeyboardEvent.hpp"
#include "MouseEvent.hpp"

//TODO remove these includes
#include "World.hpp"
#include "Vector.hpp"
#include "Queue.hpp"
#include "Rope.hpp"
#include "Player.hpp"
#include "Polygon.hpp"
#include "Flame.hpp"
#include "Tentacle.hpp"

#define FPS 30
#define AUTO_LOCK_CURSOR false

using namespace nautical;

KeyboardEvent::Key getKeyFromSDLKey(int SDL_key);

bool GameManager::init = false;

SDL_Window * p_window = nullptr;
bool fullscreen = false;

SDL_Renderer * p_renderer = nullptr;

//const int SCREEN_WIDTH = 960;
//const int SCREEN_HEIGHT = 640;
//const int SCREEN_CENTER_X = SCREEN_WIDTH / 2;
//const int SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;

const long targetTime = 1000 / FPS;
int totalCycles = 0;

bool GameManager::running = false;
bool paused = false;
bool reset = false;
unsigned int cycles = 0;

bool GameManager::startup() {
    if (init)
        return init;
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) { //initialize SDL
        Logger::writeLog(ERROR_MESSAGE, "GameManager::init(): %s\n", SDL_GetError());
        return false;
    } else {
        p_window = SDL_CreateWindow("Climber", 0, 0, GraphicsManager::screenWidth, GraphicsManager::screenHeight, SDL_WINDOW_SHOWN); //create window
        if (p_window == nullptr) {
            Logger::writeLog(ERROR_MESSAGE, "GameManager::init(): %s\n", SDL_GetError());
            return false;
        } else {
            if (AUTO_LOCK_CURSOR)
                GraphicsManager::setMouseTrapped(SDL_SetRelativeMouseMode(SDL_TRUE) == 0);
            
            p_renderer = SDL_CreateRenderer(p_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); //init renderer
            if (p_renderer == nullptr) {
                Logger::writeLog(ERROR_MESSAGE, "GameManager::init(): %s\n", SDL_GetError());
                return false;
            } else {
                SDL_SetRenderDrawBlendMode(p_renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, 255);
                SDL_RenderClear(p_renderer);
                
                GraphicsManager::p_renderer = p_renderer;
            }
            
            //init SDL2_mixer for audio
            //if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            //    printf("Main ERROR: %s\n", Mix_GetError());
            //    return false;
            //}
            
            //if using controller, init controller
            //SDL_JoystickEventState(SDL_ENABLE);
            //controller = SDL_JoystickOpen(0);
            //if (controller == nullptr) {
            //    Logger::writeLog("Controller not loaded");
            //} else {
            //    Logger::writeLog("Controller loaded! Controller name: %s\n", SDL_JoystickName(controller));
            //}
        }
    }
    
    return (init = true);
}

bool GameManager::shutdown() {
    if (!init)
        return !init;
    
    //exit mixer
    //Mix_Quit();
    
    //close controller
    //SDL_JoystickClose(controller);
    //controller = nullptr;
    
    //close window and renderer
    SDL_DestroyRenderer(p_renderer);
    p_renderer = nullptr;
    SDL_DestroyWindow(p_window);
    p_window = nullptr;
    SDL_Quit();
    
    return !(init = false);
}

void GameManager::run() {
    if (!startup()) {
        Logger::writeLog(ERROR_MESSAGE, "GameManager::run(): attempted to run game loop without first initializing window");
        return;
    }
    
    Logger::writeLog(PLAIN_MESSAGE, "GameManager::run(): starting game loop...");
    
    reset = false;
    running = true;
    
    Queue<Event*> events;
    World level;
    
    Coordinate playerCoor(390, 480);
    level.addObject(new climber::Player(playerCoor));
    GraphicsManager::setCenter(playerCoor, true);
    
    climber::Flame * flame = new climber::Flame(Coordinate(450, 300));
    flame->addOrigin(40);
    //flame->addOrigin(20, Vector(-15, 0));
    //flame->addOrigin(20, Vector(15, 0));
    level.addObject(flame);
    
    unsigned int start;
    while (running) {
        start = SDL_GetTicks();
        
        events.clear();
        pollEvents(events);
        
        if (!running)
            break;
        
        if (!paused)
            level.update(events);
        
        if (DEBUG_MODE)
            runTests();
        
        GraphicsManager::updateCenter();
        GraphicsManager::updateZoom();
        
        level.draw();
        GraphicsManager::drawCoordinate(GraphicsManager::screenToWorld(GraphicsManager::getMouseCoor()));
        
        SDL_RenderPresent(p_renderer);
        SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, 255);
        SDL_RenderClear(p_renderer);
        
        long elapsed = SDL_GetTicks() - start;
        long wait = (targetTime - elapsed);
        if (wait >= 0) {
#ifdef __APPLE__
            usleep((useconds_t)(wait * 1000));
#endif
#ifdef _WIN32
			Sleep(wait);
#endif
            //printf("Hit deadline with %ld ms to spare! (targetTime: %ld)\n", wait, targetTime);
        } else {
            Logger::writeLog(WARNING_MESSAGE, "Missed deadline by %ld ms", -wait);
        }
    }
    
    if (reset)
        run();
    else
        shutdown();
}

void GameManager::pollEvents(Collection<Event*> & events) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if ((event.type == SDL_KEYDOWN) || (event.type == SDL_KEYUP)) {
            events.insert(new KeyboardEvent(((event.type == SDL_KEYDOWN) ? KeyboardEvent::KEY_PRESSED : KeyboardEvent::KEY_RELEASED), getKeyFromSDLKey(event.key.keysym.sym)));
        }
        
        switch (event.type) {
            case SDL_QUIT: {
                running = false;
                break;
            }
            case SDL_MOUSEMOTION: {
                GraphicsManager::setMouseCoor(Coordinate(event.motion.x, event.motion.y));
                //printf("mouse at world location: %d:%d\n", (int)GraphicsManager::getMouseCoor().getX(), (int)GraphicsManager::getMouseCoor().getY());
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                if (event.button.button == SDL_BUTTON_LEFT)
                    events.insert(new MouseEvent(MouseEvent::LEFT_BUTTON_PRESS));
                else if (event.button.button == SDL_BUTTON_RIGHT)
                    events.insert(new MouseEvent(MouseEvent::RIGHT_BUTTON_PRESS));
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                if (event.button.button == SDL_BUTTON_LEFT)
                    events.insert(new MouseEvent(MouseEvent::LEFT_BUTTON_RELEASE));
                else if (event.button.button == SDL_BUTTON_RIGHT)
                    events.insert(new MouseEvent(MouseEvent::RIGHT_BUTTON_RELEASE));
                break;
            }
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    case SDLK_RETURN:
                        nautical::DEBUG_MODE = !nautical::DEBUG_MODE;
                        break;
                    case SDLK_PERIOD:
                        GraphicsManager::setZoom(GraphicsManager::getZoom() * (4.f / 3.f));
                        break;
                    case SDLK_COMMA:
                        GraphicsManager::setZoom(GraphicsManager::getZoom() * (3.f / 4.f));
                        break;
                    case SDLK_r:
                        running = false;
                        reset = true;
                        break;
                }
                break;
            }
        }
    }
}

void GameManager::switchPause() {
    paused = !paused;
}

void GameManager::setPause(bool pause) {
    paused = pause;
}

KeyboardEvent::Key getKeyFromSDLKey(int SDL_key) {
    switch (SDL_key) {
        case SDLK_SPACE:
            return KeyboardEvent::SPACE;
        case SDLK_RETURN:
        case SDLK_RETURN2:
            return KeyboardEvent::RETURN;
        case SDLK_ESCAPE:
            return KeyboardEvent::ESCAPE;
        case SDLK_TAB:
            return KeyboardEvent::TAB;
        case SDLK_LEFT:
            return KeyboardEvent::LEFTARROW;
        case SDLK_RIGHT:
            return KeyboardEvent::RIGHTARROW;
        case SDLK_UP:
            return KeyboardEvent::UPARROW;
        case SDLK_DOWN:
            return KeyboardEvent::DOWNARROW;
        case SDLK_MINUS:
            return KeyboardEvent::MINUS;
        case SDLK_PLUS:
            return KeyboardEvent::PLUS;
        case SDLK_PERIOD:
            return KeyboardEvent::PERIOD;
        case SDLK_COMMA:
            return KeyboardEvent::COMMA;
        case SDLK_SLASH:
            return KeyboardEvent::SLASH;
        case SDLK_LCTRL:
            return KeyboardEvent::LEFTCONTROL;
        case SDLK_RCTRL:
            return KeyboardEvent::RIGHTCONTROL;
        case SDLK_LSHIFT:
            return KeyboardEvent::LEFTSHIFT;
        case SDLK_RSHIFT:
            return KeyboardEvent::RIGHTSHIFT;
        case SDLK_a:
            return KeyboardEvent::A;
        case SDLK_b:
            return KeyboardEvent::B;
        case SDLK_c:
            return KeyboardEvent::C;
        case SDLK_d:
            return KeyboardEvent::D;
        case SDLK_e:
            return KeyboardEvent::E;
        case SDLK_f:
            return KeyboardEvent::F;
        case SDLK_g:
            return KeyboardEvent::G;
        case SDLK_h:
            return KeyboardEvent::H;
        case SDLK_i:
            return KeyboardEvent::I;
        case SDLK_j:
            return KeyboardEvent::J;
        case SDLK_k:
            return KeyboardEvent::K;
        case SDLK_l:
            return KeyboardEvent::L;
        case SDLK_m:
            return KeyboardEvent::M;
        case SDLK_n:
            return KeyboardEvent::N;
        case SDLK_o:
            return KeyboardEvent::O;
        case SDLK_p:
            return KeyboardEvent::P;
        case SDLK_q:
            return KeyboardEvent::Q;
        case SDLK_r:
            return KeyboardEvent::R;
        case SDLK_s:
            return KeyboardEvent::S;
        case SDLK_t:
            return KeyboardEvent::T;
        case SDLK_u:
            return KeyboardEvent::U;
        case SDLK_v:
            return KeyboardEvent::V;
        case SDLK_w:
            return KeyboardEvent::W;
        case SDLK_x:
            return KeyboardEvent::X;
        case SDLK_y:
            return KeyboardEvent::Y;
        case SDLK_z:
            return KeyboardEvent::Z;
        case SDLK_0:
            return KeyboardEvent::NUM0;
        case SDLK_1:
            return KeyboardEvent::NUM1;
        case SDLK_2:
            return KeyboardEvent::NUM2;
        case SDLK_3:
            return KeyboardEvent::NUM3;
        case SDLK_4:
            return KeyboardEvent::NUM4;
        case SDLK_5:
            return KeyboardEvent::NUM5;
        case SDLK_6:
            return KeyboardEvent::NUM6;
        case SDLK_7:
            return KeyboardEvent::NUM7;
        case SDLK_8:
            return KeyboardEvent::NUM8;
        case SDLK_9:
            return KeyboardEvent::NUM9;
        case SDLK_F1:
            return KeyboardEvent::F1;
        case SDLK_F2:
            return KeyboardEvent::F2;
        case SDLK_F3:
            return KeyboardEvent::F3;
        case SDLK_F4:
            return KeyboardEvent::F4;
        case SDLK_F5:
            return KeyboardEvent::F5;
        case SDLK_F6:
            return KeyboardEvent::F6;
        case SDLK_F7:
            return KeyboardEvent::F7;
        case SDLK_F8:
            return KeyboardEvent::F8;
        case SDLK_F9:
            return KeyboardEvent::F9;
        case SDLK_F10:
            return KeyboardEvent::F10;
        case SDLK_F11:
            return KeyboardEvent::F11;
        case SDLK_F12:
            return KeyboardEvent::F12;
        case SDLK_F13:
            return KeyboardEvent::F13;
        case SDLK_F14:
            return KeyboardEvent::F14;
        case SDLK_F15:
            return KeyboardEvent::F15;
        case SDLK_F16:
            return KeyboardEvent::F16;
        case SDLK_F17:
            return KeyboardEvent::F17;
        case SDLK_F18:
            return KeyboardEvent::F18;
        case SDLK_F19:
            return KeyboardEvent::F19;
        case SDLK_F20:
            return KeyboardEvent::F20;
        case SDLK_F21:
            return KeyboardEvent::F21;
        case SDLK_F22:
            return KeyboardEvent::F22;
        case SDLK_F23:
            return KeyboardEvent::F23;
        case SDLK_F24:
            return KeyboardEvent::F24;
        default:
            return KeyboardEvent::UNDEFINED_KEY;
    }
}

void GameManager::runTests() {
    /*Queue<Coordinate> coors;
    coors.insert(Coordinate(150, 250));
    coors.insert(Coordinate(220, 300));
    coors.insert(Coordinate(220, 150));
    coors.insert(Coordinate(200, 180));
    coors.insert(Coordinate(150, 150));
    coors.insert(Coordinate(100, 100));
    static Polygon polygon(&coors);
    
    bool contains = polygon.contains(GraphicsManager::getMouseCoor());
    
    polygon.drawFilled(RED);
    polygon.Drawable::draw(contains ? GREEN : WHITE);*/
}

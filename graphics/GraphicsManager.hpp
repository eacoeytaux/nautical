//
//  GraphicsManager.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/24/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef GraphicsManager_hpp
#define GraphicsManager_hpp

#include <string>

#ifdef __APPLE__
#include "SDL2/SDL.h"
#endif
#ifdef _WIN32
#include "SDL.h"
#endif

#include "SpriteSheet.hpp"
#include "Color.hpp"
#include "Angle.hpp"
#include "Coordinate.hpp"
#include "Line.hpp"
#include "Parabola.hpp"

namespace nautical {
    class GraphicsManager {
    public:
        static bool startup(SDL_Window * p_window); //TODO combine .cpp file with GameManager.cpp to avoid dependencies on SDL?
        static bool shutdown();
        static bool draw(); //draws canvas on screen
        
        static int getScreenWidth();
        static int getScreenHeight();
        
        static int getPixelScale();
        static void setPixelScale(int scale);
        
        //converts world coordinates to screen coordinates (for drawing)
        static Coordinate worldToScreen(const Coordinate & coor);
        static double worldToScreenX(double x);
        static double worldToScreenY(double y);
        
        //converts screen coordinate to world coordinates (for input such as mouse)
        static Coordinate screenToWorld(const Coordinate & coor);
        static double screenToWorldX(double x);
        static double screenToWorldY(double y);
        
        static Coordinate getMouseCoor();
        static void setMouseCoor(const Coordinate & coor);
        static bool isMouseTrapped();
        static void setMouseTrapped(bool mouseTrapped);
        
        static void updateCenter(); //moves actual center closer to target center
        static Coordinate getCenter();
        static Coordinate getCenterTarget();
        static void setCenter(const Coordinate & center, bool hardSet = false); //if (hardSet == true) then the actual value is set to target value
        static void setCenterSpeedRatio(float centerSpeedRatio);
        
        static void updateZoom(); //moves actual zoom value closer to target zoom value
        static float getZoom();
        static float getZoomTarget();
        static void setZoom(float zoom, bool hardSet = false); //if (hardSet == true) then the actual value is set to target value
        static void setZoomSpeedRatio(float zoomSpeedRatio);
        
        //used to draw on canvas
        //if (adjust == true) then coordinates are treated as world coordinates and converted to screen coordinates
        static void drawCoordinate(const Coordinate & coor, Color color = DEFAULT_COLOR, bool adjust = true);
        static void drawCoordinate(double x, double y, Color color = DEFAULT_COLOR, bool adjust = true);
        static void drawLine(const Line & line, Color color = DEFAULT_COLOR, bool adjust = true);
        static void drawLine(double x1, double y1, double x2, double y2, Color color = DEFAULT_COLOR, bool adjust = true);
        static void drawParabola(const Parabola & parabola, Color color = DEFAULT_COLOR, bool adjust = true);
        static void drawImageFromSpriteSheet(const SpriteSheet * p_sheet, int frame, Coordinate coor, Angle angle = Angle(0), bool flipHorizontal = false, bool flipVertical = false, int alpha = 255, bool adjust = true);
        
        //used to load graphic images
        static SpriteSheet * loadSpriteSheet(std::string filePath, int widthCount, int heightCount, int scale = 1);
        
    private:
        static bool init;
        
        static int screenWidth, //width of screen in pixels
        screenHeight, //height of screen in pixels
        pixelScale; //independent pixel resolution from monitor
        
        static Coordinate mouseCoor; //stored in screen coordinates
        static bool mouseTrapped; //whether or not mouse is trapped within window (when not fullscreen)
        
        static double xOffset, yOffset; //x and y offsets of world to screen coordinates
        
        static Coordinate center, centerTarget; //world center of camera (actual and target)
        static float centerSpeedRatio; //speed at which actual center approaches target (v = (target - set) * ratio)
        
        static float zoom, zoomTarget, zoomSpeedRatio;
        
        static void setOffsets(); //function for setting offsets (called whenever values that change offset are changed)
    };
}

#endif /* GraphicsManager_hpp */

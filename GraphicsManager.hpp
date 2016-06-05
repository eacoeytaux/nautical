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

#include "SDL2/SDL.h"

#include "SpriteSheet.hpp"
#include "Color.hpp"
#include "Angle.hpp"
#include "Coordinate.hpp"
#include "Line.hpp"
#include "Parabola.hpp"

namespace nautical {
    class GraphicsManager {
        friend class GameManager;
        
    public:
        static int getScreenWidth();
        static int getScreenHeight();
        
        static Coordinate worldToScreen(Coordinate coor);
        static double worldToScreenX(double x);
        static double worldToScreenY(double y);
        static Coordinate screenToWorld(Coordinate coor);
        static double screenToWorldX(double x);
        static double screenToWorldY(double y);
        
        static Coordinate getMouseCoor();
        static void setMouseCoor(Coordinate coor);
        static bool isMouseTrapped();
        static void setMouseTrapped(bool mouseTrapped);
        
        static void updateCenter();
        static Coordinate getCenter();
        static Coordinate getCenterSet();
        static void setCenter(Coordinate center, bool hardSet = false);
        static void setCenterSpeedRatio(float centerSpeedRatio);
        
        static void updateZoom();
        static float getZoom();
        static float getZoomSet();
        static void setZoom(float zoom, bool hardSet = false);
        static void setZoomSpeedRatio(float zoomSpeedRatio);
        
        static void drawCoordinate(Coordinate coor, Color color = DEFAULT_COLOR, bool adjust = true);
        static void drawCoordinate(double x, double y, Color color = DEFAULT_COLOR, bool adjust = true);
        static void drawLine(Line line, Color color = DEFAULT_COLOR, bool adjust = true);
        static void drawLine(double x1, double y1, double x2, double y2, Color color = DEFAULT_COLOR, bool adjust = true);
        static void drawParabola(Parabola parabola, Color color = DEFAULT_COLOR, bool adjust = true);
        
        static SpriteSheet * loadSpriteSheet(std::string filePath, int widthCount, int heightCount, int scale = 1);
        static void drawImageFromSpriteSheet(const SpriteSheet * p_sheet, int frame, Coordinate coor, Angle angle, bool flipHorizontal, bool flipVertical, int alpha = 255, bool adjust = true);
        
    private:
        static SDL_Renderer * p_renderer;
        
        static int screenWidth,
        screenHeight;
        
        static Coordinate mouseCoor; //stored in screen coordinates
        static bool mouseTrapped;
        
        static double xOffset, yOffset;
        static Coordinate center, centerSet;
        static float centerSpeedRatio;
        
        static float zoom, zoomSet, zoomSpeedRatio;
        
        static void setOffsets();
    };
}

#endif /* GraphicsManager_hpp */

//
//  GraphicsManager.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/24/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#ifndef GraphicsManager_hpp
#define GraphicsManager_hpp

#include "SDL2/SDL.h"

#include "Color.hpp"
#include "Coordinate.hpp"
#include "Line.hpp"
#include "Image.hpp"

namespace nautical {
    class GraphicsManager {
        friend class GameManager;
        
    public:
        static Coordinate getMouseCoor();
        static void setMouseCoor(Coordinate coor);
        static void setMouseCoorScreenCoordinates(Coordinate coor);
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
        
        static void drawCoordinate(Coordinate coor, Color color = DEFAULT_COLOR);
        static void drawLine(Line line, Color color = DEFAULT_COLOR);
        
        Image * createImage(std::string filePath, int scale = 1);
        
    private:
        static SDL_Renderer * p_renderer;
        
        static Coordinate mouseCoor;
        static bool mouseTrapped;
        
        static double xOffset, yOffset;
        static Coordinate center, centerSet;
        static float centerSpeedRatio;
        
        static float zoom, zoomSet, zoomSpeedRatio;
        
        static void setOffsets();
    };
}

#endif /* GraphicsManager_hpp */

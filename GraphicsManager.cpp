//
//  GraphicsManager.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 9/24/15.
//  Copyright © 2015 EthanCo. All rights reserved.
//

#include "GraphicsManager.hpp"

#include "GameManager.hpp"
#include "Utility.hpp"
#include "Vector.hpp"

#define CENTER_OFFSET_MINIMUM 1
#define ZOOM_OFFSET_MINIMUM 0.00001

using namespace nautical;

int GraphicsManager::screenWidth = 960;
int GraphicsManager::screenHeight = 640;

SDL_Renderer * GraphicsManager::p_renderer = nullptr;

Coordinate GraphicsManager::mouseCoor = Coordinate(0, 0);
bool GraphicsManager::mouseTrapped = false;

double GraphicsManager::xOffset = 0;
double GraphicsManager::yOffset = 0;
Coordinate GraphicsManager::center = Coordinate(0, 0);
Coordinate GraphicsManager::centerSet = Coordinate(0, 0);
float GraphicsManager::centerSpeedRatio = 0.125;

float GraphicsManager::zoom = 1;
float GraphicsManager::zoomSet = 1;
float GraphicsManager::zoomSpeedRatio = 0.125;

void setRenderColor(SDL_Renderer * p_renderer, Color color) {
    SDL_SetRenderDrawColor(p_renderer, color.getR(), color.getG(), color.getB(), color.getA());
}

int GraphicsManager::getScreenWidth() {
    return screenWidth;
}

int GraphicsManager::getScreenHeight() {
    return screenHeight;
}

void GraphicsManager::setOffsets() {
    xOffset = (center.getX() * -zoom) + (screenWidth / 2);
    yOffset = (center.getY() * zoom) + (screenHeight / 2);
}

Coordinate GraphicsManager::worldToScreen(Coordinate coor) {
    return Coordinate(worldToScreenX(coor.getX()), worldToScreenY(coor.getY()));
}

double GraphicsManager::worldToScreenX(double x) {
    return ceil(x * zoom) + xOffset;
}

double GraphicsManager::worldToScreenY(double y) {
    return -ceil(y * zoom) + yOffset;
}

Coordinate GraphicsManager::screenToWorld(Coordinate coor) {
    return Coordinate(screenToWorldX(coor.getX()), screenToWorldY(coor.getY()));
}

double GraphicsManager::screenToWorldX(double x) {
    return (x - xOffset) / zoom;
}

double GraphicsManager::screenToWorldY(double y) {
    return (y - yOffset) / -zoom;
}

Coordinate GraphicsManager::getMouseCoor() {
    return mouseCoor;
}

void GraphicsManager::setMouseCoor(Coordinate coor) {
    mouseCoor = coor;
}

bool GraphicsManager::isMouseTrapped() {
    return mouseTrapped;
}

void GraphicsManager::setMouseTrapped(bool mouseTrapped) {
    GraphicsManager::mouseTrapped = mouseTrapped;
}

void GraphicsManager::updateCenter() {
    if (findDistance(center, centerSet) > CENTER_OFFSET_MINIMUM) {
        center += Vector(center, centerSet) * fmin(centerSpeedRatio, 1);
    } else {
        center = centerSet;
    }
    setOffsets();
}

Coordinate GraphicsManager::getCenter() {
    return GraphicsManager::center;
}

Coordinate GraphicsManager::getCenterSet() {
    return GraphicsManager::centerSet;
}

void GraphicsManager::setCenter(Coordinate center, bool hardSet) {
    if (hardSet) {
        GraphicsManager::center = center;
        setOffsets();
    }
    centerSet = center;
}

void GraphicsManager::setCenterSpeedRatio(float centerSpeedRatio) {
    if ((centerSpeedRatio > 1) || (centerSpeedRatio <= 0))
        Logger::writeLog(WARNING, "GraphicsManager::setCenterSpeedRatio(): centerSpeedRatio must be between [1, 0)");
    else
        GraphicsManager::centerSpeedRatio = centerSpeedRatio;
}

void GraphicsManager::updateZoom() {
    if (fabs(zoom - zoomSet) > ZOOM_OFFSET_MINIMUM) {
        zoom += (zoomSet - zoom) * zoomSpeedRatio;
    } else {
        zoom = zoomSet;
    }
    setOffsets();
}

float GraphicsManager::getZoom() {
    return zoom;
}

float GraphicsManager::getZoomSet() {
    return zoomSet;
}

void GraphicsManager::setZoom(float zoom, bool hardSet) {
    if (hardSet) {
        GraphicsManager::zoom = zoom;
        setOffsets();
    }
    zoomSet = zoom;
}

void GraphicsManager::setZoomSpeedRatio(float zoomSpeedRatio) {
    if ((zoomSpeedRatio > 1) || (zoomSpeedRatio <= 0))
        Logger::writeLog(WARNING, "GraphicsManager::setZoomSpeedRatio(): zoomSpeedRatio must be between [1, 0)");
    else
        GraphicsManager::zoomSpeedRatio = zoomSpeedRatio;
}

void GraphicsManager::drawCoordinate(Coordinate coor, Color color, bool adjust) {
    if (!p_renderer)
        return;
    
    if (adjust)
        coor = worldToScreen(coor);
    
    static SDL_Rect rect;
    rect.x = coor.getX() - 1;
    rect.y = coor.getY() - 1;
    rect.w = 3;
    rect.h = 3;
    
    setRenderColor(p_renderer, color);
    SDL_RenderFillRect(p_renderer, &rect);
}

void GraphicsManager::drawLine(Line line, Color color, bool adjust) {
    if (!p_renderer)
        return;
    
    Coordinate coor1 = line.getCoor1();
    Coordinate coor2 = line.getCoor2();
    
    if (adjust) {
        coor1 = worldToScreen(coor1);
        coor2 = worldToScreen(coor2);
    }
    
    setRenderColor(p_renderer, color);
    SDL_RenderDrawLine(p_renderer, coor1.getX(), coor1.getY(), coor2.getX(), coor2.getY());
}

Image * GraphicsManager::createImage(std::string filePath, int scale) {
    bool failureFlag = false;
    Image * p_image = new Image(&failureFlag, p_renderer, filePath, scale);
    if (failureFlag) {
        Logger::writeLog(ERROR, "GraphicsManager::createImage(): could not load image");
        return nullptr;
    } else {
        return p_image;
    }
}

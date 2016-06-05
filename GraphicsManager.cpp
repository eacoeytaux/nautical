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
    drawCoordinate(coor.getX(), coor.getY(), color, adjust);
}

void GraphicsManager::drawCoordinate(double x, double y, Color color, bool adjust) {
    if (!p_renderer)
        return;
    
    if (adjust) {
        x = worldToScreenX(x);
        y = worldToScreenY(y);
    }
    
    static SDL_Rect rect;
    rect.x = x - 1;
    rect.y = y - 1;
    rect.w = 3;
    rect.h = 3;
    
    setRenderColor(p_renderer, color);
    SDL_RenderFillRect(p_renderer, &rect);
}

void GraphicsManager::drawLine(Line line, Color color, bool adjust) {
    drawLine(line.getCoor1().getX(), line.getCoor1().getY(), line.getCoor2().getX(), line.getCoor2().getY(), color, adjust);
}

void GraphicsManager::drawLine(double x1, double y1, double x2, double y2, Color color, bool adjust) {
    if (!p_renderer)
        return;
    
    if (adjust) {
        x1 = worldToScreenX(x1);
        y1 = worldToScreenY(y1);
        x2 = worldToScreenX(x2);
        y2 = worldToScreenY(y2);
    }
    
    setRenderColor(p_renderer, color);
    SDL_RenderDrawLine(p_renderer, x1, y1, x2, y2);
}

void GraphicsManager::drawParabola(Parabola parabola, Color color, bool adjust) {
    Coordinate coor1 = parabola.getLeftCoor();
    Coordinate coor2 = parabola.getRightCoor();
    
    int newY, lastY = parabola.calculateY(coor1.getX());
    for (int x = coor1.getX() + 1; x < coor2.getX(); x++) {
        newY = parabola.calculateY(x);
        drawLine(x - 1, lastY, x, newY, color, adjust);
        lastY = newY;
    }
    drawLine(coor2.getX() - 1, lastY, coor2.getX(), coor2.getY(), color, adjust);
}

SpriteSheet * GraphicsManager::loadSpriteSheet(std::string filePath, int widthCount, int heightCount, int scale) {
    bool failureFlag = false;
    SpriteSheet * p_sheet = new SpriteSheet(&failureFlag, p_renderer, filePath, widthCount, heightCount, scale);
    if (failureFlag) {
        Logger::writeLog(ERROR, "GraphicsManager::createSpriteSheet(): could not load spritesheet from filepath %s", filePath.c_str());
        return nullptr;
    } else {
        return p_sheet;
    }
}

void GraphicsManager::drawImageFromSpriteSheet(const SpriteSheet * p_sheet, int frame, Coordinate coor, Angle angle, bool flipHorizontal, bool flipVertical, int alpha, bool adjust) {
    if (!p_renderer)
        return;
    
    if (adjust)
        coor = worldToScreen(coor);
    
    int width = p_sheet->getImageWidth();
    int height = p_sheet->getImageHeight();
    int widthCount = p_sheet->getWidthCount();
    int heightCount = p_sheet->getHeightCount();
    float scale = p_sheet->getScale() * zoom;
    
    if (scale == 0)
        return;
    
    int x = frame % widthCount;
    int y = frame / widthCount;
    
    SDL_Rect srcRect;
    srcRect.x = x * (widthCount / width);
    srcRect.y = y * (heightCount / height);
    srcRect.w = width / widthCount;
    srcRect.h = height / heightCount;
    
    SDL_Rect dstRect;
    dstRect.x = coor.getX() - (scale * width / widthCount / 2);
    dstRect.y = coor.getY() - (scale * height / heightCount / 2);
    dstRect.w = scale * width / widthCount;
    dstRect.h = scale * height / heightCount;
    
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (flipHorizontal || flipVertical) {
        if (!flipVertical)
            flip = SDL_FLIP_HORIZONTAL;
        else if (!flipHorizontal)
            flip = SDL_FLIP_VERTICAL;
        else
            flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
    }
    
    SDL_SetTextureAlphaMod(p_sheet->getTexture(), alpha); //TODO only call this when necessary?
    
    SDL_RenderCopyEx(p_renderer, p_sheet->getTexture(), &srcRect, &dstRect, Angle::radiansToDegrees(angle.getValue()), nullptr, flip);
}

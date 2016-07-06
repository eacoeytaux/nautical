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

#define AUTO_LOCK_CURSOR false

#define CENTER_OFFSET_MINIMUM 1
#define ZOOM_OFFSET_MINIMUM 0.00001

using namespace nautical;

bool GraphicsManager::init = false;

SDL_Renderer * p_renderer = nullptr;
SDL_Texture * p_texture = nullptr;

int GraphicsManager::screenWidth = 960;
int GraphicsManager::screenHeight = 640;
int GraphicsManager::pixelScale = 1;

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

bool GraphicsManager::startup(SDL_Window * p_window) {
    if (init)
        return init;
    
    if (AUTO_LOCK_CURSOR)
        GraphicsManager::setMouseTrapped(SDL_SetRelativeMouseMode(SDL_TRUE) == 0);
    
    if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0") == SDL_FALSE)
        Logger::writeLog(ERROR_MESSAGE, "GraphicsManager::startup(): could not set hint SDL_HINT_RENDER_SCALE_QUALITY to \"0\"");
    
    p_renderer = SDL_CreateRenderer(p_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); //init renderer
    if (!p_renderer) {
        Logger::writeLog(ERROR_MESSAGE, "GraphicsManager::startup(): %s", SDL_GetError());
        return false;
    } else {
        SDL_SetRenderDrawBlendMode(p_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, 255);
        SDL_RenderClear(p_renderer);
        
        p_texture = SDL_CreateTexture(p_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
        if (!p_texture) {
            Logger::writeLog(ERROR_MESSAGE, "GraphicsManager::startup(): %s", SDL_GetError());
            return false;
        } else {
            SDL_SetRenderTarget(p_renderer, p_texture);
        }
    }
    
    return (init = true);
}

bool GraphicsManager::shutdown() {
    if (!init)
        return !init;
    
    //destroy renderer and texture
    SDL_DestroyRenderer(p_renderer);
    p_renderer = nullptr;
    SDL_DestroyTexture(p_texture);
    p_texture = nullptr;
    
    return !(init = false);
}

bool GraphicsManager::draw() {
    if (!init) {
        Logger::writeLog(ERROR_MESSAGE, "GraphicsManager::draw(): GraphicsManager not init");
        return false;
    }
    
    SDL_SetRenderTarget(p_renderer, nullptr);
    SDL_Rect srcRect = {0, 0, GraphicsManager::screenWidth, GraphicsManager::screenHeight};
    SDL_Rect dstRect = {0, 0, GraphicsManager::screenWidth * pixelScale, GraphicsManager::screenHeight * pixelScale};
    SDL_RenderCopyEx(p_renderer, p_texture, &srcRect, &dstRect, 0, nullptr, SDL_FLIP_NONE);
    SDL_RenderPresent(p_renderer);
    SDL_SetRenderTarget(p_renderer, p_texture);
    SECRET_MODE ? SDL_SetRenderDrawColor(p_renderer, 255, 255, 255, 255) : SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, 255);
    SDL_RenderClear(p_renderer);
    return true;
}

int GraphicsManager::getScreenWidth() {
    return screenWidth;
}

int GraphicsManager::getScreenHeight() {
    return screenHeight;
}

int GraphicsManager::getPixelScale() {
    return GraphicsManager::pixelScale;
}

void GraphicsManager::setPixelScale(int scale) {
    float ratio = pixelScale / (float)scale;
    mouseCoor.setX(mouseCoor.getX() * ratio);
    mouseCoor.setY(mouseCoor.getY() * ratio);
    zoomSet *= ratio;
    zoom *= ratio;
    pixelScale = scale;
}

void GraphicsManager::setOffsets() {
    xOffset = (center.getX() * -zoom) + (screenWidth / (2 * pixelScale));
    yOffset = (center.getY() * zoom) + (screenHeight / (2 * pixelScale));
}

Coordinate GraphicsManager::worldToScreen(const Coordinate & coor) {
    return Coordinate(worldToScreenX(coor.getX()), worldToScreenY(coor.getY()));
}

double GraphicsManager::worldToScreenX(double x) {
    return ceil(x * zoom) + xOffset;
}

double GraphicsManager::worldToScreenY(double y) {
    return -ceil(y * zoom) + yOffset;
}

Coordinate GraphicsManager::screenToWorld(const Coordinate & coor) {
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

void GraphicsManager::setMouseCoor(const Coordinate & coor) {
    mouseCoor = coor;
}

bool GraphicsManager::isMouseTrapped() {
    return mouseTrapped;
}

void GraphicsManager::setMouseTrapped(bool mouseTrapped) {
    GraphicsManager::mouseTrapped = mouseTrapped;
}

void GraphicsManager::updateCenter() {
    if (!DEBUG_MODE && (findDistance(center, centerSet) > CENTER_OFFSET_MINIMUM)) {
        center += physics::Vector(center, centerSet) * fmin(centerSpeedRatio, 1);
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

void GraphicsManager::setCenter(const Coordinate & center, bool hardSet) {
    if (hardSet) {
        GraphicsManager::center = center;
        setOffsets();
    }
    centerSet = center;
}

void GraphicsManager::setCenterSpeedRatio(float centerSpeedRatio) {
    if ((centerSpeedRatio > 1) || (centerSpeedRatio <= 0))
        Logger::writeLog(WARNING_MESSAGE, "GraphicsManager::setCenterSpeedRatio(): centerSpeedRatio must be between [1, 0)");
    else
        GraphicsManager::centerSpeedRatio = centerSpeedRatio;
}

void GraphicsManager::updateZoom() {
    if (!DEBUG_MODE && (fabs(zoom - zoomSet) > ZOOM_OFFSET_MINIMUM)) {
        zoom += (zoomSet - zoom) * zoomSpeedRatio;
    } else {
        zoom = zoomSet;
    }
    setOffsets();
}

float GraphicsManager::getZoom() {
    return zoom * pixelScale;
}

float GraphicsManager::getZoomSet() {
    return zoomSet * pixelScale;
}

void GraphicsManager::setZoom(float zoom, bool hardSet) {
    zoom /= pixelScale;
    if (hardSet) {
        GraphicsManager::zoom = zoom;
        setOffsets();
    }
    zoomSet = zoom;
}

void GraphicsManager::setZoomSpeedRatio(float zoomSpeedRatio) {
    if ((zoomSpeedRatio > 1) || (zoomSpeedRatio <= 0))
        Logger::writeLog(WARNING_MESSAGE, "GraphicsManager::setZoomSpeedRatio(): zoomSpeedRatio must be between [1, 0)");
    else
        GraphicsManager::zoomSpeedRatio = zoomSpeedRatio;
}

void GraphicsManager::drawCoordinate(const Coordinate & coor, Color color, bool adjust) {
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
    rect.x = (int)x - 1;
    rect.y = (int)y - 1;
    rect.w = 3;
    rect.h = 3;
    
    setRenderColor(p_renderer, color);
    SDL_RenderFillRect(p_renderer, &rect);
}

void GraphicsManager::drawLine(const Line & line, Color color, bool adjust) {
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
    SDL_RenderDrawLine(p_renderer, (int)x1, (int)y1, (int)x2, (int)y2);
}

void GraphicsManager::drawParabola(const Parabola & parabola, Color color, bool adjust) {
    Coordinate coor1 = parabola.getLeftCoor();
    Coordinate coor2 = parabola.getRightCoor();
    
    int newY, lastY = (int)parabola.calculateY(coor1.getX());
    for (int x = (int)coor1.getX() + 1; x < coor2.getX(); x++) {
        newY = (int)parabola.calculateY(x);
        drawLine(x - 1, lastY, x, newY, color, adjust);
        lastY = newY;
    }
    drawLine(coor2.getX() - 1, lastY, coor2.getX(), coor2.getY(), color, adjust);
}

SpriteSheet * GraphicsManager::loadSpriteSheet(std::string filePath, int widthCount, int heightCount, int scale) {
    bool failureFlag = false;
    SpriteSheet * p_sheet = new SpriteSheet(&failureFlag, p_renderer, filePath, widthCount, heightCount, scale);
    if (failureFlag) {
        Logger::writeLog(ERROR_MESSAGE, "GraphicsManager::createSpriteSheet(): could not load spritesheet from filepath %s", filePath.c_str());
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
    dstRect.x = (int)(coor.getX() - (scale * width / widthCount / 2));
    dstRect.y = (int)(coor.getY() - (scale * height / heightCount / 2));
    dstRect.w = (int)(scale * width / widthCount);
    dstRect.h = (int)(scale * height / heightCount);
    
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (flipHorizontal || flipVertical) {
        if (!flipVertical)
            flip = SDL_FLIP_HORIZONTAL;
        else if (!flipHorizontal)
            flip = SDL_FLIP_VERTICAL;
        else
            flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
    }
    
    static int lastAlpha = 255;
    if (alpha != lastAlpha) {
        SDL_SetTextureAlphaMod(p_sheet->getTexture(), (Uint8)alpha);
        lastAlpha = alpha;
    }
    
    SDL_RenderCopyEx(p_renderer, p_sheet->getTexture(), &srcRect, &dstRect, Angle::radiansToDegrees(angle.getValue()), nullptr, flip);
}

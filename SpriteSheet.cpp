//
//  SpriteSheet.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 6/1/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "SpriteSheet.hpp"

#ifdef __APPLE__
#include "SDL2_image/SDL_image.h"
#endif
#ifdef _WIN32
#include "SDL_image.h"
#endif

#include "Logger.hpp"

using namespace nautical;

SpriteSheet::SpriteSheet(bool * failureFlag, SDL_Renderer * p_renderer, std::string filePath, int widthCount, int heightCount, float scale) : widthCount(widthCount), heightCount(heightCount), scale(scale) {
	SDL_Surface * p_loadedSurface = IMG_Load(filePath.c_str());
    
    if (!p_loadedSurface) {
        Logger::writeLog(ERROR_MESSAGE, "SpriteSheet::SpriteSheet(): Unable to load image %s: SDL_image Error: %s", filePath.c_str(), IMG_GetError());
        
        if (failureFlag != nullptr)
            *failureFlag = true;
    } else {
        p_texture = SDL_CreateTextureFromSurface(p_renderer, p_loadedSurface);
        if (!p_texture) {
            Logger::writeLog(ERROR_MESSAGE, "SpriteSheet::SpriteSheet(): Unable to create texture from %s: SDL Error: %s", filePath.c_str(), SDL_GetError());
            
            if (failureFlag != nullptr)
                *failureFlag = true;
        }
        SDL_FreeSurface(p_loadedSurface);
        
        if (SDL_QueryTexture(p_texture, nullptr, nullptr, &imageWidth, &imageHeight) < 0) {
            Logger::writeLog(ERROR_MESSAGE, "SpriteSheet::SpriteSheet(): %s", SDL_GetError());
        }
    }
}

SpriteSheet::~SpriteSheet() {
    SDL_DestroyTexture(p_texture);
    p_texture = nullptr;
}

SDL_Texture * SpriteSheet::getTexture() const {
    return p_texture;
}

int SpriteSheet::getImageWidth() const {
    return imageWidth;
}

int SpriteSheet::getImageHeight() const {
    return imageHeight;
}

int SpriteSheet::getWidthCount() const {
    return widthCount;
}

int SpriteSheet::getHeightCount() const {
    return heightCount;
}

float SpriteSheet::getScale() const {
    return scale;
}

void SpriteSheet::setScale(float scale) const {
    this->scale = scale;
}


//
//  Image.cpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/12/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "Image.hpp"

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"

#include "Logger.hpp"

using namespace nautical;

Image::Image(bool * p_failureFlag, SDL_Renderer * p_renderer, std::string filePath, int scale) {
    this->scale = scale;
    
    SDL_Surface * p_loadedSurface = IMG_Load(filePath.c_str());
    
    if (!p_loadedSurface) {
        Logger::writeLog(ERROR, "Image::Image(): Unable to load image %s: SDL_image Error: %s\n", filePath.c_str(), IMG_GetError());
        
        if (p_failureFlag)
            *p_failureFlag = true;
    } else {
        p_texture = SDL_CreateTextureFromSurface(p_renderer, p_loadedSurface);
        if (!p_texture) {
            Logger::writeLog(ERROR, "Image::Image(): Unable to create texture from %s: SDL Error: %s\n", filePath.c_str(), SDL_GetError());
            
            if (p_failureFlag)
                *p_failureFlag = true;
        }
        SDL_FreeSurface(p_loadedSurface);
        
        if (SDL_QueryTexture(p_texture, nullptr, nullptr, &width, &height) < 0) {
            Logger::writeLog(ERROR, "Image::Image(): %s\n", SDL_GetError());
            if (p_failureFlag)
                *p_failureFlag = true;
        } else {
            width *= scale;
            height *= scale;
        }
    }
}

Image::Image(bool * failureFlag, SDL_Texture * p_texture, int scale) : p_texture(p_texture), scale(scale) {
    if (SDL_QueryTexture(p_texture, nullptr, nullptr, &width, &height) < 0) {
        Logger::writeLog(ERROR, "Image::Image(): %s\n", SDL_GetError());
        if (failureFlag)
            *failureFlag = true;
    } else {
        width *= scale;
        height *= scale;
    }
}

Image::~Image() {
    SDL_DestroyTexture(p_texture);
    p_texture = nullptr;
}

int Image::getWidth() const {
    return width;
}

int Image::getHeight() const {
    return height;
}

int Image::getScale() const {
    return scale;
}

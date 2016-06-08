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


using namespace nautical;

SpriteSheet::SpriteSheet(bool * failureFlag, SDL_Renderer * renderer, std::string filePath, int widthCount, int heightCount, float scale) : widthCount(widthCount), heightCount(heightCount), scale(scale) {
    SDL_Surface * loadedSurface = IMG_Load(filePath.c_str());
    
    if (loadedSurface == nullptr) {
        printf("SpriteSheet Error: Unable to load image %s: SDL_image Error: %s\n", filePath.c_str(), IMG_GetError());
        
        if (failureFlag != nullptr)
            *failureFlag = true;
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (texture == nullptr) {
            printf("SpriteSheet Error: Unable to create texture from %s: SDL Error: %s\n", filePath.c_str(), SDL_GetError());
            
            if (failureFlag != nullptr)
                *failureFlag = true;
        }
        SDL_FreeSurface(loadedSurface);
        
        if (SDL_QueryTexture(texture, nullptr, nullptr, &imageWidth, &imageHeight) < 0) {
            printf("SpriteSheet Error: %s\n", SDL_GetError());
        }
    }
}

SpriteSheet::~SpriteSheet() {
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

SDL_Texture * SpriteSheet::getTexture() const {
    return texture;
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

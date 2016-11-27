//
//  SpriteSheet.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 6/1/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef SpriteSheet_hpp
#define SpriteSheet_hpp

#include <string>

struct SDL_Texture;
struct SDL_Renderer;

namespace nautical {
    class SpriteSheet {
        friend class GraphicsManager;
        
    public:
        virtual ~SpriteSheet();
        
        int getImageWidth() const;
        int getImageHeight() const;
        int getWidthCount() const;
        int getHeightCount() const;
        float getScale() const;
        void setScale(float scale) const;
        
    private:
        SDL_Texture * p_texture = nullptr;
        
        int imageWidth = 0; //width of spritesheet in pixels
        int imageHeight = 0; //height of spritesheet in pixels
        int widthCount = 0; //horizontal frame count
        int heightCount = 0; //vertical frame count
        mutable float scale = 1; //pixel scale spritesheet should be drawn
        
        SpriteSheet(bool * failureFlag, SDL_Renderer * p_renderer, std::string filePath, int widthCount, int heightCount, float scale = 1); //private constructor so SpriteSheet must be created through GraphicsManager
        
        SDL_Texture * getTexture() const;
    };
}

#endif /* SpriteSheet_hpp */

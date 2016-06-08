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
        SDL_Texture * texture;
        
        int imageWidth = 0;
        int imageHeight = 0;
        int widthCount = 0;
        int heightCount = 0;
        mutable float scale = 1;
        
        SpriteSheet(bool * failureFlag, SDL_Renderer * renderer, std::string filePath, int widthCount, int heightCount, float scale = 1);
        
        SDL_Texture * getTexture() const;
    };
}

#endif /* SpriteSheet_hpp */

//
//  Image.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/12/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Image_hpp
#define Image_hpp

#include <string>

class SDL_Texture;
class SDL_Renderer;

namespace nautical {
    class Image {
        friend class GraphicsManager;
        
    public:
        virtual ~Image();
        
        int getWidth() const;
        int getHeight() const;
        int getScale() const;
        
    private:
        SDL_Texture * p_texture;
        
        int width = 0;
        int height = 0;
        int scale = 1;
        
        Image(bool * p_failureFlag, SDL_Renderer * p_renderer, std::string filePath, int scale = 1);
        Image(bool * p_failureFlag, SDL_Texture * p_texture, int scale = 1);
    };
}

#endif /* Image_hpp */

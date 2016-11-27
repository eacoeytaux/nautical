//
//  SaveFile.hpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/3/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef SaveFile_hpp
#define SaveFile_hpp

#include <string>

namespace nautical {
    class SaveFile {
    public:
        SaveFile(std::string fileName, std::string extensionName = "NTCLSAV");
        virtual ~SaveFile();
        
        std::string getFileName() const;
        SaveFile & setFileName(std::string fileName);
        std::string getExtensionName() const;
        SaveFile & setExtensionName(std::string extensionName);
        
        //slot indicates which slot to save to, -1 means no slot
        virtual SaveFile & save(int slot = -1) = 0;
        virtual SaveFile & load(int slot = -1) = 0;
        
    private:
        std::string fileName,
        extensionName;
    };
}

#endif /* SaveFile_hpp */

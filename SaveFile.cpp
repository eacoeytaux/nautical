//
//  SaveFile.cpp
//  nautical
//
//  Created by Ethan Coeytaux on 7/3/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#include "SaveFile.hpp"

using namespace nautical;

SaveFile::SaveFile(std::string fileName, std::string extensionName) :
fileName(fileName),
extensionName(extensionName) { }

SaveFile::~SaveFile() { }

std::string SaveFile::getFileName() const {
    return fileName;
}

SaveFile & SaveFile::setFileName(std::string fileName) {
    this->fileName = fileName;
    return *this;
}

std::string SaveFile::getExtensionName() const {
    return extensionName;
}

SaveFile & SaveFile::setExtensionName(std::string extensionName) {
    this->extensionName = extensionName;
    return *this;
}

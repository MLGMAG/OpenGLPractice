#pragma once
#include <string>

struct ImageInfo {
    int width;
    int height;
    int nrChannels;
    unsigned char *data;
};

unsigned int bindTextureRGB(const std::string &filePath);

unsigned int bindTextureRGBA(const std::string &filePath);

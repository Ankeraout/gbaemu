#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include <gbaemu/gba/gba.hpp>

using namespace std;

gbaemu::gba::GBA::GBA(const char *biosFilePath) {
    this->loadBIOS(biosFilePath);
}

void gbaemu::gba::GBA::loadBIOS(const char *biosFilePath) {
    this->bios = new uint8_t[biosFileSize];

    if(!this->bios) {
        throw runtime_error("Memory allocation failed.");
    }

    if(!filesystem::exists(biosFilePath)) {
        throw runtime_error("BIOS file does not exist.");
    }

    if(filesystem::file_size(biosFilePath) != biosFileSize) {
        throw runtime_error("Bad BIOS file size.");
    }

    ifstream biosFile(biosFilePath, ifstream::in | fstream::binary);

    if(!biosFile) {
        throw runtime_error("Failed to open BIOS file.");
    }

    for(size_t i = 0; i < biosFileSize; i++) {
        biosFile >> this->bios[i];
    }

    biosFile.close();
}

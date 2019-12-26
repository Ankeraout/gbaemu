#include <fstream>
#include <iostream>
#include <stdexcept>

#include <gbaemu/fs.hpp>
#include <gbaemu/gba/gba.hpp>

using namespace std;

gbaemu::gba::GBA::GBA(const char *biosFilePath, const char *romFilePath) {
    this->loadBIOS(biosFilePath);
    this->loadROM(romFilePath);
}

void gbaemu::gba::GBA::loadBIOS(const char *biosFilePath) {
    this->bios = new uint8_t[biosFileSize];

    if(!this->bios) {
        throw runtime_error("Memory allocation failed.");
    }

    if(!gbaemu::fileExists(biosFilePath)) {
        throw runtime_error("BIOS file does not exist.");
    }

    if(gbaemu::getFileSize(biosFilePath) != biosFileSize) {
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

void gbaemu::gba::GBA::loadROM(const char *romFilePath) {
    if(!gbaemu::fileExists(romFilePath)) {
        throw runtime_error("ROM file does not exist.");
    }

    size_t romFileSize(gbaemu::getFileSize(romFilePath));

    this->rom = new uint8_t[romFileSize];

    if(!this->rom) {
        throw runtime_error("Memory allocation failed.");
    }

    ifstream romFile(romFilePath, ifstream::in | fstream::binary);

    if(!romFile) {
        throw runtime_error("Failed to open ROM file.");
    }

    for(size_t i = 0; i < romFileSize; i++) {
        romFile >> this->rom[i];
    }

    romFile.close();
}

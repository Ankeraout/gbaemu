#include <fstream>
#include <iostream>
#include <stdexcept>

#include <gbaemu/fs.hpp>
#include <gbaemu/gba/cartridge.hpp>
#include <gbaemu/gba/gba.hpp>

using namespace std;

namespace gbaemu::gba {
    static inline void loadBIOS(const char *biosFilePath);

    uint8_t biosData[biosFileSize];

    void init(const char *biosFilePath, const char *romFilePath) {
        loadBIOS(biosFilePath);
        cartridge::init(romFilePath);
    }

    static inline void loadBIOS(const char *biosFilePath) {
        if(!biosData) {
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
            biosFile >> biosData[i];
        }

        biosFile.close();
    }
}

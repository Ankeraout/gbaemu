#include <fstream>
#include <iostream>
#include <stdexcept>

#include <gbaemu/fs.hpp>
#include <gbaemu/gba/cartridge.hpp>
#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/gba.hpp>
#include <gbaemu/gba/io.hpp>
#include <gbaemu/gba/keypad.hpp>
#include <gbaemu/gba/lcd.hpp>

using namespace std;

namespace gbaemu::gba {
    static inline void loadBIOS(const char *biosFilePath);

    uint8_t biosData[biosFileSize];
    uint8_t wramData_fast[wramFastSize];
    uint8_t wramData_slow[wramSlowSize];

    void init(const char *biosFilePath, const char *romFilePath) {
        loadBIOS(biosFilePath);
        cartridge::init(romFilePath);
        cpu::init();
        io::init();
        keypad::init();
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

        biosFile.read((char *)biosData, biosFileSize);

        if(biosFile.eof()) {
            throw runtime_error("EOF reached before the end of the BIOS file.");
        }

        if(biosFile.fail()) {
            throw runtime_error("an error occurred while reading BIOS.");
        }

        biosFile.close();
    }

    void cycle() {
        cpu::cycle();
        lcd::cycle();
    }
}

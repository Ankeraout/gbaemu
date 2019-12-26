#include <iostream>
#include <stdexcept>
#include <fstream>
#include <cstring>
#include <gbaemu/gbaemu.hpp>
#include <gbaemu/fs.hpp>
#include <gbaemu/gba/cartridge.hpp>

using namespace std;

gbaemu::gba::Cartridge::Cartridge(const char *romFilePath) {
    this->saveFilePath = NULL;
    this->save = NULL;
    this->rom = NULL;
    this->romSize = 0;
    this->saveSize = 0;
    this->saveType = NONE;

    if(!gbaemu::fileExists(romFilePath)) {
        throw runtime_error("ROM file does not exist.");
    }

    size_t romFileSize(gbaemu::getFileSize(romFilePath));

    this->rom = new uint8_t[romFileSize];

    if(!this->rom) {
        throw runtime_error("memory allocation failed.");
    }

    ifstream romFile(romFilePath, ifstream::in | fstream::binary);

    if(!romFile) {
        throw runtime_error("failed to open ROM file.");
    }

    romFile.read((char *)this->rom, romFileSize);

    if(romFile.eof()) {
        throw runtime_error("EOF reached before the end of the file.");
    }

    if(romFile.fail()) {
        throw runtime_error("an error occurred while reading ROM.");
    }

    romFile.close();

    this->romSize = romFileSize;

    if(gbaemu::conf.forcedSaveType) {
        this->readForcedSaveType();
    } else {
        this->detectSaveType();
    }

    this->allocateSave();
}

void gbaemu::gba::Cartridge::loadSave(const char *saveFilePath) {
    // Temporary code
    UNUSED_PARAMETER(saveFilePath);
}

void gbaemu::gba::Cartridge::saveSave() {

}

void gbaemu::gba::Cartridge::saveSave(const char *saveFilePath) {
    // Temporary code
    UNUSED_PARAMETER(saveFilePath);
}

void gbaemu::gba::Cartridge::detectSaveType() {
    const char *saveStringTable[] = {
        "SRAM_V",
        "FLASH_V",
        "FLASH512_V",
        "FLASH1M_V"
    };
    
    const SaveType saveTypeTable[] = {
        SRAM_256KB,
        FLASH_512KB,
        FLASH_512KB,
        FLASH_1MB
    };

    const char *saveStringTableMessage[] = {
        "SRAM 32 Kbytes (256Kbit)",
        "FLASH 64 Kbytes (512Kbit) (old ROM)",
        "FLASH 64 Kbytes (512Kbit) (new ROM)",
        "FLASH 128 Kbytes (1Mbit)"
    };

    int detectedSaveType = -1;
    bool multipleSaveTypes = false;
    
    for(int i = 0; i < 4; i++) {
        size_t len = strlen(saveStringTable[i]);
        size_t jMax = this->romSize - len;

        for(size_t j = 0; j < jMax; j += 4) {
            if(strncmp(saveStringTable[i], (const char *)&this->rom[j], len) == 0) {
                cout << "Detected save type: " << saveStringTableMessage[i] << endl;

                if(detectedSaveType != -1) {
                    multipleSaveTypes = true;
                }

                detectedSaveType = i;

                break;
            }
        }
    }

    if(multipleSaveTypes) {
        throw runtime_error("multiple save type signatures have been found.");
    }

    if(detectedSaveType == -1) {
        cout << "Save type was not detected, defaulting to none." << endl;
        this->saveType = NONE;
    } else {
        this->saveType = saveTypeTable[detectedSaveType];
    }
}


void gbaemu::gba::Cartridge::readForcedSaveType() {
    const char *forcedSaveTypeTable[] = {
        "none",
        "eeprom4kb",
        "eeprom64kb",
        "sram256kb",
        "flash512kb_old",
        "flash512kb_new",
        "flash512kb",
        "flash1mb"
    };

    const SaveType saveTypeTable[] = {
        NONE,
        EEPROM_4KB,
        EEPROM_64KB,
        SRAM_256KB,
        FLASH_512KB,
        FLASH_512KB,
        FLASH_512KB,
        FLASH_1MB
    };

    bool found = false;

    for(int i = 0; i < 8; i++) {
        if(strcmp(gbaemu::conf.forcedSaveType, forcedSaveTypeTable[i]) == 0) {
            found = true;
            this->saveType = saveTypeTable[i];
            break;
        }
    }

    if(!found) {
        throw runtime_error(string("Unknown save type '") + gbaemu::conf.forcedSaveType + string("'."));
    }
}

void gbaemu::gba::Cartridge::allocateSave() {
    const size_t saveSizeTable[] = {
        0,
        512,
        8192,
        32768,
        65536,
        131072
    };

    size_t saveSize = saveSizeTable[this->saveType];

    if(saveSize > 0) {
        this->save = new uint8_t[saveSize];
    }
}

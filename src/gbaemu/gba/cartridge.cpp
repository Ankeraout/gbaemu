#include <iostream>
#include <stdexcept>
#include <fstream>
#include <cstring>
#include <gbaemu/gbaemu.hpp>
#include <gbaemu/fs.hpp>
#include <gbaemu/gba/cartridge.hpp>

using namespace std;

namespace gbaemu::gba::cartridge {
    const char *saveFilePath;
    uint8_t *saveData;
    uint8_t *romData;
    size_t romSize;
    size_t saveSize;
    SaveType saveType;
    uint32_t romAddressMask;

    void detectSaveType();
    void readForcedSaveType();
    void allocateSave();

    void init(const char *romFilePath) {
        saveFilePath = NULL;
        saveData = NULL;
        romData = NULL;
        romSize = 0;
        saveSize = 0;
        saveType = NONE;

        if(!gbaemu::fileExists(romFilePath)) {
            throw runtime_error("ROM file does not exist.");
        }

        size_t romFileSize(gbaemu::getFileSize(romFilePath));

        romData = new uint8_t[romFileSize];

        if(!romData) {
            throw runtime_error("memory allocation failed.");
        }

        ifstream romFile(romFilePath, ifstream::in | fstream::binary);

        if(!romFile) {
            throw runtime_error("failed to open ROM file.");
        }

        romFile.read((char *)romData, romFileSize);

        if(romFile.eof()) {
            throw runtime_error("EOF reached before the end of the file.");
        }

        if(romFile.fail()) {
            throw runtime_error("an error occurred while reading ROM.");
        }

        romFile.close();

        romSize = romFileSize;
        romAddressMask = romSize - 1;

        if(gbaemu::conf.forcedSaveType) {
            readForcedSaveType();
        } else {
            detectSaveType();
        }

        allocateSave();
    }

    void loadSave(const char *saveFilePath) {
        // Temporary code
        UNUSED(saveFilePath);
    }

    void saveSave() {

    }

    void saveSave(const char *saveFilePath) {
        // Temporary code
        UNUSED(saveFilePath);
    }

    void detectSaveType() {
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
            size_t jMax = romSize - len;

            for(size_t j = 0; j < jMax; j += 4) {
                if(strncmp(saveStringTable[i], (const char *)&romData[j], len) == 0) {
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
            saveType = NONE;
        } else {
            saveType = saveTypeTable[detectedSaveType];
        }
    }


    void readForcedSaveType() {
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
                saveType = saveTypeTable[i];
                break;
            }
        }

        if(!found) {
            throw runtime_error(string("Unknown save type '") + gbaemu::conf.forcedSaveType + string("'."));
        }
    }

    void allocateSave() {
        const size_t saveSizeTable[] = {
            0,
            512,
            8192,
            32768,
            65536,
            131072
        };

        size_t saveSize = saveSizeTable[saveType];

        if(saveSize > 0) {
            saveData = new uint8_t[saveSize];
        }
    }
}

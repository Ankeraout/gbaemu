#pragma once

#include <cstdint>

namespace gbaemu::gba {
    typedef enum {
        NONE,
        EEPROM_4KB,
        EEPROM_64KB,
        SRAM_256KB,
        FLASH_512KB,
        FLASH_1MB
    } SaveType;

    class Cartridge {
        private:
        const char *saveFilePath;
        uint8_t *save;
        uint8_t *rom;
        size_t romSize;
        size_t saveSize;
        SaveType saveType;

        void detectSaveType();
        void readForcedSaveType();
        void allocateSave();

        public:
        Cartridge(const char *romFilePath);
        void loadSave(const char *saveFilePath);
        void saveSave();
        void saveSave(const char *saveFilePath);
    };
}

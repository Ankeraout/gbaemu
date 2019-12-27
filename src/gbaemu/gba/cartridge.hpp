#pragma once

#include <cstdint>

namespace gbaemu::gba::cartridge {
    typedef enum {
        NONE,
        EEPROM_4KB,
        EEPROM_64KB,
        SRAM_256KB,
        FLASH_512KB,
        FLASH_1MB
    } SaveType;

    extern void init(const char *romFilePath);
    extern void loadSave(const char *saveFilePath);
    extern void saveSave();
    extern void saveSave(const char *saveFilePath);
}

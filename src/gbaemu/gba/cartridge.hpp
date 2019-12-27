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

    void init(const char *romFilePath);
    void loadSave(const char *saveFilePath);
    void saveSave();
    void saveSave(const char *saveFilePath);
}

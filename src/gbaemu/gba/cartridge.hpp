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

    extern const char *saveFilePath;
    extern uint8_t *saveData;
    extern uint8_t *romData;
    extern size_t romSize;
    extern size_t saveSize;
    extern SaveType saveType;
    extern uint32_t romAddressMask;
    extern uint32_t sramAddressMask;

    extern void init(const char *romFilePath);
    extern void loadSave(const char *saveFilePath);
    extern void saveSave();
    extern void saveSave(const char *saveFilePath);
    extern uint32_t read32_rom(uint32_t address);
}

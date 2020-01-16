#include <cstring>
#include <iostream>

#include <gbaemu/gbaemu.hpp>
#include <gbaemu/gba/cpu.hpp>
#include <gbaemu/gba/gba.hpp>

#include <gbaemu/frontend.hpp>

using namespace std;

namespace gbaemu {
    conf_t conf;

    int _main(int argc, const char *argv[]);
    void resetConfiguration();
    int readCommandLineArguments(int argc, const char *argv[]);
    int checkConfiguration();
}

int main(int argc, const char *argv[]) {
    try {
        gbaemu::_main(argc, argv);
    } catch(runtime_error &e) {
        cerr << "Error: " << e.what() << endl;
    }
}

namespace gbaemu {
    int _main(int argc, const char *argv[]) {
        resetConfiguration();

        if(readCommandLineArguments(argc, argv)) {
            cout << "GBAEmu help" << endl;
            cout << "===========" << endl;
            cout << endl;
            cout << "Required command-line options:" << endl;
            cout << "  --bios <bios_file_name>       : Specifies the BIOS file to use." << endl;
            cout << "  --rom <rom_file_name>         : Specifies the ROM file to use." << endl;
            cout << endl;
            cout << "Available command-line options:" << endl;
            cout << "  --help                        : Shows this message." << endl;
            cout << "  --force-save-type <save_type> : Specifies the save type of the cartridge." << endl;

            return 0;
        }

        if(checkConfiguration()) {
            cerr << "Error: the application configuration is incorrect." << endl;
            return EXIT_FAILURE;
        }

        // Init front-end
        if(gbaemu::frontend::init()) {
            cerr << "Error: front-end initialization failed." << endl;
            return EXIT_FAILURE;
        }

        gbaemu::gba::init(gbaemu::conf.biosPath, gbaemu::conf.romPath);

        // Jump directly to ROM
        gbaemu::gba::cpu::registerWrite(15, 0x08000000);
        gbaemu::gba::cpu::cpsr.fields.mode = gbaemu::gba::cpu::PSR_MODE_USR;

        while(true) {
            gbaemu::gba::cycle();
        }

        gbaemu::gba::cpu::displayState();

        return 0;
    }

    void resetConfiguration() {
        memset(&gbaemu::conf, 0, sizeof(gbaemu::conf));
    }

    int readCommandLineArguments(int argc, const char *argv[]) {
        bool flag_bios = false;
        bool flag_rom = false;

        for(int i = 1; i < argc; i++) {
            if(flag_bios) {
                if(gbaemu::conf.biosPath) {
                    throw runtime_error("too many BIOS files.");
                } else {
                    gbaemu::conf.biosPath = argv[i];
                    flag_bios = false;
                }
            } else if(flag_rom) {
                if(gbaemu::conf.romPath) {
                    throw runtime_error("too many ROM files.");
                } else {
                    gbaemu::conf.romPath = argv[i];
                    flag_rom = false;
                }
            } else if(strcmp(argv[i], "--bios") == 0) {
                flag_bios = true;
            } else if(strcmp(argv[i], "--rom") == 0) {
                flag_rom = true;
            } else if(strcmp(argv[i], "--help") == 0) {
                return 1;
            } else {
                throw runtime_error(string("invalid argument: '") + argv[i] + string("."));
            }
        }

        if(flag_bios) {
            throw runtime_error("expected file path after \"--bios\".");
        } else if(flag_rom) {
            throw runtime_error("expected file path after \"--rom\".");
        }

        return 0;
    }

    int checkConfiguration() {
        if(!gbaemu::conf.biosPath) {
            cerr << "Error: no BIOS file path specified." << endl;
            return 1;
        }

        return 0;
    }
}

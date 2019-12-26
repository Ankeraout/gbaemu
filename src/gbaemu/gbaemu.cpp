#include <cstring>
#include <iostream>

#include <gbaemu/gbaemu.hpp>
#include <gbaemu/gba/gba.hpp>

using namespace std;

int _main(int argc, const char *argv[]);
void resetConfiguration();
int readCommandLineArguments(int argc, const char *argv[]);
int checkConfiguration();

int main(int argc, const char *argv[]) {
    try {
        _main(argc, argv);
    } catch(runtime_error &e) {
        cerr << "Error: " << e.what() << endl;
    }
}

int _main(int argc, const char *argv[]) {
    resetConfiguration();

    if(readCommandLineArguments(argc, argv)) {
        cerr << "Error: could not read the command-line arguments." << endl;
        return EXIT_FAILURE;
    }

    if(checkConfiguration()) {
        cerr << "Error: the application configuration is incorrect." << endl;
        return EXIT_FAILURE;
    }

    gbaemu::gba::GBA(gbaemu::conf.biosPath, gbaemu::conf.romPath);

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

namespace gbaemu {
    conf_t conf;
}

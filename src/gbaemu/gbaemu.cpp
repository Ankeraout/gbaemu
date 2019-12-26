#include <cstring>
#include <iostream>

#include <gbaemu/gbaemu.hpp>
#include <gbaemu/gba/gba.hpp>

using namespace std;

void resetConfiguration();
int readCommandLineArguments(int argc, const char *argv[]);
int checkConfiguration();

int main(int argc, const char *argv[]) {
    resetConfiguration();

    if(readCommandLineArguments(argc, argv)) {
        cerr << "Error: could not read the command-line arguments." << endl;
        return EXIT_FAILURE;
    }

    if(checkConfiguration()) {
        cerr << "Error: the application configuration is incorrect." << endl;
        return EXIT_FAILURE;
    }

    gbaemu::gba::GBA(gbaemu::conf.biosPath);

    return 0;
}

void resetConfiguration() {
    memset(&gbaemu::conf, 0, sizeof(gbaemu::conf));
}

int readCommandLineArguments(int argc, const char *argv[]) {
    bool flag_bios = false;

    for(int i = 1; i < argc; i++) {
        if(flag_bios) {
            if(gbaemu::conf.biosPath) {
                cerr << "Error: too many configuration files." << endl;
                return 1;
            } else {
                gbaemu::conf.biosPath = argv[i];
                flag_bios = false;
            }
        } else if(strcmp(argv[i], "--bios") == 0) {
            flag_bios = true;
        }
    }

    if(flag_bios) {
        cerr << "Error: expected file path after \"--bios\"." << endl;
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

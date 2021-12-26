//==============================================================================
// Included files
//==============================================================================
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//==============================================================================
// Private constants
//==============================================================================
/**
 * @brief This constant defines the maximum size of the BIOS file in bytes.
 */
#define C_MAX_BIOS_FILE_SIZE_BYTES 16384

/**
 * @brief This constant defines the maximum size of the ROM file in bytes.
 */
#define C_MAX_ROM_FILE_SIZE_BYTES 33554432

/**
 * @brief This constant defines the maximum size of the save file in bytes.
 */
#define C_MAX_SRAM_FILE_SIZE_BYTES 1048576

//==============================================================================
// Public variables
//==============================================================================
/**
 * @brief This variable contains a pointer to an array that contains the BIOS
 *        ROM data.
 */
uint8_t *g_biosBuffer = NULL;

/**
 * @brief This variable contains a pointer to an array that contains the ROM
 *        data.
 */
uint8_t *g_romBuffer = NULL;

/**
 * @brief This variable contains the size of the ROM buffer.
 */
long g_romBufferSize;

/**
 * @brief This variable contains a pointer to an array that contains the SRAM
 *        data.
 */
uint8_t *g_sramBuffer = NULL;

/**
 * @brief This variable contains the size of the SRAM buffer.
 */
long g_sramBufferSize;

//==============================================================================
// Private variables
//==============================================================================
/**
 * @brief This variable contains the path to the BIOS file.
 */
const char *s_biosFile = NULL;

/**
 * @brief This variable contains the path to the ROM file.
 */
const char *s_romFile = NULL;

/**
 * @brief This variable contains the path to the SRAM file.
 */
const char *s_sramFile = NULL;

//==============================================================================
// Private functions declaration
//==============================================================================
/**
 * @brief Returns a boolean value that indicates whether the current parameter
 *        is equal to the given parameter in its short or long name version.
 *
 * @param[in] p_currentParameter The parameter to compare (read from argv).
 * @param[in] p_longParameterName The long name of the parameter compared (for
 *                                example "rom-file" for "--rom-file"). Case-
 *                                insensitive.
 * @param[in] p_shortParameterName The short name of the parameter compared (for
 *                                 example "R" for "-R"). Case-sensitive.
 *
 * @returns A boolean value that indicates whether the current parameter is
 *          equal to the given parameter in its short or long name version.
 * @retval true if the parameter is equal.
 * @retval false if the parameter is not equal.
 */
static bool parameterEquals(const char *p_currentParameter,
                            const char *p_longParameterName,
                            const char *p_shortParameterName);

/**
 * @brief Reads the command-line parameters.
 *
 * @param[in] p_argc The number of parameters.
 * @param[in] p_argv The array of parameters.
 *
 * @returns An integer that indicates the result of the operation.
 * @retval 0 if no error occurred.
 * @retval Any other value if an error occurred.
 */
static int readParameters(int p_argc, const char *p_argv[]);

/**
 * @brief Loads the ROM file.
 *
 * @returns An integer that indicates the result of the operation.
 * @retval 0 if no error occurred.
 * @retval Any other value if an error occurred.
 */
static int loadRom();

/**
 * @brief Loads the BIOS file.
 *
 * @returns An integer that indicates the result of the operation.
 * @retval 0 if no error occurred.
 * @retval Any other value if an error occurred.
 */
static int loadBios();

/**
 * @brief Loads the SRAM file.
 *
 * @returns An integer that indicates the result of the operation.
 * @retval 0 if no error occurred.
 * @retval Any other value if an error occurred.
 */
static int loadSram();

/**
 * @brief Saves the SRAM file.
 *
 * @returns An integer that indicates the result of the operation.
 * @retval 0 if no error occurred.
 * @retval Any other value if an error occurred.
 */
static int saveSram();

/**
 * @brief Rounds the given integer to the higher power of two.
 *
 * @param[in] p_initialValue The number to round.
 *
 * @returns The given integer rounded to the higher power of two.
 */
static long po2_ceil(long p_initialValue);

/**
 * @brief Reads the given file.
 *
 * @param[in] p_fileName The path to the file to read.
 * @param[in,out] p_fileSize The size of the file to read in bytes. The input
 *                           value is the maximum allowed size, the output
 *                           value is the actual size.
 * @param[in] p_po2 A boolean value that indicates whether or not to round the
 *                  size of the buffer to the higher power of two.
 *
 * @returns A pointer to the buffer that contains the data of the file, or NULL
 *          if an error occurred.
 */
static void *readFile(const char *p_fileName, long *p_fileSize, bool p_po2);

/**
 * @brief Writes the given buffer to the given file.
 *
 * @param[in] p_fileName The path to the file to write.
 * @param[in] p_buffer The buffer to write to the file.
 * @param[in] p_bufferSize The size in bytes of the buffer to write.
 *
 * @returns An integer that indicates the result of the operation.
 * @retval 0 if no error occurred.
 * @retval Any other value if an error occurred.
 */
static int writeFile(const char *p_fileName,
              const void *p_buffer,
              size_t p_bufferSize);

//==============================================================================
// Public functions definition
//==============================================================================
int emulatorInit(int p_argc, const char *p_argv[]) {
    bool l_error = false;

    if(readParameters(p_argc, p_argv)
        || loadRom()
        || loadBios()
        || loadSram()
    ) {
        l_error = true;
    }

    if(l_error) {
        return 1;
    } else {
        return 0;
    }
}

void emulatorExecute(void) {

}

//==============================================================================
// Private functions definition
//==============================================================================
static bool parameterEquals(const char *p_currentParameter,
                            const char *p_longParameterName,
                            const char *p_shortParameterName) {
    int l_longParameterNameLength = strlen(p_longParameterName);
    int l_shortParameterNameLength = strlen(p_shortParameterName);

    char l_longParameterName[l_longParameterNameLength + 3];
    char l_shortParameterName[l_shortParameterNameLength + 2];

    sprintf(l_longParameterName, "--%s", p_longParameterName);
    sprintf(l_shortParameterName, "-%s", p_shortParameterName);

    bool l_longParameterCorresponds = strcasecmp(l_longParameterName,
                                                 p_currentParameter) == 0;
    bool l_shortParameterCorresponds = strcmp(l_shortParameterName,
                                              p_shortParameterName) == 0;

    return l_longParameterCorresponds || l_shortParameterCorresponds;
}

static int readParameters(int p_argc, const char *p_argv[]) {
    int l_parameterIndex = 1;
    bool l_error = false;
    bool l_expectBiosFile = false;
    bool l_expectRomFile = false;

    while((l_parameterIndex < p_argc) && !l_error) {
        const char *l_currentParameter = p_argv[l_parameterIndex];

        if(l_expectBiosFile) {
            s_biosFile = l_currentParameter;
            l_expectBiosFile = false;
        } else if(l_expectRomFile) {
            s_romFile = l_currentParameter;
            l_expectRomFile = false;
        } else if(parameterEquals(l_currentParameter, "bios", "B")) {
            l_expectBiosFile = true;
        } else if(parameterEquals(l_currentParameter, "rom", "R")) {
            l_expectRomFile = true;
        } else {
            fprintf(stderr, "Unknown parameter: \"%s\".\n", l_currentParameter);
            l_error = true;
        }
    }

    if(l_expectBiosFile) {
        fprintf(stderr, "Expected argument after \"--bios\".\n");
        l_error = true;
    } else if(l_expectRomFile) {
        fprintf(stderr, "Expected argument after \"--rom\".\n");
        l_error = true;
    } else if(s_biosFile == NULL) {
        fprintf(stderr, "No BIOS file name specified.\n");
        l_error = true;
    } else if(s_romFile == NULL) {
        fprintf(stderr, "No ROM file name specified.\n");
        l_error = true;
    }

    if(l_error) {
        return 1;
    } else {
        return 0;
    }
}

static int loadRom() {
    g_romBufferSize = C_MAX_ROM_FILE_SIZE_BYTES;

    g_romBuffer = readFile(s_romFile, &g_romBufferSize, true);

    if(g_romBuffer == NULL) {
        fprintf(stderr, "Failed to read ROM file.\n");
        return 1;
    }

    return 0;
}

static int loadBios() {
    long l_fileSize = C_MAX_BIOS_FILE_SIZE_BYTES;

    g_biosBuffer = readFile(s_biosFile, &l_fileSize, true);

    if(g_biosBuffer == NULL) {
        fprintf(stderr, "Failed to read BIOS file.\n");
        return 1;
    }

    if(l_fileSize != C_MAX_BIOS_FILE_SIZE_BYTES) {
        fprintf(stderr, "Wrong BIOS file size.\n");
        return 1;
    }

    return 0;
}

static int loadSram() {
    g_sramBufferSize = C_MAX_SRAM_FILE_SIZE_BYTES;

    g_sramBuffer = readFile(s_sramFile, &g_sramBufferSize, true);

    if(g_sramBuffer == NULL) {
        fprintf(stderr, "Failed to read SRAM file.\n");
        return 1;
    }

    return 0;
}

static int saveSram() {
    return writeFile(s_sramFile, g_sramBuffer, g_sramBufferSize);
}

static long po2_ceil(long p_initialValue) {
    int l_hammingWeight = 0;
    int l_lastBitTo1 = 0;

    for(unsigned long i = 0; i < sizeof(long); i++) {
        for(unsigned long j = 0; j < 8; j++) {
            unsigned long shift = i * 8 + j;
            int bit = (p_initialValue >> shift) & 1;

            if(bit) {
                l_lastBitTo1 = shift;
                l_hammingWeight++;
            }
        }
    }

    if(l_hammingWeight == 1) {
        return p_initialValue;
    } else {
        return 1 << (l_lastBitTo1 + 1);
    }
}

static void *readFile(const char *p_fileName, long *p_fileSize, bool p_po2) {
    // Open the file as read-binary
    FILE *l_file = fopen(p_fileName, "rb");

    // If the file could not be opened, return an error
    if(!l_file) {
        fprintf(stderr, "readFile(): Failed to open %s.", p_fileName);
        return NULL;
    }

    // Determine the file size
    fseek(l_file, 0, SEEK_END);
    long l_foundFileSize = ftell(l_file);
    fseek(l_file, 0, SEEK_SET);

    // Compare the file size
    if(*p_fileSize > 0 && l_foundFileSize > *p_fileSize) {
        fprintf(stderr, "readFile(): File is too large: %s.", p_fileName);
        return NULL;
    }

    long l_bufferSize = p_po2 ? po2_ceil(l_foundFileSize) : l_foundFileSize;

    // Allocate the buffer
    void *l_buffer = calloc(l_bufferSize, 1);

    if(!l_buffer) {
        fprintf(stderr, "readFile(): Failed to allocate buffer.\n");
        return NULL;
    }

    // Read the file
    size_t l_readBytes = fread(l_buffer, 1, l_foundFileSize, l_file);

    if(l_readBytes != (size_t)l_foundFileSize) {
        free(l_buffer);
        fprintf(
            stderr,
            "readFile(): Failed to read %ld bytes.\n",
            l_foundFileSize
        );
        return NULL;
    }

    *p_fileSize = l_bufferSize;

    return l_buffer;
}

static int writeFile(const char *p_fileName,
                     const void *p_buffer,
                     size_t p_bufferSize) {
    // Open the file as read-binary
    FILE *l_file = fopen(p_fileName, "rb");

    // If the file could not be opened, return an error
    if(!l_file) {
        fprintf(stderr, "writeFile(): Failed to open %s.", p_fileName);
        return 1;
    }

    // Write the data to the file
    size_t l_writtenBytes = fwrite(p_buffer, 1, p_bufferSize, l_file);

    // Close the file
    fclose(l_file);

    // If the file could not be written, return an error
    if(l_writtenBytes != p_bufferSize) {
        fprintf(
            stderr,
            "writeFile(): Failed to write %llu bytes in %s.",
            p_bufferSize,
            p_fileName
        );

        return 1;
    }

    return 0;
}
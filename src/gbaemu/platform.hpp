// Detect Unix
#if defined(unix)  || defined(__unix__) || defined(__unix)
#   define DETECTED_OS_UNIX
#endif

// Detect Windows
#if defined(WIN32) || defined(_WIN32)
#   define DETECTED_OS_WINDOWS
#endif

// Check if at least 1 OS has been detected
#if !defined(DETECTED_OS_UNIX) && !defined(DETECTED_OS_WINDOWS)
#   error Unsupported compilation target.
#endif

// Check if only one OS has been detected
#if defined(DETECTED_OS_UNIX) && defined(DETECTED_OS_WINDOWS)
#   error OS detection failed: Unix and Windows were detected.
#else
#   if defined(DETECTED_OS_UNIX)
#       define GBAEMU_OS_UNIX
#   elif defined(DETECTED_OS_WINDOWS)
#       define GBAEMU_OS_WINDOWS
#   endif
#endif

// Undefine detected OS macros
#undef DETECTED_OS_UNIX
#undef DETECTED_OS_WINDOWS

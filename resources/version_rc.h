#ifndef APP_VERSION_RC_H
#define APP_VERSION_RC_H

// Auto-generate version from CMakeLists.txt PROJECT_VERSION
// CMake will define these via command line: -DAPP_VERSION_MAJOR=0 -DAPP_VERSION_MINOR=0 -DAPP_VERSION_PATCH=1

#ifndef APP_VERSION_MAJOR
#define APP_VERSION_MAJOR 0
#endif

#ifndef APP_VERSION_MINOR
#define APP_VERSION_MINOR 0
#endif

#ifndef APP_VERSION_PATCH
#define APP_VERSION_PATCH 1
#endif

// Helper macros to convert version to comma-separated and string format
#define VER_FILEVERSION             APP_VERSION_MAJOR,APP_VERSION_MINOR,APP_VERSION_PATCH,0
#define VER_PRODUCTVERSION          APP_VERSION_MAJOR,APP_VERSION_MINOR,APP_VERSION_PATCH,0

// Stringify helper macros
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define VER_FILEVERSION_STR         TOSTRING(APP_VERSION_MAJOR) "." TOSTRING(APP_VERSION_MINOR) "." TOSTRING(APP_VERSION_PATCH) ".0"
#define VER_PRODUCTVERSION_STR      TOSTRING(APP_VERSION_MAJOR) "." TOSTRING(APP_VERSION_MINOR) "." TOSTRING(APP_VERSION_PATCH)

#define VER_COMPANYNAME_STR         "TanmoyTheBoT"
#define VER_FILEDESCRIPTION_STR     "TOTP Manager - Secure 2FA Authentication"
#define VER_INTERNALNAME_STR        "TOTPManager"
#define VER_LEGALCOPYRIGHT_STR      "Copyright (C) 2026 TanmoyTheBoT"
#define VER_ORIGINALFILENAME_STR    "TOTPManager.exe"
#define VER_PRODUCTNAME_STR         "TOTP Manager"

#endif // APP_VERSION_RC_H

cmake_minimum_required(VERSION 3.20)

project(
    TOTPManager
    VERSION 0.0.1
    LANGUAGES CXX C
)

# =====================================
# C++ Standard
# =====================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

# =====================================
# Qt
# =====================================

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Network
)

# =====================================
# Executable
# =====================================

qt_add_executable(${PROJECT_NAME})

# =====================================
# Resources
# =====================================

qt_add_resources(${PROJECT_NAME} "app_resources"
    PREFIX "/"
    BASE "resources"
    FILES
        resources/icons/app.png
)

# =====================================
# Source Files
# =====================================

target_sources(${PROJECT_NAME}
PRIVATE

# Application
app/main.cpp

# Resources
resources/app.rc

# ================= Version =================

src/version.h

# ================= UI =================

src/ui/MainWindow.cpp
src/ui/MainWindow.h

# Dialogs
src/ui/dialogs/AddAccountDialog.cpp
src/ui/dialogs/AddAccountDialog.h
src/ui/dialogs/ShowQRDialog.cpp
src/ui/dialogs/ShowQRDialog.h
src/ui/dialogs/SettingsDialog.cpp
src/ui/dialogs/SettingsDialog.h
src/ui/dialogs/DeleteConfirmDialog.cpp
src/ui/dialogs/DeleteConfirmDialog.h
src/ui/dialogs/ImportExportDialog.cpp
src/ui/dialogs/ImportExportDialog.h
src/ui/dialogs/AboutDialog.cpp
src/ui/dialogs/AboutDialog.h

# Widgets
src/ui/widgets/AccountCard.h
src/ui/widgets/AccountCard.cpp
src/ui/widgets/AccountCard_icons.cpp

# Styles
src/ui/styles/darktheme.h

# ================= Core =================

src/core/base32.cpp
src/core/base32.h

src/core/sha1.cpp
src/core/sha1.h

src/core/hmac.cpp
src/core/hmac.h

src/core/hotp.cpp
src/core/hotp.h

src/core/totp.cpp
src/core/totp.h

# ================= Services =================

src/services/otp_uri.cpp
src/services/otp_uri.h

# ================= QR =================

src/qr/qrcode.cpp
src/qr/qrcode.h

src/qr/qrcodegen.cpp
src/qr/qrcodegen.hpp

# ================= Database =================

src/database/database.cpp
src/database/database.h

src/database/sqlite3.c
src/database/sqlite3.h

# ================= Models =================

src/models/account.h

# ================= Utils =================

src/utils/timer.cpp
src/utils/timer.h
)

# =====================================
# Include Directories
# =====================================

target_include_directories(${PROJECT_NAME}
PRIVATE

app

src

src/ui
src/core
src/services
src/database
src/models
src/utils
src/qr

third_party/stb
)

# =====================================
# Link Libraries
# =====================================

target_link_libraries(${PROJECT_NAME}
PRIVATE
Qt6::Core
Qt6::Gui
Qt6::Widgets
Qt6::Network
-fstack-protector
)

# =====================================
# Windows
# =====================================

if(WIN32)
    set_target_properties(${PROJECT_NAME}
        PROPERTIES
        WIN32_EXECUTABLE TRUE
    )

    # Pass version to resource file
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        APP_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        APP_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        APP_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    )
endif()

# =====================================
# Output
# =====================================

# Detect architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH "win64")
else()
    set(ARCH "win32")
endif()

# Professional executable name with version and architecture
set_target_properties(${PROJECT_NAME}
PROPERTIES
    OUTPUT_NAME "TOTPManager-v${PROJECT_VERSION}-${ARCH}"
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)

# =====================================
# Build Message
# =====================================

message(STATUS "Building ${PROJECT_NAME}")
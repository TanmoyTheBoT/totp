cmake_minimum_required(VERSION 3.20)

project(
    TOTPManager
    VERSION 1.0
    LANGUAGES CXX C
)

# =====================================
# C++ Standard
# =====================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
)

# =====================================
# Executable
# =====================================

qt_add_executable(${PROJECT_NAME})

# =====================================
# Source Files
# =====================================

target_sources(${PROJECT_NAME}
PRIVATE

# Application
app/main.cpp

# ================= UI =================

src/ui/MainWindow.cpp
src/ui/MainWindow.h
src/ui/dialogs/AddAccountDialog.cpp
src/ui/dialogs/AddAccountDialog.h

# Future UI
# src/ui/AboutDialog.cpp
# src/ui/AboutDialog.h
# src/ui/QRDialog.cpp
# src/ui/QRDialog.h

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
-fstack-protector
)

# =====================================
# Windows
# =====================================

if(WIN32)
    set_target_properties(${PROJECT_NAME}
        PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()

# =====================================
# Output
# =====================================

set_target_properties(${PROJECT_NAME}
PROPERTIES
RUNTIME_OUTPUT_DIRECTORY
${PROJECT_SOURCE_DIR}/bin
)

# =====================================
# Build Message
# =====================================

message(STATUS "Building ${PROJECT_NAME}")
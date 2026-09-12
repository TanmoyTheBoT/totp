#ifndef VERSION_H
#define VERSION_H

#include <QString>

namespace Version {
    constexpr const char* APP_NAME = "TOTP Manager";
    constexpr const char* VERSION = "0.0.1";
    constexpr const char* BUILD_DATE = __DATE__;
    constexpr const char* DEVELOPER = "TanmoyTheBoT";
    constexpr const char* LICENSE = "MIT License";
    constexpr const char* WEBSITE = "https://github.com/tanmoythebot/totp-manager";
    constexpr const char* DESCRIPTION = "Secure 2FA for a safer tomorrow.";

    inline QString getVersionString() {
        return QString("v%1").arg(VERSION);
    }

    inline QString getFullVersionString() {
        return QString("%1 %2").arg(APP_NAME, getVersionString());
    }

    inline QString getBuildInfo() {
        return QString("Built on %1").arg(BUILD_DATE);
    }
}

#endif // VERSION_H

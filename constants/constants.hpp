#pragma once  // Header guard see https://en.wikipedia.org/wiki/Include_guard

// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// Qt headers
#include <QSysInfo>
#include <QSize>

// C++ headers
#include <filesystem>
#include <QDir>
#include <string>

// project headers
#include "config/config.hpp"

namespace srilakshmikanthanp::pulldog::constants {
/**
 * @brief Get the Application Version
 * @return const char*
 */
const char* getAppVersion();

/**
 * @brief Get the App UUID
 */
const char* getAppUUID();

/**
 * @brief Get the Application Name
 * @return const char*
 */
const char* getAppName();

/**
 * @brief Get logo path
 * @return const char*
 */
const char* getAppLogo();

/**
 * @brief Get the Application QSS File
 * @return const char*
 */
const char* getAppQSSLight();

/**
 * @brief Get the Application QSS File
 * @return const char*
 */
const char* getAppQSSDark();

/**
 * @brief Get App Home Directory
 * @return const char*
 */
std::string getAppHome();

/**
 * @brief Get App Log File
 */
std::string getAppLogFile();

/**
 * @brief Get the App Home Page
 *
 * @return const char*
 */
const char* getAppHomePage();

/**
 * @brief Get the App Issue Page
 *
 * @return const char*
 */
const char* getAppIssuePage();

/**
 * @brief Get the App Donate Page
 * @return const char*
 */
const char* getAppDonatePage();

/**
 * @brief Get the App Org Name object
 *
 * @return const char*
 */
const char* getAppOrgName();
}  // namespace srilakshmikanthanp::pulldog::config

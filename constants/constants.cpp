// Copyright (c) 2023 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "constants.hpp"

namespace srilakshmikanthanp::pulldog::constants {
/**
 * @brief Get the Application Version
 * @return const char*
 */
const char* getAppVersion() {
  return PULLDOG_VERSION;
}

/**
 * @brief Get the App UUID
 */
const char* getAppUUID() {
  return PULLDOG_UUID;
}

/**
 * @brief Get the Application Name
 * @return const char*
 */
const char* getAppName() {
  return PULLDOG_NAME;
}

/**
 * @brief Get logo path
 * @return const char*
 */
const char* getAppLogo() {
  return PULLDOG_LOGO;
}

/**
 * @brief Get the Application QSS File
 * @return const char*
 */
const char* getAppQSSLight() {
  return PULLDOG_LIGHT_QSS_PATH;
}

/**
 * @brief Get the Application QSS File
 * @return const char*
 */
const char* getAppQSSDark() {
  return PULLDOG_DARK_QSS_PATH;
}

/**
 * @brief Get App Home Directory
 * @return const char*
 */
std::string getAppHome() {
  return (std::filesystem::path(QDir::homePath().toStdString()) / (std::string(".") + getAppName())).string();
}

/**
 * @brief Get App Log File
 */
std::string getAppLogFile() {
  return (std::filesystem::path(getAppHome()) / "clipbird.log").string();
}

/**
 * @brief Get the App Home Page
 *
 * @return const char*
 */
const char* getAppHomePage() {
  return PULLDOG_HOMEPAGE;
}

/**
 * @brief Get the App Issue Page
 *
 * @return const char*
 */
const char* getAppIssuePage() {
  return PULLDOG_ISSUEPAGE;
}

/**
 * @brief Get the App Donate Page
 * @return const char*
 */
const char* getAppDonatePage() {
  return PULLDOG_DONATE;
}

/**
 * @brief Get the Organization Name
 *
 * @return const char*
 */
const char* getAppOrgName() {
  return PULLDOG_ORG_NAME;
}
}  // namespace srilakshmikanthanp::pulldog::config

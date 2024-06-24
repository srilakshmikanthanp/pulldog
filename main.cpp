// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <SingleApplication>
#include <QGuiApplication>
#include <QStyleHints>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QFile>
#include <QMenu>

#include <csignal>

#include "constants/constants.hpp"
#include "controller/controller.hpp"
#include "ui/gui/window/pulldog/pulldog.hpp"
#include "utility/logging/logging.hpp"

namespace srilakshmikanthanp::pulldog {
/**
 * @brief Single Application class specialization
 * for ClipBird Application
 */
class PullDogApplication : public SingleApplication {
 private:  // Member Functions
  /**
   * @brief On Tray Icon Clicked
   */
  void onTrayIconClicked(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::ActivationReason::Trigger) {
      window->isVisible() ? window->hide() : window->show();
    }
  }

  /**
   * @brief Set the Qss File for the color scheme
   */
  void setQssFile(Qt::ColorScheme scheme) {
    // detect system theme is dark or light
    bool isDark     = scheme == Qt::ColorScheme::Dark;

    // qss
    std::string qss = isDark ? constants::getAppQSSDark() : constants::getAppQSSLight();

    // QFile to read the qss file
    QFile qssFile(QString::fromStdString(qss));

    // open the qss file
    qssFile.open(QFile::ReadOnly);

    // set the style sheet
    qApp->setStyleSheet(qssFile.readAll());
  }

 private:  //  Member Variables and Objects

  ui::gui::window::PullDog *window;
  Controller *controller;
  QSystemTrayIcon *trayIcon;
  QMenu *trayMenu;

 private:  // Disable Copy, Move and Assignment

  Q_DISABLE_COPY_MOVE(PullDogApplication);

 public:  // Constructors and Destructors

  /**
   * @brief Construct a new PullDog Application object
   *
   * @param argc argument count
   * @param argv argument vector
   */
  PullDogApplication(int &argc, char **argv) : SingleApplication(argc, argv) {
    // Storage Instance
    auto storage = &storage::Storage::instance();

    // create the objects of the class
    controller = new Controller();
    window     = new ui::gui::window::PullDog(controller);
    trayIcon   = new QSystemTrayIcon();
    trayMenu   = new QMenu();

    // set the menu items
    trayMenu->addAction("Exit", [=]() { qApp->quit(); });

    // set the tray menu
    trayIcon->setContextMenu(trayMenu);

    // set the signal handler for all os
    signal(SIGTERM, [](int sig) { qApp->quit(); });
    signal(SIGINT, [](int sig) { qApp->quit(); });
    signal(SIGABRT, [](int sig) { qApp->quit(); });

    // set initial theme
    setQssFile(QGuiApplication::styleHints()->colorScheme());

    // set ToolTip
    trayIcon->setToolTip(QString::fromStdString(constants::getAppName()));

    // set the icon to tray
    trayIcon->setIcon(QIcon(constants::getAppLogo()));

    // set tooltip
    trayIcon->setToolTip(constants::getAppName());

    // set the icon to content
    window->setWindowIcon(QIcon(constants::getAppLogo()));

    // set minimum size
    window->setMinimumSize(constants::getAppMinSize());

    // set destination root
    controller->setDestinationRoot(storage->getDownloadPath());
    window->setDestinationRoot(storage->getDownloadPath());

    // set watch list
    for (const auto &path : storage->getPaths()) {
      controller->addWatchPath(path);
      window->addWatchPath(path);
    }

    // tray icon click from content
    QObject::connect(
      trayIcon, &QSystemTrayIcon::activated,
      this, &PullDogApplication::onTrayIconClicked
    );

    // set the signal for instance Started
    QObject::connect(
      this, &SingleApplication::instanceStarted,
      window, &ui::gui::window::PullDog::show
    );

    // detect the system theme
    QObject::connect(
      QGuiApplication::styleHints(),  &QStyleHints::colorSchemeChanged,
      this, &PullDogApplication::setQssFile
    );

    using ui::gui::window::PullDog;

    // set destination root signal to controller
    connect(
      window, &PullDog::destinationPathChanged,
      storage, &storage::Storage::setDownloadPath
    );

    // storage to controller
    connect(
      storage, &storage::Storage::onDownloadPathChanged,
      controller, &Controller::setDestinationRoot
    );

    // set watch list signal to controller
    connect(
      window, &PullDog::onFolderAdded,
      storage, &storage::Storage::addPath
    );

    // get watch list signal from controller
    connect(
      storage, &storage::Storage::onPathAdded,
      [=](const QString &path) { controller->addWatchPath(path); }
    );

    // remove watch list signal to controller
    connect(
      window, &PullDog::onFolderRemoved,
      storage, &storage::Storage::removePath
    );

    // storage to controller
    connect(
      storage, &storage::Storage::onPathRemoved,
      [=](const QString &path) { controller->removeWatchPath(path); }
    );

    // on copy start
    connect(
      controller, &Controller::onCopyStart,
      window, &PullDog::addTransfer
    );

    // on copy end
    connect(
      controller, &Controller::onCopyEnd,
      window, &PullDog::removeTransfer
    );

    // on progress changed
    connect(
      controller, &Controller::onCopy,
      window, &PullDog::onProgressChanged
    );

    // on copy cancel
    connect(
      controller, &Controller::onCopyCancel,
      window, &PullDog::removeTransfer
    );

    // show tray icon
    trayIcon->show();
  }

  /**
   * @brief Destroy the PullDog Application
   * Object and it's members
   */
  virtual ~PullDogApplication() {
    delete controller;
    delete window;
    delete trayIcon;
    delete trayMenu;
  }

  /**
   * @brief get the controller
   */
  Controller *getController() const {
    return controller;
  }
};

/**
 * @brief Custom event filter for the application that
 * captures window shown event and if the window is decorated
 * apply some attributes to the window
 */
class PullDogEventFilter : public QObject {
  virtual bool eventFilter(QObject *o, QEvent *e) {
    if (e->type() == QEvent::WindowActivate) {
      handleWindowShownEvent(dynamic_cast<QWidget *>(o));
    }

    return QObject::eventFilter(o, e);
  }

  void handleWindowShownEvent(QWidget *window) {
    if (!(window->windowFlags() & Qt::FramelessWindowHint)) {
      utility::setPlatformAttributes(window);
    }
  }
};
} // namespace srilakshmikanthanp::pulldog

/**
 * @brief Global Error Handler that helps to log
 */
void globalErrorHandler() {
  try {
    std::rethrow_exception(std::current_exception());
  } catch (const std::exception &e) {
    qCritical() << e.what();
  } catch (...) {
    qCritical() << "Unknown Exception";
  }

  std::abort();
}

/**
 * @brief main function
 */
int main(int argc, char *argv[]) {
  // using some classes from namespace
  using srilakshmikanthanp::pulldog::constants::getAppHome;
  using srilakshmikanthanp::pulldog::constants::getAppLogo;
  using srilakshmikanthanp::pulldog::constants::getAppLogFile;
  using srilakshmikanthanp::pulldog::constants::getAppName;
  using srilakshmikanthanp::pulldog::constants::getAppHome;
  using srilakshmikanthanp::pulldog::constants::getAppOrgName;
  using srilakshmikanthanp::pulldog::constants::getAppVersion;
  using srilakshmikanthanp::pulldog::PullDogApplication;
  using srilakshmikanthanp::pulldog::PullDogEventFilter;
  using srilakshmikanthanp::pulldog::Controller;
  using srilakshmikanthanp::pulldog::logging::Logger;

  PullDogApplication app(argc, argv);

#ifdef NDEBUG
  // log file to record the logs
  QFile logfile(QString::fromStdString(getAppLogFile()));

  // open the log file
  logfile.open(QIODevice::WriteOnly | QIODevice::Append);

  // open QStream to write the log file
  QTextStream logstream(&logfile);

  // Set the log file
  Logger::setLogStream(&logstream);
#else
  // open QStream to write the log file
  QTextStream logstream(stdout);

  // Set the log file as std::cout
  Logger::setLogStream(&logstream);
#endif

  // Home Directory of the application
  auto path = QString::fromStdString(getAppHome());

  // make app home directory if not exists
  if (!QDir(path).exists() && !QDir().mkdir(path)) {
    QMessageBox::critical(nullptr, "Error", "Can't Create App Home");
    return EXIT_FAILURE;
  }

  // get controller
  auto controller = app.getController();

  // controller on error signal to log
  QObject::connect(
    controller, &Controller::onError,
    [](const QString &error) { qCritical() << error; }
  );

  // set not to quit on last content closed
  app.setQuitOnLastWindowClosed(false);

  // event filter for the application
  auto filter = new PullDogEventFilter();

  // set the event filter
  app.installEventFilter(filter);

  // Set the custom message handler
  qInstallMessageHandler(Logger::handler);

  // Set the global error handler
  std::set_terminate(globalErrorHandler);

  // set parent for the filter
  filter->setParent(&app);

  return app.exec();
}

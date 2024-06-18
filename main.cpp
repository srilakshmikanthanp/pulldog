// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <SingleApplication>
#include <QApplication>
#include <QPushButton>

#include "controller/controller.hpp"

#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <iostream>

int main(int argc, char *argv[]) {
  SingleApplication app(argc, argv);
  using srilakshmikanthanp::pulldog::Controller;
  auto &controller = Controller::instance("D:\\Temp");
  controller.addPath("S:\\Txt");
  controller.addPath("D:\\Txt");
  return app.exec();
}

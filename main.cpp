// Copyright (c) 2024 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <SingleApplication>
#include <QPushButton>

#include "controller/controller.hpp"

int main(int argc, char *argv[]) {
  SingleApplication app(argc, argv);
  using srilakshmikanthanp::pulldog::Controller;
  auto &controller = Controller::instance("D:\\Temp");
  controller.addPath("S:\\Txt");
  controller.addPath("D:\\Txt");
  QPushButton button("Hello World");
  button.show();
  return app.exec();
}

#include <iostream>

#include <QApplication>
#include "viewer.h"

using namespace std;

int main(int argc, char** argv) {
  if (2 != argc) {
    cout << "usage:\n" << argv[0] << " <stl file>\n";
    return -1;
  }

  QApplication application(argc, argv);
  Viewer viewer{argv[1]};
  return application.exec();
}
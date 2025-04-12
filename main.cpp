#include <QApplication>
#include "filemanager.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>
#include <QTimer>
#include <QMap>
#include <QDirIterator>



int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    FileManager window;
    window.show();
    return app.exec();
}

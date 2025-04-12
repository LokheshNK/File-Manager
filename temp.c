
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

FileManager::FileManager(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    model = new QFileSystemModel(this);
    QString defaultPath = QDir::homePath();
    model->setRootPath(defaultPath);

    treeView = new QTreeView(this);
    treeView->setModel(model);
    treeView->setRootIndex(model->index(defaultPath));
    connect(treeView, &QTreeView::doubleClicked, this, &FileManager::onFileOpened);

    searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("Search for a file...");
    searchResults = new QListWidget(this);
    searchResults->setVisible(false); // Hide until needed

    connect(searchBox, &QLineEdit::textChanged, this, &FileManager::updateSearchResults);
    connect(searchResults, &QListWidget::itemDoubleClicked, this, &FileManager::openSelectedFile);

    deleteButton = new QPushButton("Delete File", this);
    copyButton = new QPushButton("Copy File", this);
    moveButton = new QPushButton("Move File", this);
    pasteButton = new QPushButton("Paste File", this);
    pasteButton->setVisible(false);
    digitalWellBeingButton = new QPushButton("View Digital Well-Being", this);
    showRecycleBinButton = new QPushButton("Show Recycle Bin", this);
    emptyRecycleBinButton = new QPushButton("Empty Recycle Bin", this);

    connect(deleteButton, &QPushButton::clicked, this, &FileManager::deleteFile);
    connect(copyButton, &QPushButton::clicked, this, &FileManager::copyFile);
    connect(moveButton, &QPushButton::clicked, this, &FileManager::moveFile);
    connect(pasteButton, &QPushButton::clicked, this, &FileManager::pasteFile);
    connect(digitalWellBeingButton, &QPushButton::clicked, this, &FileManager::viewDigitalWellBeing);
    connect(showRecycleBinButton, &QPushButton::clicked, this, &FileManager::showRecycleBin);
    connect(emptyRecycleBinButton, &QPushButton::clicked, this, &FileManager::emptyRecycleBin);

    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(copyButton);
    buttonLayout->addWidget(moveButton);
    buttonLayout->addWidget(pasteButton);
    buttonLayout->addWidget(digitalWellBeingButton);
    buttonLayout->addWidget(showRecycleBinButton);
    buttonLayout->addWidget(emptyRecycleBinButton);
    buttonLayout->setAlignment(Qt::AlignRight);

    mainLayout->addWidget(searchBox);
    mainLayout->addWidget(searchResults);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(treeView);
}


void FileManager::updateSearchResults(const QString &query) {
    searchResults->clear();

    if (query.isEmpty()) {
        searchResults->setVisible(false);
        return;
    }

    QString searchPath = model->rootPath();
    QDirIterator it(searchPath, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QStringList matches;

    while (it.hasNext()) {
        QString filePath = it.next();
        QFileInfo fileInfo(filePath);
        if (fileInfo.fileName().contains(query, Qt::CaseInsensitive)) {
            matches.append(filePath);
        }
    }

    if (matches.isEmpty()) {
        searchResults->addItem("No matches found");
    } else {
        searchResults->addItems(matches);
    }

    searchResults->setVisible(true);
}

void FileManager::openSelectedFile(QListWidgetItem *item) {
    QString filePath = item->text();
    if (filePath == "No matches found") return;

    trackFileUsage(filePath, true);
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(filePath))) {
        QMessageBox::warning(this, "Error", "Failed to open file.");
    }
}


void FileManager::trackFileUsage(const QString &filePath, bool isOpening) {
    QFile logFile("file_usage_log.txt");
    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&logFile);
        if (isOpening) {
            fileOpenTime[filePath] = QDateTime::currentDateTime();
            out << filePath << ",opened," << QDateTime::currentDateTime().toString() << "\n";
        } else {
            if (fileOpenTime.contains(filePath)) {
                QDateTime openTime = fileOpenTime[filePath];
                qint64 duration = openTime.msecsTo(QDateTime::currentDateTime()) / 1000; // Duration in seconds
                out << filePath << ",closed," << QDateTime::currentDateTime().toString() << ",duration," << duration << " seconds\n";
                fileOpenTime.remove(filePath);
            }
        }
    }
}

void FileManager::onFileOpened(const QModelIndex &index) {
    QString filePath = model->filePath(index);
    trackFileUsage(filePath, true);
    try {
        if (!QDesktopServices::openUrl(QUrl::fromLocalFile(filePath))) {
            throw std::runtime_error("Failed to open file");
        }
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}


void FileManager::copyFile() {
    QModelIndex index = treeView->currentIndex();
    copiedFilePath = model->filePath(index);
    pasteButton->setVisible(true);  // Show paste button after copying
    QMessageBox::information(this, "File Copied", "Copied: " + copiedFilePath);
}

void FileManager::moveFile() {
    QModelIndex index = treeView->currentIndex();
    QString filePath = model->filePath(index);
    QString destPath = QFileDialog::getExistingDirectory(this, "Select Destination Folder");
    if (!destPath.isEmpty()) {
        QString fileName = QFileInfo(filePath).fileName();
        QString newFilePath = destPath + "/" + fileName;
        if (QFile::rename(filePath, newFilePath)) {
            QMessageBox::information(this, "File Moved", "Moved to: " + newFilePath);
        } else {
            QMessageBox::warning(this, "Error", "Could not move file.");
        }
    }
}

void FileManager::pasteFile() {
    if (copiedFilePath.isEmpty()) return;

    QString destinationDir = QFileDialog::getExistingDirectory(this, "Select Destination");
    if (destinationDir.isEmpty()) return;

    QFileInfo fileInfo(copiedFilePath);
    QString destinationPath = destinationDir + "/" + fileInfo.fileName();

    if (QFile::exists(destinationPath)) {
        QMessageBox::warning(this, "Error", "File already exists at the destination.");
        return;
    }

    if (QFile::copy(copiedFilePath, destinationPath)) {
        QMessageBox::information(this, "Success", "File copied to " + destinationPath);
    } else {
        QMessageBox::warning(this, "Error", "File copy failed.");
    }
}

void FileManager::viewDigitalWellBeing() {
    QString pythonPath = "C:/Python313/python.exe";  // Ensure this path is correct
    QString scriptPath = "C:/Users/lokhe/OneDrive/Documents/untitled/digital_wellbeing.py";

    if (!QFile::exists(pythonPath)) {
        QMessageBox::warning(this, "Error", "Python executable not found: " + pythonPath);
        return;
    }
    if (!QFile::exists(scriptPath)) {
        QMessageBox::warning(this, "Error", "Script not found: " + scriptPath);
        return;
    }

    bool started = QProcess::startDetached(pythonPath, QStringList() << scriptPath);
    if (!started) {
        QMessageBox::warning(this, "Error", "Failed to start Digital Well-Being script.");
    }
}


void FileManager::deleteFile() {
    QModelIndex index = treeView->currentIndex();
    QString filePath = model->filePath(index);
    QString recycleBinPath = QDir::homePath() + "/RecycleBin/";

    if (!QDir(recycleBinPath).exists()) {
        QDir().mkdir(recycleBinPath);
    }

    QString fileName = QFileInfo(filePath).fileName();
    QString newFilePath = recycleBinPath + fileName;
    if (QFile::rename(filePath, newFilePath)) {
        QMessageBox::information(this, "File Moved to Recycle Bin", "Moved: " + filePath);
    } else {
        QMessageBox::warning(this, "Error", "Could not move file to Recycle Bin.");
    }
}

void FileManager::showRecycleBin() {
    QWidget *recycleBinWindow = new QWidget;
    recycleBinWindow->setWindowTitle("Recycle Bin");
    QVBoxLayout *layout = new QVBoxLayout(recycleBinWindow);
    recycleBinList = new QListWidget(recycleBinWindow);
    QPushButton *restoreButton = new QPushButton("Restore", recycleBinWindow);
    QPushButton *deleteButton = new QPushButton("Delete Permanently", recycleBinWindow);

    QString recycleBinPath = QDir::homePath() + "/RecycleBin/";
    QDir dir(recycleBinPath);
    QStringList files = dir.entryList(QDir::Files);
    recycleBinList->addItems(files);

    layout->addWidget(recycleBinList);
    layout->addWidget(restoreButton);
    layout->addWidget(deleteButton);

    connect(restoreButton, &QPushButton::clicked, this, &FileManager::restoreFile);
    connect(deleteButton, &QPushButton::clicked, this, &FileManager::removeFromRecycleBin);

    recycleBinWindow->setLayout(layout);
    recycleBinWindow->show();
}

void FileManager::restoreFile() {
    QString recycleBinPath = QDir::homePath() + "/RecycleBin/";
    QString fileName = recycleBinList->currentItem()->text();
    QString filePath = recycleBinPath + fileName;
    QString restorePath = QDir::homePath() + "/" + fileName;

    if (QFile::rename(filePath, restorePath)) {
        QMessageBox::information(this, "File Restored", "Restored: " + restorePath);
        delete recycleBinList->takeItem(recycleBinList->currentRow());
    } else {
        QMessageBox::warning(this, "Error", "Could not restore file.");
    }
}

void FileManager::removeFromRecycleBin() {
    QString recycleBinPath = QDir::homePath() + "/RecycleBin/";
    QString fileName = recycleBinList->currentItem()->text();
    QString filePath = recycleBinPath + fileName;

    if (QFile::remove(filePath)) {
        QMessageBox::information(this, "File Deleted Permanently", "Deleted: " + fileName);
        delete recycleBinList->takeItem(recycleBinList->currentRow());
    } else {
        QMessageBox::warning(this, "Error", "Could not delete file.");
    }
}

void FileManager::emptyRecycleBin() {
    QString recycleBinPath = QDir::homePath() + "/RecycleBin/";
    QDir dir(recycleBinPath);
    dir.setFilter(QDir::Files);
    foreach (QString file, dir.entryList()) {
        dir.remove(file);
    }
    QMessageBox::information(this, "Recycle Bin Emptied", "All files removed.");
}



int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    FileManager window;
    window.show();
    return app.exec();
}

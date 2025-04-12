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
#include <QtConcurrent>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>
#include <QTimer>
#include <QMap>
#include <QDirIterator>
#include <QFuture>
#include <QFutureWatcher>
#include <QProcess>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>
#include <QInputDialog>
#include <QDir>


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
    newFileButton = new QPushButton("New File", this);
    copyButton = new QPushButton("Copy File", this);
    pasteButton = new QPushButton("Paste File", this);
    pasteButton->setVisible(false);
    moveButton = new QPushButton("Move File", this);
    renameButton = new QPushButton("Rename", this);
    deleteButton = new QPushButton("Delete File", this);
    showRecycleBinButton = new QPushButton("Show Recycle Bin", this);
    emptyRecycleBinButton = new QPushButton("Empty Recycle Bin", this);
    propertiesButton = new QPushButton("Properties", this);
    QPushButton *uploadButton = new QPushButton("Upload to Drive", this);
    digitalWellBeingButton = new QPushButton("View Digital Well-Being", this);
    buttonLayout->addWidget(newFileButton);

    connect(propertiesButton, &QPushButton::clicked, this, &FileManager::showFileProperties);
    //openCmdButton = new QPushButton("Open in CMD", this);

    //connect(openCmdButton, &QPushButton::clicked, this, &FileManager::openInCommandPrompt);
    connect(searchBox, &QLineEdit::textChanged, this, &FileManager::updateSearchResults);
    connect(searchResults, &QListWidget::itemDoubleClicked, this, &FileManager::openSelectedFile);

    connect(newFileButton, &QPushButton::clicked, this, &FileManager::createNewFile);
    connect(deleteButton, &QPushButton::clicked, this, &FileManager::deleteFile);
    connect(copyButton, &QPushButton::clicked, this, &FileManager::copyFile);
    connect(moveButton, &QPushButton::clicked, this, &FileManager::moveFile);
    connect(pasteButton, &QPushButton::clicked, this, &FileManager::pasteFile);
    connect(renameButton, &QPushButton::clicked, this, &FileManager::renameFileOrFolder);
    connect(digitalWellBeingButton, &QPushButton::clicked, this, &FileManager::viewDigitalWellBeing);
    connect(showRecycleBinButton, &QPushButton::clicked, this, &FileManager::showRecycleBin);
    connect(emptyRecycleBinButton, &QPushButton::clicked, this, &FileManager::emptyRecycleBin);
    connect(uploadButton, &QPushButton::clicked, this, &FileManager::uploadToGoogleDrive);

    buttonLayout->addWidget(copyButton);
    buttonLayout->addWidget(moveButton);
    buttonLayout->addWidget(pasteButton);
    buttonLayout->addWidget(renameButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(showRecycleBinButton);
    buttonLayout->addWidget(emptyRecycleBinButton);
    buttonLayout->addWidget(digitalWellBeingButton);
    buttonLayout->addWidget(propertiesButton);
    buttonLayout->addWidget(uploadButton);
    //buttonLayout->addWidget(openCmdButton);
    buttonLayout->setAlignment(Qt::AlignRight);
    mainLayout->addWidget(searchBox);
    mainLayout->addWidget(searchResults);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(treeView);
}
void FileManager::uploadToGoogleDrive() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select File to Upload", "", "All Files (*)");

    if (filePath.isEmpty()) {
        qDebug() << "No file selected. Upload canceled.";
        return;
    }

    qDebug() << "Selected File:" << filePath;

    QString folderID = ""; // Replace with actual Google Drive folder ID if needed
    bool success = uploadFileToGoogleDrive(filePath, folderID);

    if (success) {
        QMessageBox::information(this, "Success", "File uploaded successfully!");
    } else {
        QMessageBox::critical(this, "Error", "File upload failed.");
    }
}

#define TOKEN_FILE "token.txt"

// Function to read OAuth 2.0 token
bool FileManager::readOAuthToken(QString &token) {
    QFile tokenFile(TOKEN_FILE);
    if (!tokenFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "❌ Error opening token file!";
        return false;
    }
    QTextStream in(&tokenFile);
    token = in.readLine().trimmed();
    tokenFile.close();

    if (token.isEmpty()) {
        qDebug() << "❌ Token file is empty!";
        return false;
    }

    return true;
}

// Function to upload a file to Google Drive
bool FileManager::uploadFileToGoogleDrive(const QString &filePath, const QString &folderID) {
    QString token;
    if (!readOAuthToken(token)) {
        return false;
    }

    QString fileName = QFileInfo(filePath).fileName();

    // Construct the cURL command
    QString command = QString(
                          "curl -X POST -H \"Authorization: Bearer %1\" "
                          "-H \"Content-Type: multipart/related; boundary=foo_bar_baz\" "
                          "--data-binary @- \"https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart\" <<EOF\n"
                          "--foo_bar_baz\n"
                          "Content-Type: application/json; charset=UTF-8\n\n"
                          "{\"name\": \"%2\"}\n"
                          "--foo_bar_baz\n"
                          "Content-Type: application/octet-stream\n\n"
                          "@%3\n"
                          "--foo_bar_baz--\n"
                          "EOF"
                          ).arg(token, fileName, filePath);

    QProcess process;
    process.start(command);
    process.waitForFinished();

    QString output = process.readAllStandardOutput();
    QString error = process.readAllStandardError();

    if (!error.isEmpty()) {
        qDebug() << "❌ Upload Error: " << error;
        return false;
    }

    qDebug() << "✅ Upload Success: " << output;
    return true;
}




void FileManager::showFileProperties() {
    QModelIndex index = treeView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Error", "No file selected!");
        return;
    }

    QString filePath = model->filePath(index);
    QFileInfo fileInfo(filePath);

    if (!fileInfo.exists()) {
        QMessageBox::warning(this, "Error", "File not found!");
        return;
    }

    // Format file size
    qint64 fileSize = fileInfo.size();
    QString sizeStr;
    if (fileSize < 1024)
        sizeStr = QString::number(fileSize) + " bytes";
    else if (fileSize < 1024 * 1024)
        sizeStr = QString::number(fileSize / 1024.0, 'f', 2) + " KB";
    else if (fileSize < 1024 * 1024 * 1024)
        sizeStr = QString::number(fileSize / (1024.0 * 1024), 'f', 2) + " MB";
    else
        sizeStr = QString::number(fileSize / (1024.0 * 1024 * 1024), 'f', 2) + " GB";

    // Get file type (extension)
    QString fileType = fileInfo.suffix().isEmpty() ? "Unknown" : fileInfo.suffix().toUpper();

    // Get file attributes (similar to Windows)
    QString attributes;
    if (fileInfo.isReadable()) attributes += "Readable, ";
    if (fileInfo.isWritable()) attributes += "Writable, ";
    if (fileInfo.isExecutable()) attributes += "Executable, ";
    if (fileInfo.isHidden()) attributes += "Hidden, ";
    if (!attributes.isEmpty()) attributes.chop(2);  // Remove trailing comma

    // Get timestamps
    QString createdTime = fileInfo.birthTime().toString("yyyy-MM-dd hh:mm:ss");
    QString modifiedTime = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
    QString accessedTime = fileInfo.lastRead().toString("yyyy-MM-dd hh:mm:ss");

    // Format output message
    QString details = QString(
                          "File Name: %1\n"
                          "File Path: %2\n"
                          "File Size: %3\n"
                          "File Type: %4\n"
                          "Created: %5\n"
                          "Last Modified: %6\n"
                          "Last Accessed: %7\n"
                          "Attributes: %8"
                          ).arg(fileInfo.fileName())
                          .arg(fileInfo.absoluteFilePath())
                          .arg(sizeStr)
                          .arg(fileType)
                          .arg(createdTime.isEmpty() ? "Unknown" : createdTime)
                          .arg(modifiedTime)
                          .arg(accessedTime)
                          .arg(attributes.isEmpty() ? "None" : attributes);

    // Show the properties dialog
    QMessageBox::information(this, "File Properties", details);
}

void FileManager::openInCommandPrompt() {
    QModelIndex index = treeView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Error", "No folder or file selected!");
        return;
    }

    QString path = model->filePath(index);
    QFileInfo fileInfo(path);

    // If a file is selected, open CMD in its parent directory
    QString directoryPath = fileInfo.isDir() ? fileInfo.absoluteFilePath() : fileInfo.absolutePath();

    // Convert path to Windows format
    directoryPath = QDir::toNativeSeparators(directoryPath);

    // Debugging output
    qDebug() << "Opening CMD in directory:" << directoryPath;

    // Open CMD in a new terminal window
    bool started = QProcess::startDetached("cmd.exe", QStringList() << "/K" << "cd /d \"" + directoryPath + "\"");

    if (!started) {
        QMessageBox::warning(this, "Error", "Failed to open Command Prompt.");
    }
}

void FileManager::createNewFile() {
    // Ask the user for the file name with extension
    bool ok;
    QString fileName = QInputDialog::getText(this, "Create New File",
                                             "Enter file name with extension (e.g., myfile.txt):",
                                             QLineEdit::Normal, "", &ok);

    if (!ok || fileName.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "Invalid file name!");
        return;
    }

    // Ask the user for the directory to save the file
    QString directoryPath = QFileDialog::getExistingDirectory(this, "Select Folder to Save File");
    if (directoryPath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No folder selected!");
        return;
    }

    QString fullPath = directoryPath + "/" + fileName;

    // Check if the file already exists
    if (QFile::exists(fullPath)) {
        QMessageBox::warning(this, "Error", "File already exists!");
        return;
    }

    // Create the file
    QFile newFile(fullPath);
    if (newFile.open(QIODevice::WriteOnly)) {
        newFile.close();
        QMessageBox::information(this, "Success", "File created successfully:\n" + fullPath);
    } else {
        QMessageBox::warning(this, "Error", "Could not create file.");
    }
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


std::mutex mtx;  // Mutex for thread synchronization if needed

// Class Member
QString copiedFilePath; // Path of the copied file for paste operation

void FileManager::copyFile() {
    QModelIndex index = treeView->currentIndex();
    copiedFilePath = model->filePath(index);
    pasteButton->setVisible(true);  // Show paste button after copying
    QMessageBox::information(this, "File Copied", "Copied: " + copiedFilePath);

    // Run the copy operation in a new thread
    std::thread copyThread(&FileManager::performCopy, this);
    copyThread.detach();  // Detach the thread to allow independent execution
}

void FileManager::moveFile() {
    QModelIndex index = treeView->currentIndex();
    QString filePath = model->filePath(index);
    QString destPath = QFileDialog::getExistingDirectory(this, "Select Destination Folder");
    if (!destPath.isEmpty()) {
        QString fileName = QFileInfo(filePath).fileName();
        QString newFilePath = destPath + "/" + fileName;

        // Run the move operation in a new thread
        std::thread moveThread(&FileManager::performMove, this, filePath, newFilePath);
        moveThread.detach();  // Detach the thread to allow independent execution
    }
}

void FileManager::renameFileOrFolder() {
    QModelIndex index = treeView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Error", "No file or folder selected!");
        return;
    }

    QString oldPath = model->filePath(index);
    QFileInfo fileInfo(oldPath);
    QString oldName = fileInfo.fileName();

    bool ok;
    QString newName = QInputDialog::getText(this, "Rename",
                                            "Enter new name:", QLineEdit::Normal,
                                            oldName, &ok);
    if (!ok || newName.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "Invalid name!");
        return;
    }

    QString newPath = fileInfo.absolutePath() + "/" + newName;

    if (QFile::exists(newPath)) {
        QMessageBox::warning(this, "Error", "A file or folder with this name already exists!");
        return;
    }

    if (QFile::rename(oldPath, newPath)) {
        QMessageBox::information(this, "Success", "Renamed successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Rename failed!");
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


void FileManager::pasteFile() {
    if (copiedFilePath.isEmpty()) return;

    QString destinationDir = QFileDialog::getExistingDirectory(this, "Select Destination");
    if (destinationDir.isEmpty()) return;

    QFileInfo fileInfo(copiedFilePath);
    QString destinationPath = destinationDir + "/" + fileInfo.fileName();

    // Run the paste operation in a new thread
    std::thread pasteThread(&FileManager::performPaste, this, copiedFilePath, destinationPath);
    pasteThread.detach();  // Detach the thread to allow independent execution
}

// Copy operation executed in a separate thread
void FileManager::performCopy() {
    std::lock_guard<std::mutex> lock(mtx);  // Lock to ensure thread safety if needed
    bool success = QFile::copy(copiedFilePath, copiedFilePath); // Placeholder copy logic
    emit copyFinished(success);  // Notify when copy is finished
}

// Move operation executed in a separate thread
void FileManager::performMove(const QString &filePath, const QString &newFilePath) {
    std::lock_guard<std::mutex> lock(mtx);  // Lock to ensure thread safety if needed
    bool success = QFile::rename(filePath, newFilePath);  // Move the file
    emit moveFinished(success);  // Notify when move is finished
}



// Paste operation executed in a separate thread
void FileManager::performPaste(const QString &srcPath, const QString &destPath) {
    std::lock_guard<std::mutex> lock(mtx);  // Lock to ensure thread safety if needed
    bool success = QFile::copy(srcPath, destPath);  // Copy file to destination
    emit pasteFinished(success);  // Notify when paste is finished
}

// Slot to handle copy operation completion
void FileManager::onCopyFinished(bool success) {
    if (success) {
        QMessageBox::information(this, "Copy Finished", "File copied successfully.");
    } else {
        QMessageBox::warning(this, "Copy Failed", "Failed to copy the file.");
    }
}

// Slot to handle move operation completion
void FileManager::onMoveFinished(bool success) {
    if (success) {
        QMessageBox::information(this, "Move Finished", "File moved successfully.");
    } else {
        QMessageBox::warning(this, "Move Failed", "Failed to move the file.");
    }
}


// Slot to handle paste operation completion
void FileManager::onPasteFinished(bool success) {
    if (success) {
        QMessageBox::information(this, "Paste Finished", "File pasted successfully.");
    } else {
        QMessageBox::warning(this, "Paste Failed", "Failed to paste the file.");
    }
}

void FileManager::viewDigitalWellBeing() {
    QString pythonPath = "C:/Python313/python.exe";
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

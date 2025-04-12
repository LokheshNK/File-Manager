#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QListWidget>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QInputDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>
#include <QThreadPool>
#include <QObject>
#include <QFutureWatcher>
#include <QLineEdit>
#include <QMap>
#include <thread>
#include <mutex>

class FileManager : public QWidget {
    Q_OBJECT
public:
    explicit FileManager(QWidget *parent = nullptr);
    void trackFileUsage(const QString &filePath);
    void trackFileUsage(const QString &filePath, bool isOpening);
    void viewDigitalWellBeing();

private:
    QPushButton *digitalWellBeingButton;
    QPushButton *propertiesButton;
    QPushButton *openCmdButton;
    QPushButton *showRecycleBinButton;
    QPushButton *newFileButton;
    QPushButton *emptyRecycleBinButton;
    QListWidget *recycleBinList;
    QFileSystemModel *model;
     QPushButton *renameButton;
    void setupGoogleDriveButtons(QHBoxLayout *buttonLayout);
    QTreeView *treeView;
    QPushButton *deleteButton, *copyButton, *moveButton, *pasteButton;
    QString copiedFilePath;
    QMap<QString, QDateTime> fileOpenTime;

    std::mutex fileMutex;

private slots:
    void updateSearchResults(const QString &query);
    void openInCommandPrompt();
    void showFileProperties();
    void openSelectedFile(QListWidgetItem *item);
    void createNewFile();
    void mountGoogleDrive();
    void uploadToGoogleDrive();
    void downloadFromGoogleDrive();
    void showRecycleBin();
    void restoreFile();
    void removeFromRecycleBin();
    void emptyRecycleBin();
    void onFileOpened(const QModelIndex &index);
    void deleteFile();
    void copyFile();
    void moveFile();
    void pasteFile();
    void renameFileOrFolder();

private:
    QFutureWatcher<QStringList> *searchWatcher;
    QLineEdit *searchBox;
    QListWidget *searchResults;

    bool uploadFileToGoogleDrive(const QString& filePath, const QString& folderID);
    bool readOAuthToken(QString &token);

    // Thread functions
    void performCopy();
    void performMove(const QString &filePath, const QString &newFilePath);
    void performPaste(const QString &srcPath, const QString &destPath);


signals:
    void copyFinished(bool success);
    void moveFinished(bool success);
    void pasteFinished(bool success);

private slots:
    void onCopyFinished(bool success);
    void onMoveFinished(bool success);
    void onPasteFinished(bool success);
};

#endif // FILEMANAGER_H

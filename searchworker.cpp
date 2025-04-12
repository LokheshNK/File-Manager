#include "searchworker.h"
#include <QDirIterator>

SearchWorker::SearchWorker(QObject *parent) : QObject(parent) {}

void SearchWorker::setSearchParams(const QString &query, const QString &directory) {
    searchQuery = query;
    searchDirectory = directory;
}

void SearchWorker::searchFiles() {
    QStringList results;
    QDirIterator it(searchDirectory, QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString filePath = it.next();
        if (filePath.contains(searchQuery, Qt::CaseInsensitive)) {
            results.append(filePath);
        }
    }

    emit searchFinished(results);
}

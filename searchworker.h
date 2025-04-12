#ifndef SEARCHWORKER_H
#define SEARCHWORKER_H

#include <QObject>
#include <QStringList>

class SearchWorker : public QObject {
    Q_OBJECT
public:
    explicit SearchWorker(QObject *parent = nullptr);
    void setSearchParams(const QString &query, const QString &directory);

signals:
    void searchFinished(QStringList results);

public slots:
    void searchFiles();

private:
    QString searchQuery;
    QString searchDirectory;
};

#endif // SEARCHWORKER_H

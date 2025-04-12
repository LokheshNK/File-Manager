#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define TOKEN_FILE "token.txt"  // Store your OAuth token here
pthread_mutex_t lock;

// Function to list files in a directory
void list_files(const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("❌ Unable to open directory");
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
}

// Structure to hold copy file arguments
typedef struct {
    char src[BUFFER_SIZE];
    char dest[BUFFER_SIZE];
} CopyArgs;

// Function to copy a file (multi-threaded)
void *copy_file(void *arg) {
    pthread_mutex_lock(&lock);
    CopyArgs *args = (CopyArgs *)arg;

    FILE *source, *destination;
    char buffer[BUFFER_SIZE];
    size_t bytes;

    source = fopen(args->src, "rb");
    if (source == NULL) {
        perror("❌ Error opening source file");
        pthread_mutex_unlock(&lock);
        return NULL;
    }

    destination = fopen(args->dest, "wb");
    if (destination == NULL) {
        perror("❌ Error opening destination file");
        fclose(source);
        pthread_mutex_unlock(&lock);
        return NULL;
    }

    while ((bytes = fread(buffer, 1, BUFFER_SIZE, source)) > 0) {
        fwrite(buffer, 1, bytes, destination);
    }

    fclose(source);
    fclose(destination);

    printf("✅ File copied successfully from %s to %s\n", args->src, args->dest);
    pthread_mutex_unlock(&lock);
    return NULL;
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
                          "-F \"metadata={\\\"name\\\":\\\"%2\\\"};type=application/json\" "
                          "-F \"file=@%3\" "
                          "\"https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart\""
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

int main() {
    pthread_mutex_init(&lock, NULL);

    int choice;
    char path[BUFFER_SIZE];
    char src[BUFFER_SIZE], dest[BUFFER_SIZE];

    while (1) {
        printf("\n🔥 File Manager:\n");
        printf("1. List files in directory\n");
        printf("2. Copy a file\n");
        printf("3. Upload file to Google Drive\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();  // Consume newline

        switch (choice) {
        case 1:
            printf("Enter directory path: ");
            scanf("%s", path);
            list_files(path);
            break;
        case 2:
            printf("Enter source file path: ");
            scanf("%s", src);
            printf("Enter destination file path: ");
            scanf("%s", dest);

            CopyArgs *args = malloc(sizeof(CopyArgs));
            strcpy(args->src, src);
            strcpy(args->dest, dest);

            pthread_t thread;
            pthread_create(&thread, NULL, copy_file, args);
            pthread_join(thread, NULL);
            free(args);
            break;
        case 3:
            printf("Enter file path to upload: ");
            scanf("%s", path);
            upload_to_drive(path);
            break;
        case 4:
            pthread_mutex_destroy(&lock);
            exit(0);
        default:
            printf("❌ Invalid choice!\n");
        }
    }
    return 0;
}

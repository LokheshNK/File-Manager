# Smart File Manager

Smart File Manager is a Qt-based desktop application that provides a robust and user-friendly interface for managing files and folders. Designed with productivity and minimalism in mind, it combines essential file operations with modern utilities like digital well-being tracking and seamless Google Drive upload using OAuth authentication.

## Features
-It uses minimal UI which ensures user friendly
-just a click and upload file to drive


### Core File Management
- **File Search**: Search for files or folders by name or extension across directories.
- **Properties Viewer**: View file metadata including name, size, type, creation/modification dates.
- **Command Prompt Access**: Open a terminal from the current directory within the app.
- **File Operations**: Perform copy, move, delete, and paste actions with support for bulk operations.

### Productivity Tools
- **Recycle Bin**: Soft-delete system that stores deleted items for recovery or permanent deletion.
- **Digital Well-being Tracker**: Monitors active usage time of the application and encourages healthy digital habits.
- **Google Drive Upload**: Upload files directly to Google Drive via OAuth tokens using secure `cURL` calls.

### Prerequisites
To upload to gl drive we need RClone, so install Rclone from official website and install it.
In cmd setup rclone and generate access token for google drive.
Paste the access token in the token.txt.

## Installation
Install Qt creator from the official website
open the cloned project and makesure u completed the Prerequisites.
now enjoy the smart Filemanager by executing the file.





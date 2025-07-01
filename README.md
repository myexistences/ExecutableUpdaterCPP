


# ExecutableUpdaterCPP

![ViewCount](https://komarev.com/ghpvc/?username=myexistences&label=Views&color=blue&style=flat)
![License](https://img.shields.io/badge/license-MIT-green.svg)

> **A professional, header-only C++ library for automatic executable updates on Windows.**
>
> This project enables seamless version checking, downloading, and updating of Windows applications with minimal setup. Built using modern C++ and native Windows APIs, it’s ideal for portable or standalone apps needing self-update capability.


## 📦 Overview

**ExecutableUpdaterCPP** is a lightweight, header-only C++ updater framework for Windows applications. It checks a remote JSON file for the latest version, downloads the updated executable if needed, and safely replaces and restarts the application—all automatically.

Designed for developers who want a clean and non-intrusive way to keep apps up to date.



## ✨ Features

- ✅ Header-only design — just include and use
- ✅ Modern C++11+ compatible
- ✅ Uses `nlohmann/json` for JSON parsing (included)
- ✅ No third-party libraries for networking (WinINet API)
- ✅ Full executable replacement with seamless restart
- ✅ Clean batch scripting for update execution
- ✅ Temp directory management
- ✅ Built-in logging and error handling
- ✅ Simple one-line update check

## 🧾 JSON Format (Update Metadata)

Your hosted version file should return:

```json
{
  "AppVersion": "1.1.0",
  "UpdateLink": "https://yourdomain.com/downloads/YourApp_v1.1.exe"
}


| Key          | Description                      |
| ------------ | -------------------------------- |
| `AppVersion` | The latest available version     |
| `UpdateLink` | Direct download link to the .exe |



## 🧰 Requirements

* Windows OS
* C++11 or later
* No installer needed
* Link with:

  * `wininet.lib`
  * `shell32.lib`



## 📂 Project Structure

```
ExecutableUpdaterCPP/
├── main.cpp                 # Example main entry
├── Updater/
│   ├── Updater.h            # Header-only updater implementation
│   └── json.hpp             # nlohmann/json single-header library
└── README.md                # This documentation
```



## 🚀 Usage Example (main.cpp)

cpp
#include <iostream>
#include "Updater/Updater.h"

int main() {
    const std::string currentVersion = "1.0";

    std::cout << "Starting application (v" << currentVersion << ")...\n";

    if (Updated(currentVersion)) {
        std::cout << "Update found and applied! Restarting...\n";
        return 0;
    } else {
        std::cout << "Application is up to date. Continuing...\n";
    }

    // Your normal application logic
    std::cout << "Running main functionality...\n";
    std::cin.get();
    return 0;
}
```



## 🔧 Integration Guide

### Step 1: Add Files

* Copy `Updater.h` and `json.hpp` into an `Updater/` folder in your project.
* Include the header:

  ```cpp
  #include "Updater/Updater.h"
  ```

### Step 2: Configure Version JSON URL

* Inside `Updater.h` or via `checkForUpdates()`:

  ```cpp
  #define AUTO_UPDATER_CONFIG_URL "https://yourdomain.com/version.json"
  ```

### Step 3: Link Libraries (Windows-only)

* Add `wininet.lib` and `shell32.lib` to your linker settings.


## 🧪 Testing

1. Host a valid `version.json` on your server.
2. Launch the application.
3. The updater compares versions and performs:

   * A download of the new `.exe`
   * File replacement via `.bat` script
   * Silent relaunch of the new version


## 🛡 Security Recommendations

* Host your JSON and executable on **HTTPS**
* Use file signatures or hashing (planned)
* Keep download links private if necessary


## 📜 License

Licensed under the [MIT License](LICENSE).
You are free to use, modify, and distribute with attribution.



## 👤 Author

**myexistences**
GitHub: [https://github.com/myexistences](https://github.com/myexistences)
Repo: [ExecutableUpdaterCPP](https://github.com/myexistences/ExecutableUpdaterCPP)



## 🙏 Contribute

Contributions, issues, and feature requests are welcome!

* Fork this repo
* Create a feature branch
* Submit a Pull Request


## 📊 Repository Statistics

![ViewCount](https://komarev.com/ghpvc/?username=myexistences\&label=Total+Views\&color=blue)

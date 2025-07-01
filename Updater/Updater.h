#ifndef AUTO_UPDATER_H
#define AUTO_UPDATER_H

#include <iostream>
#include <string> 
#include <fstream>
#include <windows.h>
#include <wininet.h>
#include <shlobj.h>
#include <process.h>
#include "../auth/json.hpp" // Ensure you have the nlohmann::json library available


// JSON structure expected from the URL:

/*{
    "UpdateLink": "https://YourWeb.com/Updated.exe",
    "AppVersion" : "1.0"
}*/


// int main example for testing the updater

/*int main() {
   
    
	//Updated("1.0");  // Call the Updated function with the current version

	// Example usage of the updater
    if (Updated("1.0")) {
        // Execute something if Update is Available
        std::cout << "Update found and applied!\n";
        // This code will execute but then the program will restart
        // so it's good for logging or cleanup before update
      

    }
    else {
        // Execute something if Update is not Available
        std::cout << "No update needed, continuing with normal execution...\n";
        
        // Your main program code here
        std::cout << "Program running normally...\n";
        // All your regular program logic goes here
    }

    return 0;
}*/

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "shell32.lib")

class AutoUpdater {
private:
    std::string updateUrl;
    std::string currentVersion;
    std::string tempDir;

    // Download file from URL
    bool downloadFile(const std::string& url, const std::string& filepath) {
        HINTERNET hInternet = InternetOpenA("AutoUpdater", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!hInternet) return false;

        HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (!hUrl) {
            InternetCloseHandle(hInternet);
            return false;
        }

        std::ofstream file(filepath, std::ios::binary);
        if (!file.is_open()) {
            InternetCloseHandle(hUrl);
            InternetCloseHandle(hInternet);
            return false;
        }

        char buffer[4096];
        DWORD bytesRead;

        while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
            file.write(buffer, bytesRead);
        }

        file.close();
        InternetCloseHandle(hUrl);
        InternetCloseHandle(hInternet);
        return true;
    }

    // Get version info from JSON
    std::string getRemoteVersion(const std::string& jsonUrl) {
        std::string tempFile = tempDir + "\\version.json";

        if (!downloadFile(jsonUrl, tempFile)) {
            return "";
        }

        std::ifstream file(tempFile);
        if (!file.is_open()) return "";

        try {
            nlohmann::json j;
            file >> j;
            file.close();
            DeleteFileA(tempFile.c_str());

            return j["AppVersion"].get<std::string>();
        }
        catch (const std::exception& e) {
            file.close();
            DeleteFileA(tempFile.c_str());
            return "";
        }
    }

    // Get update download link from JSON
    std::string getUpdateLink(const std::string& jsonUrl) {
        std::string tempFile = tempDir + "\\version.json";

        if (!downloadFile(jsonUrl, tempFile)) {
            return "";
        }

        std::ifstream file(tempFile);
        if (!file.is_open()) return "";

        try {
            nlohmann::json j;
            file >> j;
            file.close();
            DeleteFileA(tempFile.c_str());

            return j["UpdateLink"].get<std::string>();
        }
        catch (const std::exception& e) {
            file.close();
            DeleteFileA(tempFile.c_str());
            return "";
        }
    }

    // Get current executable path
    std::string getCurrentExePath() {
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        return std::string(path);
    }

    // Create batch file for replacement and execution
    void createUpdateBatch(const std::string& newExePath, const std::string& currentExePath) {
        std::string batchPath = tempDir + "\\update.bat";
        std::ofstream batch(batchPath);

        batch << "@echo off\n";
        batch << "timeout /t 2 /nobreak >nul\n";
        batch << "copy /Y \"" << newExePath << "\" \"" << currentExePath << "\"\n";
        batch << "start \"\" \"" << currentExePath << "\"\n";
        batch << "del \"" << newExePath << "\"\n";
        batch << "del \"%~f0\"\n";

        batch.close();

        // Execute batch file and exit current process
        ShellExecuteA(NULL, "open", batchPath.c_str(), NULL, NULL, SW_HIDE);
        ExitProcess(0);
    }

public:
    AutoUpdater(const std::string& updateUrl) : updateUrl(updateUrl) {
        // Get temp directory
        char tempPath[MAX_PATH];
        GetTempPathA(MAX_PATH, tempPath);
        tempDir = std::string(tempPath);

        // Remove trailing backslash if present
        if (tempDir.back() == '\\') {
            tempDir.pop_back();
        }
    }

    // Main update check function
    bool checkForUpdate(const std::string& currentVer) {
        currentVersion = currentVer;

        std::cout << "Checking for updates...\n";

        // Get remote version
        std::string remoteVersion = getRemoteVersion(updateUrl);
        if (remoteVersion.empty()) {
            std::cout << "Failed to check for updates.\n";
            return false;
        }

        std::cout << "Current version: " << currentVersion << "\n";
        std::cout << "Remote version: " << remoteVersion << "\n";

        // Compare versions
        if (currentVersion == remoteVersion) {
            std::cout << "Application is up to date.\n";
            return false;
        }

        std::cout << "Update available! Downloading...\n";

        // Get update download link
        std::string downloadLink = getUpdateLink(updateUrl);
        if (downloadLink.empty()) {
            std::cout << "Failed to get update download link.\n";
            return false;
        }

        // Download new version with temporary name
        std::string newExePath = tempDir + "\\temp_update.exe";
        if (!downloadFile(downloadLink, newExePath)) {
            std::cout << "Failed to download update.\n";
            return false;
        }

        std::cout << "Update downloaded successfully. Applying update...\n";

        // Create and execute update batch
        createUpdateBatch(newExePath, getCurrentExePath());

        return true;
    }
};

// Configuration - Set your pastebin raw URL here
#define UPDATE_CHECK_URL "https://pastebin.com/raw/VQuRawhK"

// Simple function interface for easy use - just pass version
inline bool Updated(const std::string& version) {
    AutoUpdater updater(UPDATE_CHECK_URL);
    return updater.checkForUpdate(version);
}

#endif // AUTO_UPDATER_H

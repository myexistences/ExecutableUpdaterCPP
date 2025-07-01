#include <iostream>
#include "Updater/Updater.h"  // Update the path as needed

int main() { 
    const std::string currentVersion = "1.0";

    std::cout << "Starting application (v" << currentVersion << ")...\n";

    if (Updated(currentVersion)) {
        // Executed right before the application exits for an update
        std::cout << "Update found and applied!\n";
        std::cout << "Restarting application with new version...\n";

        // You can do cleanup, save state, or log here if needed

        return 0; // The application will be restarted via batch script
    } else {
        // No update found — continue as normal
        std::cout << "No update needed, continuing with normal execution...\n";
        std::cout << "Program running normally...\n";

        // Your regular application logic here
        // Example:
        std::cout << "Hello from version " << currentVersion << "!\n";

        std::cin.get(); // Pause for demonstration
    }

    return 0;
}

// Helper utility to find sounds directory relative to executable
// This could be integrated into the main game code

#include <string>
#include <filesystem>

std::string GetSoundsPath() {
    // Try multiple possible paths
    std::vector<std::string> paths = {
        "sounds/",                    // Same directory as executable
        "../sounds/",                 // Parent directory
        "../Resources/sounds/",       // macOS app bundle Resources
        "./sounds/",                  // Current directory
        "resources/sounds/",          // Alternative
    };
    
    for (const auto& path : paths) {
        if (std::filesystem::exists(path)) {
            return path;
        }
    }
    
    return "sounds/"; // Default fallback
}


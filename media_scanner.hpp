#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct MediaFiles {
    std::vector<std::string> audio;
    std::vector<std::string> video;
    std::vector<std::string> images;
};

fs::path getHomeDirectory();

std::string toLower(std::string value);

bool isAudioFile(const std::string& extension);
bool isVideoFile(const std::string& extension);
bool isImageFile(const std::string& extension);

std::string makeRelativePath(
    const fs::path& filePath,
    const fs::path& basePath
);

MediaFiles scanDirectory(const fs::path& directoryPath);

std::string escapeJsonString(const std::string& value);

void writeJsonFile(
    const fs::path& outputPath,
    const MediaFiles& files
);

#include "media_scanner.hpp"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>

using namespace std;

fs::path getHomeDirectory() {
    const char* home = std::getenv("HOME");

    if (home == nullptr) {
        throw std::runtime_error("HOME environment variable is not set");
    }

    return fs::path(home);
}

// так как рашсрение файла, например, может быть .Mp3, .MP3, .mp3
// нужна функция которая будет расширение преобразовывать в универсальное
string toLower(string value) {
    for(char& c : value) {
        c = tolower(c);
    }
    return value;
}

bool containsExtension(const set<string>& extensions,
                       const string& extension) {
    return extensions.find(extension) != extensions.end();
}

bool isAudioFile(const string& extension) {
    static const set<string> audioExtensions = {
        ".mp3", ".wav", ".flac", ".ogg", ".aac", ".m4a"
    };

    return containsExtension(audioExtensions, extension);
}

bool isVideoFile(const string& extension) {
    static const set<string> videoExtensions = {
        ".mp4", ".avi", ".mkv", ".mov", ".wmv", ".webm"
    };

    return containsExtension(videoExtensions, extension);
}

bool isImageFile(const string& extension) {
    static const set<string> imageExtensions = {
        ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".webp", ".tiff"
    };

    return containsExtension(imageExtensions, extension);
}


string makeRelativePath(const fs::path& filePath, const fs::path& homePath) {
    error_code error;

    fs::path relativePath = fs::relative(filePath, homePath, error);

    if (error) {
        return filePath.generic_string();
    }

    return relativePath.generic_string();
}

MediaFiles scanDirectory(const fs::path& homePath) {
    MediaFiles result;

    error_code error;

    fs::recursive_directory_iterator iterator(
        homePath,
        fs::directory_options::skip_permission_denied,
        error
    );

    fs::recursive_directory_iterator end;

    while (iterator != end) {
        const fs::directory_entry& entry = *iterator;

        error_code entryError;

        if (entry.is_regular_file(entryError)) {
            fs::path filePath = entry.path();
            string extension = toLower(filePath.extension().string());
            string relativePath = makeRelativePath(filePath, homePath);

            if (isAudioFile(extension)) {
                result.audio.push_back(relativePath);
            } else if (isVideoFile(extension)) {
                result.video.push_back(relativePath);
            } else if (isImageFile(extension)) {
                result.images.push_back(relativePath);
            }
        }

        iterator.increment(error);

        if (error) {
            error.clear();
        }
    }

    return result;
}


void writeJsonArray(ofstream& file,
                    const string& name,
                    const vector<string>& values,
                    bool addComma) {
    
    file << "  \"" << name << "\": [\n";

    for (size_t i = 0; i < values.size(); ++i) {
        file << "    \"" << values[i] << "\"";

        if (i + 1 < values.size()) {
            file << ",";
        }

        file << "\n";
    }

    file << "  ]";

    if (addComma) {
        file << ",";
    }

    file << "\n";
}

void writeJsonFile(const fs::path& outputPath, const MediaFiles& files) {
    fs::path temporaryPath = outputPath;

    ofstream file(temporaryPath);

    if (!file.is_open()) {
        throw runtime_error("Cannot open output file");
    }

    file << "{\n";
    writeJsonArray(file, "audio", files.audio, true);
    writeJsonArray(file, "video", files.video, true);
    writeJsonArray(file, "images", files.images, false);
    file << "}\n";

    file.close();

    if (!file) {
        throw runtime_error("Cannot write output file");
    }

}

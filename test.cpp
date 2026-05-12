#include "media_scanner.hpp"
#include <iostream>
#include <unistd.h>


using namespace std;


int main(int argc, char* argv[]) {
    if (argc != 3) {
           cerr << "Usage: " << argv[0]
                << " <directory_path> <scan_interval_seconds>" << endl;
           return 1;
       }

       fs::path scanPath = argv[2];

       int scanIntervalSeconds;

       try {
           scanIntervalSeconds = stoi(argv[1]);
       } catch (const exception&) {
           cerr << "Scan interval must be a number" << endl;
           return 1;
       }

       if (scanIntervalSeconds <= 0) {
           cerr << "Scan interval must be positive" << endl;
           return 1;
       }

       if (!fs::exists(scanPath)) {
           cerr << "Directory does not exist: " << scanPath << endl;
           return 1;
       }

       if (!fs::is_directory(scanPath)) {
           cerr << "Path is not a directory: " << scanPath << endl;
           return 1;
       }

    try {
        fs::path homePath = getHomeDirectory();
        fs::path outputPath = homePath / ".media_files";

        cout << "Scanning directory: " << homePath << endl;
        cout << "Output file: " << outputPath << endl;
        cout << "Scan interval: " << scanIntervalSeconds << " seconds" << endl;

        while (true) {
            MediaFiles files = scanDirectory(homePath);
            writeJsonFile(outputPath, files);

            cout << "Media file list updated" << endl;

            
            sleep(scanIntervalSeconds);
        }
    } catch (const exception& exception) {
        cerr << "Error: " << exception.what() << endl;
        return 1;
    }

    return 0;
}

/*
 
 6
 2
 7
 3
 
 6
 1
 7
 1
 
 
 
 */

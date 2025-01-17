#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>  
#include <sys/stat.h> 
#include <algorithm> 
using namespace std;

string trim(const string &str) {
    auto start = str.find_first_not_of(" \t");
    auto end = str.find_last_not_of(" \t");
    return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
}

bool fileExists(const string &filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

bool createDirectory(const string &directory) {
    struct stat info;
    if (stat(directory.c_str(), &info) != 0) {
        return system(("mkdir -p " + directory).c_str()) == 0;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

void extractLogs(const string &inputFile, const string &outputDir, const string &targetDate) {
    if (!fileExists(inputFile)) {
        cerr << "Error: Input file '" << inputFile << "' does not exist." << endl;
        exit(EXIT_FAILURE);
    }

    if (!createDirectory(outputDir)) {
        cerr << "Error: Failed to create output directory '" << outputDir << "'." << endl;
        exit(EXIT_FAILURE);
    }

    string outputFile = outputDir + "/output_" + targetDate + ".txt";

    ifstream infile(inputFile);
    ofstream outfile(outputFile);

    if (!outfile.is_open()) {
        cerr << "Error: Could not open output file '" << outputFile << "'." << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    size_t lineCount = 0;

    while (getline(infile, line)) {
        line = trim(line); 
        if (line.rfind(targetDate, 0) == 0) { 
            outfile << line << "\n";
            lineCount++;
        }
    }

    infile.close();
    outfile.close();
    cout << "Extraction complete. " << lineCount << " log entries written to " << outputFile << endl;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: ./extract_logs <input_file> <YYYY-MM-DD>" << endl;
        return EXIT_FAILURE;
    }

    string inputFile = argv[1];
    string targetDate = argv[2];

    if (targetDate.size() != 10 || targetDate[4] != '-' || targetDate[7] != '-') {
        cerr << "Error: Date must be in the format YYYY-MM-DD." << endl;
        return EXIT_FAILURE;
    }

    string outputDir = "output";
    extractLogs(inputFile, outputDir, targetDate);

    return EXIT_SUCCESS;
}

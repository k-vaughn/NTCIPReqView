#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <string>
#include <vector>

// Function to read the entire content of a file into a string
std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for reading");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Function to write a string to a file
void writeFile(const std::string& filePath, const std::string& content) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing");
    }
    file << content;
}

// Function to process and replace the pattern in the content
std::string processContent(const std::string& content) {
    std::regex pattern(R"(<p>INTEGER\s*\{\s*([^}]*)\s*\}\s*</p>)");
    std::smatch match;
    std::string result = content;
    std::string::const_iterator searchStart(result.cbegin());
    int loop = 1;

    while (std::regex_search(result.cbegin(), result.cend(), match, pattern)) {
        std::string itemList = match[1];
        
        // Split the itemList into individual items
        std::regex itemPattern(R"(\s*(\w+\s*\(\s*\d+\s*\))\s*,?)");
        std::vector<std::string> items;
        std::sregex_iterator iter(itemList.begin(), itemList.end(), itemPattern);
        std::sregex_iterator end;
        while (iter != end) {
            items.push_back(iter->str());
            ++iter;
        }

        // Create the revItemList
        std::ostringstream revItemList;
        for (size_t i = 0; i < items.size(); ++i) {
            if (i != 0) {
                revItemList << "\\n   ";
            }
            revItemList << items[i];
        }

        // Replace the matched pattern with the revItemList
        std::string replacement = "<pre>INTEGER {\\n    " + revItemList.str() + " }</pre>";
        result.replace(match.position(), match.length(), replacement);
        searchStart = result.begin() + match.position() + replacement.length();
        std::cout << "Loop" << std::to_string(loop++) << ", " << std::to_string(match.position()) << std::endl;
    }

    return result;
}

int main() {
    std::string inputFile = "/Users/kvaughn/GitHub/ITE/NTCIPReqView/documents/NTCIP_1204.json";
    std::string outputFile = "/Users/kvaughn/Documents/Standards/NTCIP/output/NTCIP_1204.json";

    try {
        // Read the file content
        std::string content = readFile(inputFile);

        // Process the content to replace patterns
        std::string processedContent = processContent(content);

        // Write the modified content to the output file
        writeFile(outputFile, processedContent);

        std::cout << "File processing complete. Modified content saved to " << outputFile << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

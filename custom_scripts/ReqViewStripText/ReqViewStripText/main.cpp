//
//  main.cpp
//  ReqViewStripText
//
//  Created by Kenneth Vaughn on 7/16/24.
/* This program is a C++ program that creates a copy of the "documents" folder but removes the "text" and "attachment" fields from the "data" field of each document and moves the name component to the heading.  */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


using namespace std;

string path = "/Users/kvaughn/Documents/Standards/ISO/TC204 WG9/26048 - NTCIP Data/ReqView/";

// Recursive function to remove all "history" components
void RemoveDetails(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
    if (value.IsObject()) {
        string text;
        // Remove any "history" component
        if (value.HasMember("history")) {
            value.RemoveMember("history");
        }
        // Check if the "text" component exists and replace it with empty string
        if (value.HasMember("text")) {
            value["text"].SetString("", allocator);
        }
        // Check if the "name" component exists and, if so, copy to heading
        if (value.HasMember("name") && value.HasMember("heading")) {
            if (value["name"].IsString()) {
                text = value["name"].GetString();
                value["heading"].SetString(text.c_str(), allocator);
                value.RemoveMember("name");
            }
        } else if (value.HasMember("name")) {
            if (value["name"].IsString()) {
                text = value["name"].GetString();
                rapidjson::Value headingKey("heading", allocator);
                rapidjson::Value headingValue("value", allocator);
                value.AddMember(headingKey, headingValue, allocator);
                value.RemoveMember("name");
            }
        }
        // Remove any attachments
        if (value.HasMember("attachments")) {
            value.RemoveMember("attachments");
        }
        // loop through children
        for (auto& member : value.GetObject()) {
            RemoveDetails(member.value, allocator);
        }
    } else if (value.IsArray()) {
        for (auto& item : value.GetArray()) {
            RemoveDetails(item, allocator);
        }
    }
}

int main(int argc, const char * argv[]) {
    ifstream inputFile(path + "project.json");
    if (!inputFile.is_open()) {
        cerr << "Failed to open project file." << endl;
        return 1;
    }
    stringstream buffer;
    string filename;
    rapidjson::Document reqviewdoc;

    // Create a JSON document and parse the file
    rapidjson::IStreamWrapper isw(inputFile);
    rapidjson::Document doc;
    doc.ParseStream(isw);
    inputFile.close();

    // Check if "documents" exists and is an array
    if (doc.HasMember("documents") && doc["documents"].IsArray()) {
        const rapidjson::Value& documents = doc["documents"];
        // Iterate through the "documents" array
        for (rapidjson::SizeType i = 0; i < documents.Size(); i++) {
            if (documents[i].IsString()) {
                // Process each document file referenced by the project file
               filename = documents[i].GetString();
                
                // Open docuemnt file
                ifstream document(path + "documents/" + filename);
                if (!document.is_open()) {
                    std::cerr << "Failed to open document file." << path + "document/" + filename << std::endl;
                    return 2;
                }

                // Create a JSON document and parse the file
                rapidjson::IStreamWrapper rvisw(document);
                reqviewdoc.ParseStream(rvisw);
                document.close();
                
                RemoveDetails(reqviewdoc, reqviewdoc.GetAllocator());
                
                // Open the file for writing
                ofstream outputFile(path + "rev/" + filename);
                if (!outputFile.is_open()) {
                    cerr << "Could not open the file for writing: " << path + "rev/" + filename << endl;
                    return 3;
                }
                
                // Convert the modified DOM back to a JSON string
                rapidjson::StringBuffer buffer;
                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                reqviewdoc.Accept(writer);

                // Write the modified content back to the file
                outputFile << buffer.GetString() << std::endl;
                outputFile.close();

                std::cout << documents[i].GetString() << " processing complete..." << std::endl;
            }
        }
    } else {
        std::cerr << "\"documents\" key is missing or not an array" << std::endl;
    }
    
    return 0;
}

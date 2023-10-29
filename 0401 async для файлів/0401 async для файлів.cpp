#include <iostream>
#include <fstream>
#include <string>
#include <future>
#include <vector>


int createFileAndCountCharacters(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << content;
        file.close();
    }
    else {
        std::cerr << "ERROR: Не вдалося створити або записати файл " << filename << std::endl;
        return 0;
    }

    return content.length();
}

int main() {
    system("chcp 1251>nul");
    std::vector<std::pair<std::string, std::string>> filesContent = {
        {"txt1.txt", "Текст для 1 файлу"},
        {"txt2.txt", "Текст для 2 файлу"},
        {"txt3.txt", "Текст для 3 файлу"}
    };

    std::vector<std::future<int>> results;

    for (const auto& fileContent : filesContent) {
        results.push_back(std::async(std::launch::async, createFileAndCountCharacters, fileContent.first, fileContent.second));
    }

    int totalCharacterCount = 0;
    for (size_t i = 0; i < results.size(); i++) {
        int characterCount = results[i].get();
        if (characterCount > 0) {
            std::cout << "Файл " << filesContent[i].first << " містить " << characterCount << " символів." << std::endl;

            std::ifstream file(filesContent[i].first);
            if (file.is_open()) {
                std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                std::cout << "Вміст файлу " << filesContent[i].first << ":" << std::endl << fileContent << std::endl;
                file.close();
            }
            else {
                std::cerr << "ERROR: Не вдалося відкрити файл " << filesContent[i].first << " для читання." << std::endl;
            }

            totalCharacterCount += characterCount;
        }
    }

    std::cout << "Загальний розмір файлів: " << totalCharacterCount << " символів." << std::endl;

    return 0;
}

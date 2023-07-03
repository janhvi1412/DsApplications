#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <bitset>

struct HuffmanNode {
    char data;
    unsigned frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, unsigned frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(HuffmanNode* lhs, HuffmanNode* rhs) {
        return lhs->frequency > rhs->frequency;
    }
};

void calculateFrequency(std::ifstream& inFile, std::unordered_map<char, unsigned>& frequencyTable) {
    char ch;
    while (inFile.get(ch)) {
        ++frequencyTable[ch];
    }
}

HuffmanNode* buildHuffmanTree(const std::unordered_map<char, unsigned>& frequencyTable) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;

    for (const auto& entry : frequencyTable) {
        pq.push(new HuffmanNode(entry.first, entry.second));
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top();
        pq.pop();

        HuffmanNode* right = pq.top();
        pq.pop();

        HuffmanNode* parent = new HuffmanNode('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    return pq.top();
}

void generateCodes(HuffmanNode* root, const std::string& code, std::unordered_map<char, std::string>& codeTable) {
    if (root == nullptr) {
        return;
    }

    if (root->data != '\0') {
        codeTable[root->data] = code;
    }

    generateCodes(root->left, code + '0', codeTable);
    generateCodes(root->right, code + '1', codeTable);
}

void compressDOC(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile) {
        std::cerr << "Failed to open input file: " << inputFile << std::endl;
        return;
    }

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to create output file: " << outputFile << std::endl;
        inFile.close();
        return;
    }

    std::unordered_map<char, unsigned> frequencyTable;
    calculateFrequency(inFile, frequencyTable);
    inFile.clear();
    inFile.seekg(0);

    HuffmanNode* root = buildHuffmanTree(frequencyTable);
    std::unordered_map<char, std::string> codeTable;
    generateCodes(root, "", codeTable);

    // Write frequency table to output file
    unsigned int numSymbols = frequencyTable.size();
    outFile.write(reinterpret_cast<const char*>(&numSymbols), sizeof(numSymbols));

    for (const auto& entry : frequencyTable) {
        char symbol = entry.first;
        outFile.write(reinterpret_cast<const char*>(&symbol), sizeof(symbol));

        unsigned int frequency = entry.second;
        outFile.write(reinterpret_cast<const char*>(&frequency), sizeof(frequency));
    }

    // Compress the data
    std::string encodedData;
    char ch;
    while (inFile.get(ch)) {
        encodedData += codeTable[ch];
    }

    // Pad the encoded data to ensure a multiple of 8 bits
    unsigned int paddingBits = 8 - encodedData.size() % 8;
    for (unsigned int i = 0; i < paddingBits; ++i) {
        encodedData += '0';
    }

    // Write padding bits and size of encoded data to output file
    outFile.write(reinterpret_cast<const char*>(&paddingBits), sizeof(paddingBits));

    unsigned int encodedDataSize = encodedData.size();
    outFile.write(reinterpret_cast<const char*>(&encodedDataSize), sizeof(encodedDataSize));

    // Write the encoded data to output file
    std::bitset<8> byte;
    for (unsigned int i = 0; i < encodedData.size(); ++i) {
        byte[i % 8] = encodedData[i] - '0';

        if ((i + 1) % 8 == 0) {
            char encodedByte = static_cast<char>(byte.to_ulong());
            outFile.write(reinterpret_cast<const char*>(&encodedByte), sizeof(encodedByte));
            byte.reset();
        }
    }

    inFile.close();
    outFile.close();

    std::cout << "File compressed successfully!" << std::endl;
}

int main() {
    std::string inputFile = "input.doc";
    std::string compressedFile = "compressed.doc";

    compressDOC(inputFile, compressedFile);

    return 0;
}

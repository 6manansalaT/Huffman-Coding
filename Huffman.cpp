#include "Huffman.h"

// this function enters elements of a hashmap into nodes
Node* Huffman::HuffmanBuildTree() {
    unordered_map<char, int> charTable = charDictionary;

    priority_queue<Node *, vector<Node *>, compareWeights> charNodes = HuffmanNodes;
    for (auto x : charTable) {
        Node* charNode = new Node(x.first, x.second);
        charNodes.push(charNode);
    }
    while (charNodes.size() > 1) {
        Node* left = charNodes.top();
        charNodes.pop();
        Node* right = charNodes.top();
        charNodes.pop();

        int parentWeight = right->weight + left->weight;
        Node* parent = new Node ('\0', parentWeight, left, right);
        charNodes.push(parent);//Enqueue parent into nodes
    }
    return charNodes.top();
}

unordered_map<char, string> Huffman::GetCodes(Node* CodeNode, string binary, unordered_map<char, string> codeTable) {
    if (CodeNode->zero == nullptr && CodeNode->one == nullptr) {
        codeTable[CodeNode->letter] = binary;
    } else {
        codeTable = GetCodes(CodeNode->zero, binary + "0", codeTable);
        codeTable = GetCodes(CodeNode->one, binary + "1", codeTable);
    }
    compressDictionary = codeTable;
    return codeTable;
}

void Huffman::getCountsfromFile(string originalFile) {
    ifstream originalText;
    char currentChar;
    originalText.open(originalFile);
    if (originalText) {
        char currentCharacter;
        while (originalText.get(currentChar)) {
            currentCharacter = currentChar;
            if (charDictionary.find(currentCharacter) != charDictionary.end()) {
                charDictionary[currentCharacter] += 1;
            } else {
                charDictionary[currentCharacter] = 1;
            }
        }
        currentCharacter = '\3';
        charDictionary[currentCharacter] = 1;
        originalText.close();
    }
    else {
        cout << "Error opening the file" << endl;
    }
    originalText >> currentChar;
}

void Huffman::createBinaryFile(string compressedFile, string originalFile)
{
    // Create a new storage object
    Storage *storage = new Storage();

    // Open a StorageDriverTest.txt for writing
    std::string compressFile= compressedFile;

    if (!storage->open(compressFile, "write")) {
        std::cout << "There was an error opening the file." << std::endl;
        exit(0);
    }

    string charWithCodes;
    for (auto x : compressDictionary) {
        charWithCodes += x.first + x.second + '\30';
    }

    storage->setHeader(charWithCodes);

    ifstream bookText;
    char currentChar;
    bookText.open(originalFile);
    if (bookText) {
        std::string binary_string = "";
        char currentCharacter;
        while (bookText.get(currentChar)) {
            binary_string = "";
            currentCharacter = currentChar;
            if (codeMap.find(currentCharacter) != codeMap.end()) {
                binary_string += codeMap.at(currentChar);
                // insert a binary string into the storage object
                storage->insert(binary_string);
            }
        }
        binary_string = "";
        binary_string += codeMap.at('\3');
        // insert a binary string into the storage object
        storage->insert(binary_string);
        bookText.close();
    }
    else {
        cout << "Error opening the file" << endl;
    }
    bookText >> currentChar;
    
    storage->close();
}

string Huffman::HuffmanCompress(string originalFile, string compressFile) {
    getCountsfromFile(originalFile);
    treeRoot = HuffmanBuildTree();
    codeMap = GetCodes(treeRoot, "", compressDictionary);
    createBinaryFile(compressFile, originalFile);
    return compressFile;

}

void Huffman::createDecompressedFile(string binaryFile, string decompressFile) {
    // Create a new storage object
    Storage *storage = new Storage();

    // Open a StorageDriverTest.txt for writing
    std::string compressFile= binaryFile;

    // open a file for reading
    if (!storage->open(compressFile, "read")) {
        std::cout << "There was an error opening the file." << std::endl;
        exit(0);
    };

    header  = storage->getHeader();

    HuffmanRebuildTree();

    // extract all binary string 8 bits at a time until there is nothing left to read
    ofstream outputText;
    outputText.open(decompressFile);
    if (!outputText) {
        cout << "Error opening the file" << endl;
        exit(2);
    } else {
        // prep a string variable to pass by reference
        std::string binaryBits;
        string remainingBits = "";
        bool check = false;
        while (storage->extract(binaryBits)) {
            binaryBits = remainingBits + binaryBits;
            Node* traverseNode = treeRoot;
            int checkpoint = 0;
            for (int i = 0; i < binaryBits.length(); i++) {
                if (binaryBits[i] == '0') {
                    traverseNode = traverseNode->zero;
                } else {
                    traverseNode = traverseNode->one;
                }
                if (traverseNode->zero == nullptr && traverseNode->one == nullptr) {
                    if (traverseNode->letter == '\3') {
                        check = true;
                        break;
                    }
                    checkpoint = i;
                    char decompressedChar = traverseNode->letter;
                    outputText << decompressedChar;
                    traverseNode = treeRoot;
                }
            }
            if (check) {
                break;
            }
            // change to write at the same time
            remainingBits = binaryBits.substr(checkpoint + 1);
        }
    }
}

void Huffman::HuffmanRebuildTree() {
    treeRoot = new Node('\0', 0);
    Node *newNode = treeRoot;
    int counter = 0;
    char saveLetter = 'a';
    for (char currentChar : header) {
        if (counter == 0) {
            saveLetter = currentChar;
            counter++;
            continue;
        }
        if (currentChar == '0' && counter != 0) {
            if (newNode->zero == nullptr) {
                Node* newLeaf = new Node('\0', 0);
                newNode->zero = newLeaf;
            }
            newNode = newNode->zero;
            counter++;
        } else if (currentChar == '1' && counter != 0) {
            if (newNode->one == nullptr) {
                Node* newLeaf = new Node('\0', 0);
                newNode->one = newLeaf;
            }
            newNode = newNode->one;
            counter++;
        } else if (currentChar == '\30' && counter != 0) {
            newNode->letter = saveLetter;
            newNode = treeRoot;
            counter = 0;
        }
    }
}

void Huffman::HuffmanDecompress(string binaryFile, string decompressedFile) {
    createDecompressedFile(binaryFile, decompressedFile);
}

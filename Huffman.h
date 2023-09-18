#include <string>
#include <queue>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <bitset>
#include "Node.h"
#include "Storage/Storage.h"
using namespace std;

#ifndef HUFFMAN_H
#define HUFFMAN_H
/**
 * A class named Huffman which gets character counts from a file, uses those
 * counts to make a binary tree, and encodes the file into binary with that
 * tree and puts it into a new file. The class also decompresses that file by
 * taking the header and using it to reconstruct the binary tree that was made
 * earlier. Then, by reading the file and traversing through that binary tree,
 * the contents of the original file are recreated.
 */
class Huffman{
public:
    /**
     * This function uses an unordered map which consists of characters and
     * their frequencies for which they show up in the file. With this map,
     * a priority queue of nodes are created which are sorted by their
     * frequency. This priority queue is used to create a binary tree where
     * these nodes are at the leaves of the tree. The nodes' parents are empty
     * nodes with weights of the combined leaves. These parents are pushed onto
     * the priority queue until there is one node left, or all the nodes are in
     * one tree.
     * @return nodes.top()
     */
    Node* HuffmanBuildTree();
    /**
     * This function uses the binary tree. This function traverses the binary
     * tree and creates a new map of the characters and the codes that are
     * gained from traversing the tree. These codes will be binary codes used
     * for encoding the file.
     * @param CodeNode
     * @param binary
     * @param codeTable
     * @return codeTable
     */
    unordered_map<char, string> GetCodes (Node* CodeNode, string binary, unordered_map<char, string> codeTable);
    /**
     * Calls the getCountsfromFile function and passes the file path of the
     * original file. Then it sets the treeRoot variable to the result of
     * calling the HuffmanBuildTree function. Then the codes variable is set to
     * the result of calling the GetCodes function, while passing the binary
     * tree, an empty string, and an empty map. Then the createBinaryFile
     * function is called, passing in the paths of the original files and the
     * file that the compressed content should go into. Then the function
     * returns the compressed file's file path.
     * @param originalFile
     * @param compressFile
     * @return compressFile
     */
    string HuffmanCompress(string originalFile, string compressFile);
    /**
     * Calls the createDecompressedFile function, passing in the paths for the
     * compressed file and the file where the decompressed content is going to
     * go.
     * @param binaryFile
     * @param decompressedFile
     */
    void HuffmanDecompress(string binaryFile, string decompressedFile);
    /**
     * Opens the original file, reads each character. For every character
     * found for the first time, it is added into the map and the frequency
     * count for it would be one. For each time the character is found again,
     * the frequency count would increment. Then a /3 character is appended to
     * the end of the file with a frequency of 1. The file is closed. There is
     * a way to notify the user if the file cannot open for some reason.
     * @param originalFile
     */
    void getCountsfromFile(string originalFile);
    /**
     * Creates a storage object, then the function opens the file for the
     * compressed contents. A header is then created for this file, which is
     * made by using the map containing the characters and their respective
     * binary codes. In between each character and code pair, there is a /30 in
     * between. Then the function opens the original file and matches each
     * character found with their codes. These codes are then put into the
     * compressed file. At the end of the file, the binary code for /3 is put
     * and the original file closes. There is a checker just in case the file
     * does not open. The compressed file closes.
     * @param compressedFile
     * @param originalFile
     */
    void createBinaryFile(string compressedFile, string originalFile);
    /**
     * A new storage object is created. The compressed file is read and first
     * the header is received. The HuffmanRebuildTree function is called, and
     * the file where the decompressed contents are going to go is opened.
     * There is a checker to let the user know if there is an error opening the
     * file. Then the characters are found by using the tree and reading the
     * compressed file at the same time. What is extracted from the binary file
     * is used to traverse the tree, and if it reaches a leaf, then the
     * character at that leaf is printed into the decompressed file. There is
     * also a remainingBits string that adds itself to the front of the next
     * bits extracted, in the case that there are extra bits from the eight
     * that were extracted that couldn't reach a leaf.
     * @param binaryFile
     * @param decompressFile
     */
    void createDecompressedFile(string binaryFile, string decompressFile);
    /**
     * This function reads the header string and stores the first character
     * before its binary codes as a new node's letter variable. Then new nodes
     * are created using the binary codes, and leaves are put in the same
     * places as they were during compression. I used a counter system to make
     * sure that the only criteria that a character needed to pass was that it
     * was either the first character of the header or it was immediately after
     * the /30.
     * @return
     */
    void HuffmanRebuildTree();
private:
    Node* treeRoot;                                                          // a Node called treeRoot
    std::priority_queue<Node*, vector<Node*>, compareWeights> HuffmanNodes;  // a priority queue called HuffmanNodes
    std::unordered_map<char, int> charDictionary;                                // an unordered map called charDictionary
    std::unordered_map<char, string> compressDictionary;                     // an unordered map called compressDictionary
    unordered_map<char, string> codeMap;                                       // an unordered map called codes
    string header;                                                           // a string called header
};

#endif //HUFFMAN_H

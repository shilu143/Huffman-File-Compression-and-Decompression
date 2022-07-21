#include<bits/stdc++.h>
using namespace std;

struct Node {
    char ch;
    int freq;
    Node* left, * right;
};

struct comparator {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

bool checkBit(vector<char>& encodedData, int idx) {
    char c = encodedData[idx / 8];
    return c & (1 << (7 - (idx % 8)));
}

void __encode(Node* root, string str, unordered_map<char, string>& huffmanCode) {
    if (root == nullptr) {
        return;
    }
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    __encode(root->left, str + "0", huffmanCode);
    __encode(root->right, str + "1", huffmanCode);
}

void __decode(Node* root, int& idx, vector<char>& encodedData, fstream& decodedFile, int constraint) {
    if (root == nullptr || idx >= constraint) {
        return;
    }
    if (!root->left && !root->right) {
        decodedFile << root->ch;
        return;
    }
    idx++;
    if (checkBit(encodedData, idx))
        __decode(root->right, idx, encodedData, decodedFile, constraint);
    else
        __decode(root->left, idx, encodedData, decodedFile, constraint);
}

class Huffman {
private:
    fstream inputFile, outputFile, decodedFile;
    string originalData;
    vector<char> encodedbits;
    int topIdx, constraint;
    Node* root;

public:
    Huffman() {
        
#ifdef _WIN32 // Includes both 32 bit and 64 bit
        inputFile.open("originalData\input.txt", ios::in);
        outputFile.open("compressedFile\output.dat", ios::out | ios::binary);
        decodedFile.open("decodedFile\decodedfile.txt", ios::out);
#else
        inputFile.open("originalData/input.txt", ios::in);
        outputFile.open("compressedFile/output.dat", ios::out | ios::binary);
        decodedFile.open("decodedFile/decodedfile.txt", ios::out);
#endif

        originalData = "";
        topIdx = 8;
        constraint = 0;
        root = nullptr;

        string line;
        if (inputFile.is_open())
        {
            while (getline(inputFile, line))
            {
                originalData += line;
            }
            inputFile.close();
        }
        else {
            cout << "\nUnable to open the input file\n";
        }
    }
    Node* createNode(char ch, int freq, Node* lft, Node* rgt) {

        Node* node = new Node();
        node->ch = ch;
        node->freq = freq;
        node->left = lft;
        node->right = rgt;

        return node;
    }

    void buildHuffmanTree() {
        unordered_map<char, int> freqMp;

        for (auto ch : originalData) {
            freqMp[ch]++;
        }

        priority_queue<Node*, vector<Node*>, comparator> minHeap;

        for (auto pair : freqMp) {
            Node* node = createNode(pair.first, pair.second, nullptr, nullptr);
            minHeap.push(node);
        }

        while (minHeap.size() > 1) {
            Node* left = minHeap.top();
            minHeap.pop();
            Node* right = minHeap.top();
            minHeap.pop();

            int sum = left->freq + right->freq;
            Node* node = createNode('\0', sum, left, right);
            minHeap.push(node);
        }

        root = minHeap.top();
    }

    void calc(string s) {
        for (auto ch : s) {
            if (topIdx == 8) {
                topIdx = 0;
                encodedbits.push_back(0);
            }
            int n = encodedbits.size();
            encodedbits[n - 1] <<= 1;
            if (ch == '1') {
                encodedbits[n - 1]++;
            }
            topIdx++;
        }
    }

    void HuffmanEncode() {
        unordered_map<char, string> huffmanCode;

        __encode(root, "", huffmanCode);

        for (auto ch : originalData) {
            // str += huffmanCode[ch];
            calc(huffmanCode[ch]);
        }

        int n = encodedbits.size();
        encodedbits[n - 1] <<= (8 - topIdx);
        constraint = (encodedbits.size() - 1) * 8 + topIdx;
        encodeInFile();
    }
    void HuffmanDecode() {
        int idx = -1;
        __decode(root, idx, encodedbits, decodedFile, constraint);
        while (idx < constraint) {
            __decode(root, idx, encodedbits, decodedFile, constraint);
        }
        decodedFile.close();
    }
    void encodeInFile() {
        for (auto x : encodedbits) {
            outputFile << x;
        }
        outputFile.close();
    }

    // ~Huffman() {
    //     inputFile.close();
    //     outputFile.close();
    // }

};


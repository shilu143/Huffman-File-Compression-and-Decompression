#include<bits/stdc++.h>
using namespace std;

//Huffman Tree
struct Node {
    char ch;
    int freq;
    Node* left, * right;
};
vector<char> encodedbits;
int topidx = 8, constraint = 0;
void encodeInFile() {
    ofstream output("output.huff", ios::binary);
    for (auto x : encodedbits) {
        output << x;
    }
    output.close();
}

bool checkBit(int idx) {
    char c = encodedbits[idx / 8];
    return c & (1 << (7 - (idx % 8)));
}

void calc(string s) {
    for (auto ch : s) {
        if (topidx == 8) {
            topidx = 0;
            encodedbits.push_back(0);
        }
        int n = encodedbits.size();
        encodedbits[n - 1] <<= 1;
        if (ch == '1') {
            encodedbits[n - 1]++;
        }
        topidx++;
    }
}


Node* getNode(char ch, int freq, Node* left, Node* right) {
    Node* node = new Node();
    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

struct comp {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};
void encode(Node* root, string str, unordered_map<char, string>& huffmanCode) {
    if (root == nullptr) {
        return;
    }
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

void decode(Node* root, int& idx) {
    if (root == nullptr || idx >= constraint) {
        return;
    }
    if (!root->left && !root->right) {
        cout << root->ch;
        return;
    }
    idx++;
    if (checkBit(idx))
        decode(root->right, idx);
    else
        decode(root->left, idx);
}

void buildHuffmanTree(string text) {
    unordered_map<char, int> freq;
    for (char ch : text) {
        freq[ch]++;
    }

    priority_queue<Node*, vector<Node*>, comp> pq;

    for (auto pair : freq) {
        pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
    }


    while (pq.size() != 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();

        int sum = left->freq + right->freq;
        pq.push(getNode('\0', sum, left, right));
    }

    Node* root = pq.top();

    unordered_map<char, string> huffmanCode;
    encode(root, "", huffmanCode);


    cout << "Huffman Codes are :\n" << "\n";
    for (auto pair : huffmanCode) {
        cout << pair.first << " " << pair.second << "\n";
    }

    cout << "\nOriginal string was :\n" << text << "\n";

    string str = "";
    for (char ch : text) {
        str += huffmanCode[ch];
        calc(huffmanCode[ch]);
    }
    encodedbits[encodedbits.size() - 1] <<= (8 - topidx);
    constraint = (encodedbits.size() - 1) * 8 + topidx;
    cout << constraint << "\n";
    cout << "\nEncoded string is :\n" << str << "\n";

    int idx = -1;
    cout << "\nDecoded string is: \n";
    decode(root, idx);
    while (idx < constraint) {
        decode(root, idx);
    }
}
int main() {
    string line, text = "";
    ifstream myfile("input.txt");
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            text += line;
        }
        myfile.close();
    }
    else cout << "Unable to open file";

    // string text = "shilu";
    buildHuffmanTree(text);
    cout << "\nEncodedBits Size = " << encodedbits.size() << "\n";
    encodeInFile();
    for (auto x : encodedbits) {
        for (int i = 0;i < 8;i++) {
            char temp = x >> (7 - (i));
            cout << (temp & 1);
        }
    }
    cout << "\n";
    cout << topidx << "\n";
    return 0;
}
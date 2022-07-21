#include "includes/Huffman.h"

int main(int argc, char* argv[]) {
    Huffman Huff;

    Huff.buildHuffmanTree();
    Huff.HuffmanEncode();
    Huff.HuffmanDecode();

    return 0;
}
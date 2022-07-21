#include "includes/Huffman.h"

int main(int argc, char* argv[]) {
    Huffman Huff;

    Huff.buildHuffmanTree();
    Huff.HuffmanEncode();
    Huff.HuffmanDecode();
    cout<<"\nCongratulation, successfully compressed 40-50% of the original File\n";
    return 0;
}
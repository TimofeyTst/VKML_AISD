#include <iostream>
#include <fstream>
#include "Huffman.h"
#include <vector>
#include <bitset>

typedef char byte;

class IInputStream {
public:
    virtual bool Read(byte& value) = 0;
    virtual ~IInputStream() {} // ��������� ����������� ����������
};


class IOutputStream {
public:
    virtual void Write(byte value) = 0;
    virtual ~IOutputStream() {} // ��������� ����������� ����������
};

class FileInputStream : public IInputStream {
private:
    std::ifstream input;

public:
    FileInputStream(const std::string& filename) {
        input.open(filename, std::ios::binary);
    }

    bool Read(byte& value) override {
        return input.read(reinterpret_cast<char*>(&value), sizeof(value)).good();
    }
};

class FileOutputStream : public IOutputStream {
private:
    std::ofstream output;

public:
    FileOutputStream(const std::string& filename) {
        output.open(filename, std::ios::binary);
    }

    void Write(byte value) override {
        output.write(reinterpret_cast<char*>(&value), sizeof(value));
    }
};

void Encode(IInputStream& original, IOutputStream& compressed) {
    // ������� ������� �������� �� ������� ������
    std::map<byte, int> frequencies;
    byte value;
    while (original.Read(value)) {
        frequencies[value]++;
    }

    // ������� ������ �������� � ������� �����
    HuffmanTree huffman;
    huffman.buildTree(frequencies);
    std::map<byte, std::string> codes = huffman.getCodes();

    // ��������� ������� ����� � ������
    std::bitset<8> bit;
    for (auto& code : codes) {
        // ��������� ������
        compressed.Write(code.first);
        // ��������� ����� ����
        int codeLength = code.second.length();
        compressed.Write(static_cast<byte>(codeLength));
        // ��������� ���
        for (int i = 0; i < codeLength; i++) {
            bit.set(7 - (i % 8), code.second[i] == '1');
            if (i % 8 == 7) {
                compressed.Write(static_cast<byte>(bit.to_ulong()));
                bit.reset();
            }
        }
        if (codeLength % 8 != 0) {
            compressed.Write(static_cast<byte>(bit.to_ulong()));
            bit.reset();
        }
    }

    // ������� ������� ����� � ������� ������� ����� � ��������� � ������
    original = FileInputStream("input_file");
    std::string encodedData;
    while (original.Read(value)) {
        encodedData += codes[value];
    }
    int encodedDataLength = encodedData.length();
    compressed.Write(static_cast<byte>((encodedDataLength >> 24) & 0xFF));
    compressed.Write(static_cast<byte>((encodedDataLength >> 16) & 0xFF));
    compressed.Write(static_cast<byte>((encodedDataLength >> 8) & 0xFF));
    compressed.Write(static_cast<byte>(encodedDataLength & 0xFF));
    for (int i = 0; i < encodedDataLength; i++) {
        bit.set(7 - (i % 8), encodedData[i] == '1');
        if (i % 8 == 7) {
            compressed.Write(static_cast<byte>(bit.to_ulong()));
            bit.reset();
        }
    }
    if (encodedDataLength % 8 != 0) {
        compressed.Write(static_cast<byte>(bit.to_ulong()));
    }
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    // ��������� ������� ������ �� �������� ������
    std::map<byte, int> frequencies;
    int size = 0;
    byte value;
    compressed.Read(value);
    size = static_cast<int>(value);
    for (int i = 0; i < size; ++i) {
        byte symbol;
        int frequency;
        compressed.Read(symbol);
        compressed.Read(value);
        frequency = static_cast<int>(value);
        frequencies[symbol] = frequency;
    }

    // ������� ������ ��������
    HuffmanTree huffman;
    huffman.buildTree(frequencies);

    // ���������� ������ � ���������� �� � �������� �����
    HuffmanNode* node = huffman.getRoot();
    while (compressed.Read(value)) {
        std::bitset<8> bits(static_cast<unsigned char>(value));
        for (int i = 7; i >= 0; --i) {
            if (bits[i]) {
                node = node->right;
            }
            else {
                node = node->left;
            }
            if (node->isLeaf()) {
                original.Write(node->value);
                node = huffman.getRoot();
            }
        }
    }
}


int main() {
    // ������� ������� ������� ��� �������� ����� � ��������� �����
    FileInputStream input("input_file");
    FileOutputStream output("output_file");

    // �������� ������� �����������
    Encode(input, output);

    // ���������� ��������� �������� ����� �� ������
    input = FileInputStream("output_file");
    
    // �������� ������� �������������
    Decode(input, output);

    return 0;
}

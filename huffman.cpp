// huffman.cpp

#include <bits/stdc++.h>
using namespace std;

// -------------------- Huffman Tree Node --------------------
struct Node {
    char ch;
    int freq;
    Node *left, *right;

    // Leaf node constructor
    Node(char c, int f)
        : ch(c), freq(f), left(nullptr), right(nullptr) {}

    // Internal node constructor
    Node(Node* a, Node* b)
        : ch('\0'),
          freq(a->freq + b->freq),
          left(a),
          right(b) {}
};

// Comparator for priority queue (min-heap)
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

// -------------------- Generate Huffman Codes --------------------
void buildCodes(Node* node,
                string code,
                unordered_map<char, string>& codes) {

    if (!node)
        return;

    // Leaf node
    if (!node->left && !node->right) {
        codes[node->ch] = code.empty() ? "0" : code;
    }

    buildCodes(node->left, code + "0", codes);
    buildCodes(node->right, code + "1", codes);
}

// -------------------- Build Huffman Tree --------------------
Node* buildTreeFromFreq(const vector<int>& freq) {
    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            pq.push(new Node(static_cast<char>(i), freq[i]));
        }
    }

    if (pq.empty())
        return nullptr;

    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();

        Node* right = pq.top();
        pq.pop();

        pq.push(new Node(left, right));
    }

    return pq.top();
}

// -------------------- Main --------------------
int main(int argc, char* argv[]) {

    if (argc < 4) {
        cerr << "Usage: huffman <compress/decompress> <input> <output>\n";
        return 1;
    }

    string mode = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];

    // ==========================================================
    // Compression
    // ==========================================================
    if (mode == "compress") {

        ifstream in(inputFile, ios::binary);

        string text((istreambuf_iterator<char>(in)),
                    istreambuf_iterator<char>());

        // Calculate frequencies
        vector<int> freq(256, 0);
        for (char c : text) {
            freq[(unsigned char)c]++;
        }

        // Build Huffman Tree
        Node* root = buildTreeFromFreq(freq);

        // Generate codes
        unordered_map<char, string> codes;
        buildCodes(root, "", codes);

        // Encode text
        string bits;
        for (char c : text) {
            bits += codes[c];
        }

        ofstream out(outputFile, ios::binary);

        // Store frequency table
        for (int f : freq) {
            out.write(reinterpret_cast<char*>(&f), sizeof(int));
        }

        // Store number of valid bits in last byte
        int validBits = bits.size() % 8;
        if (validBits == 0)
            validBits = 8;

        out.write(reinterpret_cast<char*>(&validBits), sizeof(int));

        // Write compressed data
        for (size_t i = 0; i < bits.size(); i += 8) {

            unsigned char byte = 0;

            for (int j = 0; j < 8 && i + j < bits.size(); j++) {
                if (bits[i + j] == '1') {
                    byte |= (1 << (7 - j));
                }
            }

            out.write(reinterpret_cast<char*>(&byte), 1);
        }

        cout << "Compression successful\n";
    }

    // ==========================================================
    // Decompression
    // ==========================================================
    else if (mode == "decompress") {

        ifstream in(inputFile, ios::binary);

        // Read frequency table
        vector<int> freq(256);

        for (int i = 0; i < 256; i++) {
            in.read(reinterpret_cast<char*>(&freq[i]), sizeof(int));
        }

        // Read valid bits count
        int validBits;
        in.read(reinterpret_cast<char*>(&validBits), sizeof(int));

        // Read compressed bytes
        vector<unsigned char> data(
            (istreambuf_iterator<char>(in)),
            istreambuf_iterator<char>());

        // Rebuild Huffman Tree
        Node* root = buildTreeFromFreq(freq);

        ofstream out(outputFile, ios::binary);

        if (!root)
            return 0;

        Node* current = root;

        // Decode data
        for (size_t i = 0; i < data.size(); i++) {

            int limit = (i == data.size() - 1) ? validBits : 8;

            for (int j = 0; j < limit; j++) {

                int bit = (data[i] >> (7 - j)) & 1;

                current = bit ? current->right : current->left;

                // Reached leaf node
                if (!current->left && !current->right) {
                    out << current->ch;
                    current = root;
                }
            }
        }

        cout << "Decompression successful\n";
    }

    else {
        cerr << "Invalid mode. Use 'compress' or 'decompress'.\n";
        return 1;
    }

    return 0;
}

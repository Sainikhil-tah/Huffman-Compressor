# Huffman File Compressor

A simple file compression and decompression tool based on the Huffman Coding algorithm.

## Features

* Compress text files using Huffman Encoding.
* Decompress previously compressed files.
* Simple web interface for uploading files.
* Implemented in C++ with a lightweight frontend.

## Project Structure

```
├── huffman.cpp      # Huffman compression/decompression logic
├── templates/
│   └── index.html   # Web interface
├── static/
│   └── style.css    # Styling for the web page
├── app.py           # Backend server (if using Flask)
└── README.md
```

## How It Works

1. The program reads the input file.
2. Character frequencies are calculated.
3. A Huffman Tree is constructed.
4. Huffman codes are generated for each character.
5. The file is compressed into a binary format.
6. During decompression, the Huffman Tree is rebuilt using stored frequency data.

## Compilation

Compile the C++ program using:

```bash
g++ huffman.cpp -o huffman
```

## Usage

### Compress a File

```bash
./huffman compress input.txt compressed.bin
```

### Decompress a File

```bash
./huffman decompress compressed.bin output.txt
```

## Technologies Used

* C++
* Huffman Coding
* HTML
* CSS
* Flask 

## Future Improvements

* Support larger files efficiently.
* Add compression statistics.
* Improve the user interface.
* Support multiple file formats.

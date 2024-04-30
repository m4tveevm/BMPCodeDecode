[![C++ CI with Release](https://github.com/m4tveevm/BMPCodeDecode/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/m4tveevm/BMPCodeDecode/actions/workflows/c-cpp.yml)
# BMPCodeDecode

## Overview
BMPCodeDecode is a simple C++ project designed to encode any file into a BMP image and decode it back. This tool allows users to convert binary files into BMP images, ensuring a unique way to store and transfer files as images. The encoding includes a simple transformation where file data is directly translated into pixel values with an added security feature of byte shifting.

## Features
- **File to BMP Encoding**: Converts any file to a BMP image by reading its binary data and converting it into pixel data.
- **BMP to File Decoding**: Converts the BMP image back to the original file, reconstructing the exact binary data from the pixel information.
- **Security Feature**: Implements a byte shift for additional obfuscation, making direct reading from the BMP more challenging.
- **Command Line Interface**: Fully operational through CLI, allowing for easy automation and integration with other tools or scripts.

## Usage

### Prerequisites
- C++ compiler (e.g., GCC, Clang, MSVC)
- Basic command line knowledge

### Compilation
Compile the project using a C++ compiler. For example, using g++:

```bash
g++ -o BMPCodeDecode main.cpp
```

### Running the Program
The program can be used with the following commands:

**Encoding a file into a BMP:**

```bash
./BMPCodeDecode -e input_filename output_filename.bmp
```

**Decoding a BMP back into a file:**

```bash
./BMPCodeDecode -d input_filename.bmp output_filename
```

## Example
To encode a file named `example.txt` into a BMP image:

```bash
./BMPCodeDecode -e example.txt example.bmp
```

To decode the BMP image back into a file:

```bash
./BMPCodeDecode -d example.bmp new_example.txt
```

## Contributions
Contributions are welcome. Please feel free to fork the repository and submit a pull request.

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgments
- This project was inspired by digital image encoding techniques and aims to provide a straightforward method for converting and storing files as images.

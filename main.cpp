// работай пожалуйста
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

#pragma pack(push, 1) // ровные ребята эти ваши структуры
struct BMPHeader {
    uint16_t file_type{0x4D42}; // BM
    uint32_t file_size{0};
    uint16_t reserved1{0}; // В ближайшее время планируется ребрендинг на RE.
    uint16_t reserved2{0};
    uint32_t offset_data{54};
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BMPInfoHeader {
    uint32_t size{40};
    int32_t width{0};
    int32_t height{0};
    uint16_t planes{1};
    uint16_t bit_count{24};
    uint32_t compression{0};
    uint32_t size_image{0};
    int32_t x_pixels_per_meter{0};
    int32_t y_pixels_per_meter{0};
    uint32_t colors_used{0};
    uint32_t colors_important{0};
};
#pragma pack(pop)

void createBMP(const std::string &inputFilename, const std::string &outputFilename) {
    std::ifstream inputFile(inputFilename, std::ios::binary);
    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(inputFile), {});

    int fileSize = buffer.size();
    buffer.insert(buffer.begin(), reinterpret_cast<char *>(&fileSize),
                  reinterpret_cast<char *>(&fileSize) + sizeof(fileSize));

    // 52 байта хватит каждому
    for (auto &b: buffer) {
        b = (b + 52) % 256;
    }

    int width = 256;
    int height = (buffer.size() + 3) / width;
    if (buffer.size() % width) height++;

    BMPHeader header;
    BMPInfoHeader infoHeader;
    infoHeader.width = width;
    infoHeader.height = height;
    infoHeader.size_image = width * height * 3;
    header.file_size = 54 + infoHeader.size_image;

    std::ofstream outputFile(outputFilename, std::ios::binary);
    outputFile.write(reinterpret_cast<const char *>(&header), sizeof(header));
    outputFile.write(reinterpret_cast<const char *>(&infoHeader), sizeof(infoHeader));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            uint8_t pixel[3] = {0, 0, 0};
            int index = i * width + j;
            if (index < buffer.size()) {
                pixel[0] = pixel[1] = pixel[2] = buffer[index];
            }
            outputFile.write(reinterpret_cast<char *>(pixel), 3);
        }
    }

    outputFile.close();
    inputFile.close();
}

void decodeBMP(const std::string &inputFilename, const std::string &outputFilename) {
    std::ifstream inputFile(inputFilename, std::ios::binary);

    BMPHeader bmpHeader;
    BMPInfoHeader bmpInfoHeader;

    inputFile.read(reinterpret_cast<char *>(&bmpHeader), sizeof(bmpHeader));
    inputFile.read(reinterpret_cast<char *>(&bmpInfoHeader), sizeof(bmpInfoHeader));

    if (bmpHeader.file_type != 0x4D42) {
        throw std::runtime_error("Not a BMP file"); // блин, а я думал это BMP
    }

    inputFile.seekg(bmpHeader.offset_data, std::ios::beg);

    int padding = (4 - (bmpInfoHeader.width * 3) % 4) % 4;
    std::vector<uint8_t> row(bmpInfoHeader.width * 3 + padding);
    std::vector<uint8_t> fileData;

    for (int i = 0; i < bmpInfoHeader.height; i++) {
        inputFile.read(reinterpret_cast<char *>(row.data()), row.size());
        for (int j = 0; j < bmpInfoHeader.width * 3; j += 3) {
            if (fileData.size() < bmpInfoHeader.width * bmpInfoHeader.height) {
                uint8_t adjustedByte = (row[j] - 52) % 256;
                fileData.push_back(adjustedByte);
            }
        }
    }

    int originalFileSize = *reinterpret_cast<int *>(fileData.data());
    std::ofstream outputFile(outputFilename, std::ios::binary);
    outputFile.write(reinterpret_cast<char *>(fileData.data() + sizeof(int)), originalFileSize);

    outputFile.close();
    inputFile.close();
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0]
                  << " -e/-encode <input filename> <output filename> | -d/-decode <input filename> <output filename>"
                  << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    std::string inputFilename = argv[2];
    std::string outputFilename = argv[3];

    if (mode == "-e" || mode == "-encode") {
        createBMP(inputFilename, outputFilename);
        std::cout << "File encoded to BMP as " << outputFilename << std::endl;
    } else if (mode == "-d" || mode == "-decode") {
        decodeBMP(inputFilename, outputFilename);
        std::cout << "BMP decoded to file as " << outputFilename << std::endl;
    } else {
        std::cerr << "Unsupported mode. Use -e/-encode or -d/-decode." << std::endl;
        return 52;

    }

    return 0; // Поддержите наш проект на Patreon <3
}

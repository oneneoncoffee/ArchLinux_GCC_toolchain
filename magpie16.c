#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

#define CHUNK_SIZE 16
#define DELAY_MICROSECONDS 50000  // 50 milliseconds

uint16_t calculate_crc16(const uint8_t *data, size_t size) {
    uint16_t crc = 0;

    for (size_t i = 0; i < size; ++i) {
        crc ^= (uint16_t)data[i] << 8;
        for (int j = 0; j < 8; ++j) {
            crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1;
        }
    }

    return crc;
}

void hex_dump(FILE *file, size_t offset, size_t size, double percentage) {
    fseek(file, offset, SEEK_SET);

    uint8_t buffer[CHUNK_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, CHUNK_SIZE, file)) > 0) {
        printf("%04lx | ", offset);
        for (size_t i = 0; i < bytesRead; ++i) {
            printf("%02X ", buffer[i]);
        }

        // Print ASCII representation
        printf("| ");
        for (size_t i = 0; i < bytesRead; ++i) {
            if (buffer[i] >= 32 && buffer[i] <= 126) {
                printf("%c", buffer[i]);
            } else {
                printf(".");
            }
        }

        // Calculate CRC16 for the chunk
        uint16_t crc16 = calculate_crc16(buffer, bytesRead);

        printf(" | CRC16: 0x%04" PRIx16 "\n", crc16);

        offset += bytesRead;

        // Calculate and display percentage from 0.00% to 100.00%
        percentage = (double)offset / size * 100;
        if (percentage > 100.0) {
            percentage = 100.0;  // Cap at 100.00%
        }

        printf("Dumped %.2f%% of the file\n\n", percentage);

        usleep(DELAY_MICROSECONDS);
    }

    printf("Dumped 100.00%% of the file\n\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    FILE *file = fopen(filename, "rb");

    if (!file) {
        perror("Error opening file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    printf("File Size: %lu bytes\n", file_size);

    size_t offset = 0;
    double percentage = 0.0;

    while (offset < file_size) {
        hex_dump(file, offset, CHUNK_SIZE, percentage);
        offset += CHUNK_SIZE;
    }

    fclose(file);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef long long int64;
typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned long long uint64;

uint32_t crc_cksum(void *buf, size_t len) {
    uint32_t uVar2 = 0;
    for (size_t lVar1 = 0; lVar1 < len; lVar1 = lVar1 + 1) {
        uVar2 = (uVar2 >> 8 & 0xff | uVar2 << 8) ^ (uint32_t)*(uint8_t *)(buf + lVar1);
        uVar2 = uVar2 >> 4 & 0xf ^ uVar2;
        uVar2 = uVar2 << 0xc ^ uVar2;
        uVar2 = (uVar2 & 0xff) << 5 ^ uVar2;
    }
    return uVar2;
}

int main(int argc, char *argv[]) {
    printf("Synology grub_cksum Calculator v1.0 - K4L0\n");
    printf("------------------------------------------\n");

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open file\n");
        return 1;
    }

    fseek(fp, 0L, SEEK_END);
    long int file_size = ftell(fp);
    rewind(fp);

    char *buffer = (char*)malloc(file_size * sizeof(char));
    fread(buffer, sizeof(char), file_size, fp);
    fclose(fp);

    int64 crc = crc_cksum(buffer, file_size);
    printf("%s Encrypted: %llx\n", argv[1], crc);

    free(buffer);
    return 0;
}

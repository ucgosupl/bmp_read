#include <stdio.h>
#include <string.h>
#include <stdint.h>

enum
{
    BMP_TYPE_BM = 0x424D,
    BMP_TYPE_BA = 0x4241,
    BMP_TYPE_CI = 0x4349,
    BMP_TYPE_CP = 0x4350,
    BMP_TYPE_IC = 0x4943,
    BMP_TYPE_PT = 0x5054,
};

struct __attribute__((packed)) bmp_file_hdr
{
    uint16_t hdr_field;
    uint32_t size;
    uint8_t reserved1[4];
    uint32_t offset;
};

_Static_assert(14 == sizeof(struct bmp_file_hdr), "Struct size doesn't match");

int main(void)
{
    FILE *file;
    struct bmp_file_hdr bmp_hdr;

    file = fopen("test2.bmp", "rb");

    memset(&bmp_hdr, 0, sizeof(bmp_hdr));
    int ret = fread(&bmp_hdr, 1, sizeof(bmp_hdr), file);
 
    printf("BMP HDR contents:\n");
    printf("Header field: 0x%04X\n", bmp_hdr.hdr_field);
    printf("Size in bytes: %d\n", bmp_hdr.size);
    printf("Pixel map offset in bytes: %d\n", bmp_hdr.offset);

    fclose(file);

    return 0;
}
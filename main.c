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

enum
{
    BMP_COMP_BI_RGB = 0,
    BMP_COMP_BI_RLE8 = 1,
    BMP_COMP_BI_RLE4 = 2,
    BMP_COMP_BI_BITFIELDS = 3,
    BMP_COMP_BI_JPEG = 4,
    BMP_COMP_BI_PNG = 5,
    BMP_COMP_BI_ALPHABIT = 6,
    BMP_COMP_BI_CMYK = 11,
    BMP_COMP_BI_CMYKRLE8 = 12,
    BMP_COMP_BI_CMYKRLE4 = 13,
};

typedef uint32_t dib_hdr_size_t;

struct __attribute__((packed)) bmp_file_hdr
{
    uint16_t hdr_field;
    uint32_t size;
    uint8_t reserved1[4];
    uint32_t offset;
};

_Static_assert(14 == sizeof(struct bmp_file_hdr), "Struct size doesn't match");

struct __attribute__((packed)) bmp_core_hdr
{
    dib_hdr_size_t hdr_size;
    uint16_t width_in_pxl;
    uint16_t height_in_pxl;
    uint16_t color_planes;
    uint16_t bits_per_pxl;
};

_Static_assert(12 == sizeof(struct bmp_core_hdr), "Struct size doesn't match");

struct __attribute__((packed)) bmp_info_hdr
{
    dib_hdr_size_t hdr_size;
    uint32_t width_in_pxl;
    uint32_t height_in_pxl;
    uint16_t color_planes;
    uint16_t bits_per_pxl;
    uint32_t compression_method;
    uint32_t image_size;
    uint32_t h_res;
    uint32_t v_res;
    uint32_t colors_in_pallette;
    uint32_t important_colors;
};

_Static_assert(40 == sizeof(struct bmp_info_hdr), "Struct size doesn't match");

union dib_hdr
{
    dib_hdr_size_t size;

    struct bmp_core_hdr core_hdr;
    struct bmp_info_hdr info_hdr;

    uint8_t bytes[sizeof(struct bmp_info_hdr)];
};

int main(void)
{
    FILE *file;
    struct bmp_file_hdr bmp_hdr;

    file = fopen("test24bit.bmp", "rb");

    memset(&bmp_hdr, 0, sizeof(bmp_hdr));
    fread(&bmp_hdr, 1, sizeof(bmp_hdr), file);
 
    printf("BMP File Header contents:\n");
    printf("Header field: 0x%04X\n", bmp_hdr.hdr_field);
    printf("Size in bytes: %d\n", bmp_hdr.size);
    printf("Pixel map offset in bytes: %d\n", bmp_hdr.offset);

    printf("\n\n");

    union dib_hdr dib_hdr;

    memset(&dib_hdr, 0, sizeof(dib_hdr));
    fread(&dib_hdr, 1, sizeof(dib_hdr_size_t), file);

    switch(dib_hdr.size)
    {
        case 12:
            fread(dib_hdr.bytes + sizeof(dib_hdr_size_t), 1, sizeof(struct bmp_core_hdr) -  sizeof(dib_hdr_size_t), file);

            printf("BMP Core Header contents:\n");
            printf("Header size: %d\n", dib_hdr.core_hdr.hdr_size);
            printf("Width in pixels: %d\n", dib_hdr.core_hdr.width_in_pxl);
            printf("Height in pixels: %d\n", dib_hdr.core_hdr.height_in_pxl);
            printf("Color planes: %d\n", dib_hdr.core_hdr.color_planes);
            printf("Bits per pixel: %d\n", dib_hdr.core_hdr.bits_per_pxl);

            break;

        case 40:
            fread(dib_hdr.bytes + sizeof(dib_hdr_size_t), 1, sizeof(struct bmp_info_hdr) - sizeof(dib_hdr_size_t), file);

            printf("BMP Core Header contents:\n");
            printf("Header size: %d\n", dib_hdr.info_hdr.hdr_size);
            printf("Width in pixels: %d\n", dib_hdr.info_hdr.width_in_pxl);
            printf("Height in pixels: %d\n", dib_hdr.info_hdr.height_in_pxl);
            printf("Color planes: %d\n", dib_hdr.info_hdr.color_planes);
            printf("Bits per pixel: %d\n", dib_hdr.info_hdr.bits_per_pxl);

            break;
    }

    fclose(file);

    return 0;
}
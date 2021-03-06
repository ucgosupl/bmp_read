#include <stdio.h>
#include <string.h>
#include <stdint.h>



#define BMP_FILE_HDR_SIZE sizeof(struct bmp_file_hdr)
#define DIB_BMP_CORE_HDR_SIZE sizeof(struct dib_bmp_core_hdr)
#define DIB_BMP_INFO_HDR_SIZE sizeof(struct dib_bmp_info_hdr)
#define DIB_HDR_SIZE_SIZE sizeof(dib_hdr_size_t)

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

typedef uint16_t bmp_type_t;
typedef uint32_t dib_hdr_size_t;

typedef uint32_t comp_method_t;

struct __attribute__((packed)) bmp_file_hdr
{
    bmp_type_t bmp_type;
    uint32_t size;
    uint8_t reserved1[4];
    uint32_t offset;
};

_Static_assert(14 == sizeof(struct bmp_file_hdr), "Struct size doesn't match");

struct __attribute__((packed)) dib_bmp_core_hdr
{
    dib_hdr_size_t hdr_size;
    uint16_t width_in_pxl;
    uint16_t height_in_pxl;
    uint16_t color_planes;
    uint16_t bits_per_pxl;
};

_Static_assert(12 == sizeof(struct dib_bmp_core_hdr), "Struct size doesn't match");

struct __attribute__((packed)) dib_bmp_info_hdr
{
    dib_hdr_size_t hdr_size;
    uint32_t width_in_pxl;
    uint32_t height_in_pxl;
    uint16_t color_planes;
    uint16_t bits_per_pxl;
    comp_method_t compression_method;
    uint32_t image_size;
    uint32_t h_res;
    uint32_t v_res;
    uint32_t colors_in_pallette;
    uint32_t important_colors;
};

_Static_assert(40 == sizeof(struct dib_bmp_info_hdr), "Struct size doesn't match");

union dib_hdr
{
    dib_hdr_size_t size;

    struct dib_bmp_core_hdr core_hdr;
    struct dib_bmp_info_hdr info_hdr;

    uint8_t bytes[DIB_BMP_INFO_HDR_SIZE];
};

static void print_bmp_hdr(struct bmp_file_hdr *hdr)
{
    printf("BMP File Header contents:\n");
    printf("Header field: 0x%04X\n", hdr->bmp_type);
    printf("Size in bytes: %d\n", hdr->size);
    printf("Pixel map offset in bytes: %d\n", hdr->offset);

    printf("\n\n");
}

static void print_dib_core_hdr(struct dib_bmp_core_hdr *hdr)
{
    printf("DIB BMP Core Header contents:\n");
    printf("Header size: %d\n", hdr->hdr_size);
    printf("Width in pixels: %d\n", hdr->width_in_pxl);
    printf("Height in pixels: %d\n", hdr->height_in_pxl);
    printf("Color planes: %d\n", hdr->color_planes);
    printf("Bits per pixel: %d\n", hdr->bits_per_pxl);

    printf("\n\n");
}

static void print_dib_info_hdr(struct dib_bmp_info_hdr *hdr)
{
    printf("DIB BMP Info Header contents:\n");
    printf("Header size: %d\n", hdr->hdr_size);
    printf("Width in pixels: %d\n", hdr->width_in_pxl);
    printf("Height in pixels: %d\n", hdr->height_in_pxl);
    printf("Color planes: %d\n", hdr->color_planes);
    printf("Bits per pixel: %d\n", hdr->bits_per_pxl);

    printf("\n\n");
}

int main(void)
{
    FILE *file;
    struct bmp_file_hdr bmp_hdr;

    file = fopen("test24bit.bmp", "rb");

    memset(&bmp_hdr, 0, sizeof(bmp_hdr));
    fread(&bmp_hdr, 1, sizeof(bmp_hdr), file);
 
    print_bmp_hdr(&bmp_hdr);

    union dib_hdr dib_hdr;

    /* 
     * There are many different version of DIB headers.
     * They are recognized by first element indicating header size.
     * So we must read only this element, recognize header version, and read rest of it.
     */
    memset(&dib_hdr, 0, sizeof(dib_hdr));
    fread(&dib_hdr, 1, sizeof(dib_hdr_size_t), file);

    switch(dib_hdr.size)
    {
        case DIB_BMP_CORE_HDR_SIZE:
            fread(dib_hdr.bytes + DIB_HDR_SIZE_SIZE, 1, DIB_BMP_CORE_HDR_SIZE - DIB_HDR_SIZE_SIZE, file);
            print_dib_core_hdr(&dib_hdr.core_hdr);
            break;

        case DIB_BMP_INFO_HDR_SIZE:
            fread(dib_hdr.bytes + DIB_HDR_SIZE_SIZE, 1, DIB_BMP_INFO_HDR_SIZE - DIB_HDR_SIZE_SIZE, file);
            print_dib_info_hdr(&dib_hdr.info_hdr);
            break;
    }

    fclose(file);

    return 0;
}
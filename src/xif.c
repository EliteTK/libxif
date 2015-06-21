#include "xif.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb_image.h>

#define SIG "XIF"
#define VER 0

typedef struct {
	char sig[4];
	uint16_t width;
	uint16_t height;
	uint8_t depth;
	uint8_t pad;
	uint8_t unit;
	uint32_t plane_mask;
	struct {
		uint8_t
			ver : 4,
			    format : 2,
			    byte_order : 1,
			    bit_order : 1;
	} bf;
} xif_header_t;

void xif_write(xcb_image_t *image, char *filepath)
{
	FILE *fp = fopen(filepath, "w");

	xif_header_t *header = malloc(sizeof(xif_header_t));

	memcpy(header->sig, SIG, 3);
	header->width = image->width;
	header->height = image->height;
	header->bf.format = image->format;
	header->pad = image->scanline_pad;
	header->depth = image->depth;
	header->unit = image->unit;
	header->plane_mask = image->plane_mask;
	header->bf.byte_order = image->byte_order;
	header->bf.byte_order = image->bit_order;

	fwrite(header, sizeof(xif_header_t), 1, fp);
	fwrite(image->data, sizeof(uint8_t), image->size, fp);

	fclose(fp);
}

xcb_image_t *xif_read(char *filepath)
{
	xif_header_t header;
	FILE *fp = fopen(filepath, "r");

	if (!fp)
		return NULL;

	if (fread(&header, sizeof(xif_header_t), 1, fp) != sizeof(xif_header_t))
		return NULL;

	if (strncmp(header.sig, SIG, strlen(SIG)) != 0)
		return NULL;

	if (header.bf.ver != VER)
		fputs("Error: The file version and library versions do not match.\nExpect the unexpected.\n", stderr);

	uint32_t bytes = header.width * header.height
		* ((header.depth + header.pad) / 8);
	uint8_t *img = malloc(bytes);

	if (fread(img, sizeof(uint8_t), bytes, fp) != bytes)
		return NULL;

	xcb_image_t *image = xcb_image_create(
					      header.width, header.height,
					      header.bf.format,
					      header.pad, header.depth,
					      header.pad + header.depth,
					      header.unit,
					      header.bf.byte_order, header.bf.bit_order,
					      img, bytes, img);

	fclose(fp);

	return image;
}

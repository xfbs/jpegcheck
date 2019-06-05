#include "util.h"
#include <stdio.h>
#include <stdlib.h>

#define EXIT_HELP  1
#define EXIT_ALLOC 2
#define EXIT_FREAD 3
#define EXIT_ERROR 4

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

int show_help(const char *name) {
  fprintf(stderr, "%s <filename>\n", name);

  return EXIT_HELP;
}

const char *jpeg_error_to_str(int ret) {
  switch(ret) {
    case  0: return "ERR_SUCCESS";
    case  1: return "ERR_NO_SOI";
    case  2: return "ERR_NOT_8BIT";
    case  3: return "ERR_HEIGHT_MISMATCH";
    case  4: return "ERR_WIDTH_MISMATCH";
    case  5: return "ERR_BAD_WIDTH_OR_HEIGHT";
    case  6: return "ERR_TOO_MANY_COMPPS";
    case  7: return "ERR_ILLEGAL_HV";
    case  8: return "ERR_QUANT_TABLE_SELECTOR";
    case  9: return "ERR_NOT_YCBCR_221111";
    case 10: return "ERR_UNKNOWN_CID_IN_SCAN";
    case 11: return "ERR_NOT_SEQUENTIAL_DCT";
    case 12: return "ERR_WRONG_MARKER";
    case 13: return "ERR_NO_EOI";
    case 14: return "ERR_BAD_TABLES";
    case 15: return "ERR_DEPTH_MISMATCH";
    default: return "ERR_UNKNOWN";
  }
}

void jpeg_error_show(int ret) {
  eprintf("Error in jpeg_decode: %s.\n", jpeg_error_to_str(ret));
}

const char *read_file(const char *name) {
  FILE *file = fopen(name, "rb");

  if(file == NULL) {
    eprintf("error while opening file %s.\n", name);
    return NULL;
  }

  // find out size of file
  fseek(file, 0, SEEK_END);
  size_t length = ftell(file);
  fseek(file, 0, SEEK_SET);

  // read file into buffer
  char *buffer = malloc(length + 1);
  if(buffer) {
    if(length != fread(buffer, 1, length, file)) {
      eprintf("error while reading file %s.\n", name);
      free(buffer);
      return NULL;
    }
    buffer[length] = '\0';
  }

  fclose(file);

  return buffer;
}

int load_file(const char *name) {
  // allocate jpeg decode data struct
  struct jpeg_decdata *data = jpeg_alloc();

  if(data == NULL) {
    eprintf("can't alloc jpeg_decdata\n");
    return EXIT_ALLOC;
  }

  const char *content = read_file(name);

  if(content == NULL) {
    eprintf("can't read file %s.\n", name);
    return EXIT_FREAD;
  }

  int ret = jpeg_decode(data, (unsigned char *) content);

  if(ret != 0) {
    eprintf("error in jpeg_decode: %s.\n", jpeg_error_to_str(ret));
    return EXIT_ERROR;
  }

  printf("Managed to load the JPG file!\n");

  int width;
  int height;

  jpeg_get_size(data, &width, &height);

  printf("size: %i x %i px\n", width, height);

  int depth = 24;
  unsigned char *pic = malloc(width * height * depth / 8);

  if(!pic) {
    eprintf("can't alloc space for pic.\n");
    return EXIT_ALLOC;
  }

  ret = jpeg_show(data, pic, width, height, depth, 0);

  if(ret != 0) {
    eprintf("error in jpeg_show: %s.\n", jpeg_error_to_str(ret));
    return EXIT_ERROR;
  }

  printf("managed to extract pixel data from image.\n");

  free(pic);
  free(data);

  return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
  // filename is passed as first argument
  if(argc <= 1) {
    return show_help(argv[0]);
  } else {
    return load_file(argv[1]);
  }
}

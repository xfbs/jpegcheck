#include "util.h"
#include <stdio.h>
#include <stdlib.h>

#define EXIT_HELP  1
#define EXIT_ALLOC 2
#define EXIT_FREAD 3
#define EXIT_ERROR 4

int show_help(const char *name) {
  fprintf(stderr, "%s <filename>\n", name);

  return EXIT_HELP;
}

void show_error(int ret) {
  switch(ret) {
    case 1:
      fprintf(stderr, "ERR_NO_SOI\n");
      break;
    case 2:
      fprintf(stderr, "ERR_NOT_8BIT\n");
      break;
    case 3:
      fprintf(stderr, "ERR_HEIGHT_MISMATCH\n");
      break;
    case 4:
      fprintf(stderr, "ERR_WIDTH_MISMATCH\n");
      break;
    case 5:
      fprintf(stderr, "ERR_BAD_WIDTH_OR_HEIGHT\n");
      break;
    case 6:
      fprintf(stderr, "ERR_TOO_MANY_COMPPS\n");
      break;
    case 7:
      fprintf(stderr, "ERR_ILLEGAL_HV\n");
      break;
    case 8:
      fprintf(stderr, "ERR_QUANT_TABLE_SELECTOR\n");
      break;
    case 9:
      fprintf(stderr, "ERR_NOT_YCBCR_221111\n");
      break;
    case 10:
      fprintf(stderr, "ERR_UNKNOWN_CID_IN_SCAN\n");
      break;
    case 11:
      fprintf(stderr, "ERR_NOT_SEQUENTIAL_DCT\n");
      break;
    case 12:
      fprintf(stderr, "ERR_WRONG_MARKER\n");
      break;
    case 13:
      fprintf(stderr, "ERR_NO_EOI\n");
      break;
    case 14:
      fprintf(stderr, "ERR_BAD_TABLES\n");
      break;
    case 15:
      fprintf(stderr, "ERR_DEPTH_MISMATCH\n");
      break;
    default:
      fprintf(stderr, "unknown error occured in jpeg_decode().\n");
  }
}

const char *read_file(const char *name) {
  FILE *file = fopen(name, "rb");

  if(file == NULL) {
    fprintf(stderr, "error while opening file %s.\n", name);
    return NULL;
  }

  // find out size of file
  fseek(file, 0, SEEK_END);
  size_t length = ftell(file);
  fseek(file, 0, SEEK_SET);

  // read file into buffer
  char *buffer = malloc(length + 1);
  if(buffer) {
    fread(buffer, 1, length, file);
    buffer[length] = '\0';
  }

  fclose(file);

  return buffer;
}

int load_file(const char *name) {
  // allocate jpeg decode data struct
  struct jpeg_decdata *data = jpeg_alloc();

  if(data == NULL) {
    fprintf(stderr, "can't alloc jpeg_decdata\n");
    return EXIT_ALLOC;
  }

  const char *content = read_file(name);

  if(content == NULL) {
    fprintf(stderr, "can't read file %s.\n", name);
    return EXIT_FREAD;
  }

  int ret = jpeg_decode(data, (unsigned char *) content);

  if(ret != 0) {
    show_error(ret);
    return EXIT_ERROR;
  }

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

// jpeg.c

// allocate new jpeg_decdata struct. can be free'd with free().
struct jpeg_decdata *jpeg_alloc(void);

// decode the header of a JPEG file in buf. must be a valid file.
int jpeg_decode(struct jpeg_decdata *jpeg, unsigned char *buf);

// extract the size of the jpeg image file.
void jpeg_get_size(struct jpeg_decdata *jpeg, int *width, int *height);

// extract the pixel value into pic.
int jpeg_show(struct jpeg_decdata *jpeg, unsigned char *pic, int width
              , int height, int depth, int bytes_per_line_dest);

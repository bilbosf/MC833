#ifndef IMAGES
#define IMAGES

typedef struct image_t {
    unsigned char *data;
    long size;
} image_t;

image_t *load_image(char *path);

int save_image(char *path, image_t *img);

#endif
#include <stdio.h>
#include <stdlib.h>
#include "images.h"

image_t *load_image(char *path){
    image_t *img = malloc(sizeof(image_t));
    FILE *file;

    file = fopen(path, "rb");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    img->size = ftell(file);
    rewind(file);

    img->data = (unsigned char *)malloc(img->size);
    if (img->data == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    // Read the file into the buffer
    size_t result = fread(img->data, 1, img->size, file);
    if (result != img->size) {
        printf("Failed to read the file.\n");
        return  NULL;
    }

    fclose(file);
    return img;
}

int save_image(char *path, image_t *img){
    FILE *file = fopen(path, "wb");
    if (file == NULL) {
        printf("Failed to create the file.\n");
        return 1;
    }

    long result = fwrite(img->data, 1, img->size, file);
    if (result != img->size) {
        printf("Failed to write to the file.\n");
        return 1;
    }

    fclose(file);
    return 0;
}
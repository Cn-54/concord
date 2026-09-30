#include <stdio.h>
#include <string.h>

char *get_file_extension(char *filename){ // gets the extention including the .
    char *dot = strrchr(filename, '.');

    if (dot == NULL || dot == filename)
        return NULL;

    return dot;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage: concord <filename>");
        return 1;
    }

    char *filename = argv[1];
    FILE *file = fopen(filename, "rb");

    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    unsigned char magic_bytes[8];
    size_t bytes_read = fread(magic_bytes, 8, 1, file); // reads the magic bytes

    if (bytes_read < sizeof(magic_bytes)) {
        printf("Could not read 8 magic bytes\n");
        fclose(file);
        return 1;
    }
    fclose(file);

    printf("Extension: %s\n", get_file_extension(filename));
    printf("Magic bytes: ");
    
    for (int i = 0; i < 8; i++) {
        printf("%02X ", magic_bytes[i]);
    }

    printf("\n");

    return 0;
}
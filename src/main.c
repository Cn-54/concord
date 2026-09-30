#include <stdio.h>
#include <string.h>
#include <strings.h>

char *get_file_extension(char *filename){ // gets the extention
    char *dot = strrchr(filename, '.');

    if (dot == NULL || dot == filename)
        return NULL;

    return dot +1;
}

char *get_file_type(unsigned char *magic_bytes){
    if (memcmp(magic_bytes, "\x89PNG\r\n\x1A\n", 8) == 0)
        return "PNG";
    else if (memcmp(magic_bytes, "\xFF\xD8\xFF", 3) == 0)
        return "JPEG";
    else if (memcmp(magic_bytes, "%PDF", 4) == 0)
        return "PDF";
    else if (memcmp(magic_bytes, "MZ", 2) == 0)
        return "EXE";
    else
        return "UNKNOWN";
}

int does_extension_match_type(char *extension, char *type){
    if (strcasecmp(extension, type) == 0)
        return 1;
    else
        return 0;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage: concord <filename>\n");
        return 1;
    }

    char *filename = argv[1];
    FILE *file = fopen(filename, "rb");

    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    unsigned char magic_bytes[8];
    size_t bytes_read = fread(magic_bytes, 1, 8, file); // reads the magic bytes

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

    printf("Detected: %s\n", get_file_type(magic_bytes));

    printf("does extention match type? : %s\n", does_extension_match_type(get_file_extension(filename),get_file_type(magic_bytes))? "YES":"NO");

    return 0;
}
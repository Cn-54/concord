#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <time.h>

char *get_file_extension(char *filename){ // gets the extention
    char *dot = strrchr(filename, '.');

    if (dot == NULL || dot == filename)
        return NULL;

    return dot +1;
}

char *get_file_type(unsigned char *magic_bytes){ // gets the file type
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

int does_extension_match_type(char *extension, char *type){ // checks wether the type and extention match
    if (strcasecmp(extension, type) == 0)
        return 1;
    else
        return 0;
}

void print_header(char *filename){
    printf("\n");
    printf("╔══════════════════════════════════════════╗\n");
    printf("║                 CONCORD                  ║\n");
    printf("║          File Forensics Analysis         ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf("\n");

    printf("File: %s\n", filename);
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage: concord <filename>\n");
        return 1;
    }
    struct stat fileStat;

    char *filename = argv[1];
    FILE *file = fopen(filename, "rb");

    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    print_header(filename);

    unsigned char magic_bytes[8];
    size_t bytes_read = fread(magic_bytes, 1, 8, file); // reads the magic bytes

    if (bytes_read < sizeof(magic_bytes)) {
        printf("Could not read 8 magic bytes\n");
        fclose(file);
        return 1;
    }
    fclose(file);

    printf("\n== File Identification ====================\n");

    printf("Extension: %s\n", get_file_extension(filename));

    printf("Magic bytes: ");

    
    for (int i = 0; i < 8; i++) { // prints the magic bytes 
        printf("%02X ", magic_bytes[i]);
    }

    printf("\n");

    printf("Detected: %s\n", get_file_type(magic_bytes));

    printf("does extention match type? : %s\n", does_extension_match_type(get_file_extension(filename),get_file_type(magic_bytes))? "YES":"NO");

    if (stat(filename, &fileStat) < 0) {
        return 1;
    }

    printf("\n== Meta data ====================\n");
    printf("File Size: %ld bytes\n", fileStat.st_size);
    printf("Permissions: %o\n", fileStat.st_mode);
    printf("Last Modified: %s", ctime(&fileStat.st_mtime));

    return 0;
}
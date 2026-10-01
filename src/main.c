#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <time.h>
#include <openssl/sha.h>

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

// following code was taking and modified from https://stackoverflow.com/questions/7853156/calculate-and-print-sha256-hash-of-a-file-using-openssl

void sha256_hash_string (char hash[SHA256_DIGEST_LENGTH], char outputBuffer[65])
{
    int i = 0;

    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }

    outputBuffer[64] = 0;
}

int calc_sha256(const char *path, char output[65])
{
    FILE *file = fopen(path, "rb");
    if (!file)
        return -1;
    unsigned char buffer[32768];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        SHA256_Update(&sha256, buffer, bytesRead);
    }
    if (ferror(file)) {
        fclose(file);
        return -1;
    }
    SHA256_Final(hash, &sha256);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[64] = '\0';
    fclose(file);
    return 0;
}

// end of stack overflow code

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
    char *extension = get_file_extension(filename);
    char *type = get_file_type(magic_bytes);

    if (extension != NULL)
        printf("Extension: %s\n", extension);
    else
        printf("Extension: NONE\n");


    printf("Magic bytes: ");

    
    for (int i = 0; i < 8; i++) { // prints the magic bytes 
        printf("%02X ", magic_bytes[i]);
    }

    printf("\n");

    printf("Detected: %s\n", type);

    printf("Extension match: %s\n",extension != NULL && does_extension_match_type(extension, type) ? "YES" : "NO");
            
    if (stat(filename, &fileStat) < 0) {
        return 1;
    }

    printf("\n== Meta data ==============================\n");
    printf("File Size: %ld bytes\n", fileStat.st_size);
    printf("Permissions: %o\n", fileStat.st_mode);
    printf("Last Modified: %s", ctime(&fileStat.st_mtime));

    char hash[65];

    printf("\n== Hash ===================================\n");
    if (calc_sha256(filename, hash) == 0)
        printf("SHA-256: %s\n", hash);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME_MAXLEN 8
#define BLOCK_SIZE 128
#define INODE_SIZE 56
#define INODE_COUNT 16
#define DATA_BLOCK_COUNT 127

typedef struct inode {
    int  dir;
    char name[FILENAME_MAXLEN];
    int  size;
    int  blockptrs[8];
    int  used;
    int  rsvd;
} inode;

typedef struct dirent {
    char name[FILENAME_MAXLEN];
    int  namelen;
    int  inode;
} dirent;

typedef struct superblock {
    char free_block_list[BLOCK_SIZE];
    inode inodes[INODE_COUNT];
} superblock;

superblock sb; // Global superblock

void initializeFileSystem() {
    // Initialize the superblock and inodes
    memset(&sb, 0, sizeof(superblock));
    // Set all blocks as free initially
    memset(sb.free_block_list, 1, BLOCK_SIZE);
    // Initialize root directory
    sb.inodes[0].dir = 1;
    sb.inodes[0].used = 1;
    strcpy(sb.inodes[0].name, "/");
}

void createFile(const char* filename, int size) {
   
    for (int i = 0; i < INODE_COUNT; i++) {
        if (!sb.inodes[i].used) {
            sb.inodes[i].used = 1;
            sb.inodes[i].dir = 0;
            sb.inodes[i].size = size;
            strncpy(sb.inodes[i].name, filename, FILENAME_MAXLEN);
            printf("Created file: %s\n", filename);
            return;
        }
    }
    printf("No free inodes available\n");
}

void deleteFile(const char* filename) {
    // Find the file inode and free it
    for (int i = 0; i < INODE_COUNT; i++) {
        if (sb.inodes[i].used && !sb.inodes[i].dir && strcmp(sb.inodes[i].name, filename) == 0) {
            sb.inodes[i].used = 0;
            printf("Deleted file: %s\n", filename);
            return;
        }
    }
    printf("File not found: %s\n", filename);
}

void copyFile(const char* srcname, const char* dstname) {
    // Simplified copy logic
    printf("Copy file from %s to %s\n", srcname, dstname);
}

void moveFile(const char* srcname, const char* dstname) {
   
    printf("Move file from %s to %s\n", srcname, dstname);
}

void createDirectory(const char* dirname) {
    
    printf("Create directory: %s\n", dirname);
}

void removeDirectory(const char* dirname) {
    
    printf("Remove directory: %s\n", dirname);
}

void listAllFiles() {
    // List all files and directories
    printf("Listing all files and directories:\n");
    for (int i = 0; i < INODE_COUNT; i++) {
        if (sb.inodes[i].used) {
            printf("%s - %s - Size: %d\n", sb.inodes[i].dir ? "Directory" : "File", sb.inodes[i].name, sb.inodes[i].size);
        }
    }
}

int main(int argc, char* argv[]) {
    FILE* file = fopen("sampleinput.txt", "r");
    if (!file) {
        perror("Error opening input file");
        return 1;
    }

    initializeFileSystem();

    char command[50];
    char param1[FILENAME_MAXLEN];
    char param2[FILENAME_MAXLEN];
    int size;

    while (fgets(command, sizeof(command), file) != NULL) {
        if (sscanf(command, "CR %s %d", param1, &size) == 2) {
            createFile(param1, size);
        } else if (sscanf(command, "DL %s", param1) == 1) {
            deleteFile(param1);
        } else if (sscanf(command, "CP %s %s", param1, param2) == 2) {
            copyFile(param1, param2);
        } else if (sscanf(command, "MV %s %s", param1, param2) == 2) {
            moveFile(param1, param2);
        } else if (sscanf(command, "CD %s", param1) == 1) {
            createDirectory(param1);
        } else if (sscanf(command, "DD %s", param1) == 1) {
            removeDirectory(param1);
        } else if (strncmp(command, "LL", 2) == 0) {
            listAllFiles();
        } else {
            printf("Unknown command: %s", command);
        }
    }

    fclose(file);
    return 0;
}

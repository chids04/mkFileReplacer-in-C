#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_MODS 500
#define MAX_FILE_LEN 300
int modAmount = 0;

int validatePath(char* path){
    DIR *dir = opendir(path);
    if(dir==NULL){
        return 1;
    }
    else{
        return 0;
    }
}

char **getFiles(char *path){

    DIR *dir = opendir(path);
    if(dir==NULL){
        printf("\nError accessing mods directory");
        closedir(dir);
        return NULL;
    }

    struct dirent *entry;
    int iter = 0;
    char **mods = malloc(MAX_MODS*sizeof(char*));
    
    while ((entry = readdir(dir)) != NULL) {

        if(strcmp(entry->d_name,"Award.szs") == 0 || strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0) {
            continue;

        }else {
            mods[iter] = malloc(MAX_FILE_LEN*sizeof(char));
            strcpy(mods[iter], entry->d_name);
            iter++;
        }
    }

    modAmount = iter;
    closedir(dir);
    return mods;

}

void findPath(char *path, char* filename, char *result){
    DIR *dir = opendir(path);
    struct dirent *entry;
    char newPath[1024];

    if(dir != NULL){

        while((entry = readdir(dir)) != NULL){
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            strcpy(newPath, path);
            strcat(newPath, "/");
            strcat(newPath, entry->d_name);

            if(entry->d_type == DT_DIR){
                findPath(newPath, filename, result);
            }
            else if(entry->d_type == DT_REG){
                if((strcmp(entry->d_name,filename) == 0)){
                    strcpy(result,newPath);
                    closedir(dir);
                    return;
                }
            }
        }
    }else{
        printf("Error accessing decompiled game directory");
        closedir(dir);
        return;
    }

    
}

int copyAndReplace(char *srcPath, char* destPath){

    FILE *source_file, *destination_file;
    char buffer[4096];
    size_t bytes_read;

    // Open the source file for reading
    source_file = fopen(srcPath, "rb");
    if (source_file == NULL) {
        printf("Error: Unable to open source file.\n");
        return 1;
    }

    // Open the destination file for writing
    destination_file = fopen(destPath, "wb");
    if (destination_file == NULL) {
        printf("Error: Unable to open destination file.\n");
        return 1;
    }

    // Copy the contents of the source file to the destination file
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), source_file)) > 0) {
        fwrite(buffer, 1, bytes_read, destination_file);
    }

    // Close the source and destination files
    fclose(source_file);
    fclose(destination_file);

    return 0;
}

void savePaths(char* srcPath, char* destPath){
    FILE *paths;
    paths = fopen("paths.bin", "wb");

    fwrite(srcPath, sizeof(char), strlen(srcPath), paths);
    fputc('\n', paths);

    fwrite(destPath, sizeof(char), strlen(destPath), paths);
    fputc('\n', paths);

    fclose(paths);
}

char **loadPaths() {
    FILE *inputs;
    char srcPath[MAX_FILE_LEN];
    char destPath[MAX_FILE_LEN];

    inputs = fopen("paths.bin", "rb");
    char **paths = malloc(2*sizeof(char*));

    paths[0] = malloc(MAX_FILE_LEN*sizeof(char));
    fgets(srcPath, MAX_FILE_LEN, inputs);
    srcPath[strcspn(srcPath, "\n")] = '\0'; // remove newline character
    strcpy(paths[0], srcPath);

    paths[1] = malloc(MAX_FILE_LEN*sizeof(char));
    fgets(destPath, MAX_FILE_LEN, inputs);
    destPath[strcspn(destPath, "\n")] = '\0';
    strcpy(paths[1], destPath);

    fclose(inputs);

    return paths;
    
}


int main(void) {

    char srcDest[MAX_FILE_LEN], modDest[MAX_FILE_LEN], input[MAX_FILE_LEN];
    bool invalid;
    char choice;
    bool loaded = false;

    if (access("paths.bin", F_OK) != 0){
        do{
            printf("Enter path of decompiled game image: ");
            fgets(input, MAX_FILE_LEN, stdin);
            sscanf(input, "%[^\n]", srcDest);
    
            printf("Enter path of mods folder: ");
            fgets(input, MAX_FILE_LEN, stdin);
            sscanf(input, "%[^\n]", modDest);

            if(validatePath(srcDest) == 0 && validatePath(modDest) == 0){
                invalid = false;
            }
            else{
                printf("\nOne of the paths entered is invalid\n\n");
                invalid = true;
            }
        }   while(invalid);
    
    }else{
        
        printf("Would you like to load the last saved paths? [y/n]: ");
        scanf("%c", &choice);
        fgets(input, MAX_FILE_LEN, stdin); //clears input buffer

        if(choice == 'y' || choice == 'y') {
            char **paths = loadPaths();
            strcpy(srcDest, paths[0]);
            strcpy(modDest, paths[1]);
            loaded = true;
            printf("\nDecompiled game path: %s\nMods folder path: %s\n", srcDest, modDest);

            free(paths[0]);
            free(paths[1]);
            free(paths);

        } else {
            do{
                printf("\nEnter path of decompiled game image: ");
                fgets(input, MAX_FILE_LEN, stdin);
                sscanf(input, "%[^\n]", srcDest);

                printf("Enter path of mods folder: ");
                fgets(input, MAX_FILE_LEN, stdin);
                sscanf(input, "%[^\n]", modDest);


                if(validatePath(srcDest) == 0 && validatePath(modDest) == 0){
                    invalid = false;
                }
                else{
                    printf("\nOne of the paths entered are invalid\n");
                    invalid = true;
                    }
            }   while(invalid);
        }
    }

    if(loaded == false){
        printf("\nWould you like to save these paths? [y/n]: ");
        scanf(" %c", &choice);
    }

    if(choice == 'y' || choice == 'y') {
        savePaths(srcDest, modDest);
    }

    char **modFiles = getFiles(modDest);

    if(modAmount == 1){
        printf("\n%d mod has been found\n", modAmount);
    } else{
        printf("\n%d mods have been found\n", modAmount);
    }
    
    
    char *result = malloc(MAX_FILE_LEN*sizeof(char)); 
    char **destPaths = malloc(MAX_MODS*sizeof(char*));

    for(int i=0; i<modAmount; i++){
        destPaths[i] = malloc(MAX_FILE_LEN*sizeof(char));
        findPath(srcDest, modFiles[i], result);
        strcpy(destPaths[i], result);
                
    }

    free(result);

    for(int i=0; i<modAmount; i++){
        char modPath[MAX_FILE_LEN];
        strcpy(modPath, modDest);
        strcat(modPath, "/");
        strcat(modPath, modFiles[i]);
        copyAndReplace(modPath, destPaths[i]);
        printf("\nReplaced %s", destPaths[i]);

        free(modFiles[i]);
        free(destPaths[i]);

    }

    free(destPaths);
    free(modFiles);


    
    
}




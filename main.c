#include <stdio.h>
#include <stdbool.h>
#include <string.h>

long int findSize(FILE* fp);
long int getNumLines(FILE* fp);
long int getNumWords(FILE* fp);
long int getNumChars(FILE* fp);
void comparison(FILE* fp, char* option, long int result[3]);
bool checkFile(char* fileName);
bool checkOption(int argc, char* argv[]);

int main(int argc, char* argv[]){

    if (argc > 3){
        printf("Invalid command\n");
        return 1;
    }

    FILE* fp = stdin;
    long int result[3] = {-1, -1, -1};
    if (argc == 1){
        comparison(fp, "", result);
        printf("%ld %ld %ld %s\n", result[0], result[1], result[2], "stdin");
        return 0;
    }

    bool fileProvided = checkFile(argv[argc-1]);
    if (fileProvided == true){
        fp = fopen(argv[argc-1], "r");
    }

    if (fp == NULL){
        printf("File not found\n");
        return -1;
    }

    bool optionProvided = checkOption(argc, argv);

    if (optionProvided == false && fileProvided == false){
        printf("Invalid command\n");
        return 1;
    }

    if (argc == 2 && fileProvided == true){
        comparison(fp, "", result);
        printf("%ld %ld %ld %s\n", result[0], result[1], result[2], argv[argc-1]);
    }

    if (argc == 2 && optionProvided == true){
        comparison(fp, argv[1], result);
        printf("%ld %s\n", result[0], "stdin");
    }

    if (argc == 3){
        comparison(fp, argv[1], result);
        printf("%ld %s\n", result[0], argv[argc-1]);
    }

    fclose(fp);
    return 0;
}


void comparison(FILE* fp, char* option, long int result[3]){

    if (strcmp(option, "-c") == 0){
        result[0] = findSize(fp);
    }

    else if (strcmp(option, "-l") == 0){
        result[0] = getNumLines(fp);
    }

    else if (strcmp(option, "-w") == 0){
        result[0] = getNumWords(fp);
    }

    else if (strcmp(option, "-m") == 0){
        result[0] = getNumChars(fp);
    }

    else if (strcmp(option, "") == 0){
        result[0] = findSize(fp);
        result[1] = getNumLines(fp);
        result[2] = getNumWords(fp);
    }
}

long int getNumChars(FILE* fp){

    long int size = findSize(fp);
    char fileData[size+1];

    rewind(fp);
    size_t characters = fread(fileData, 1, size, fp);
    
    if (ferror(fp)){
        printf("Error reading file\n");
        return -1;
    }

    return characters;
}

long int getNumWords(FILE* fp){

    int inWord = 0;
    int c;
    long unsigned int count = 0;

    rewind(fp);
    while ((c = fgetc(fp)) != EOF){
        if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)){
            if (inWord == 0){
                inWord = 1;
                count++;
            }
        }
        else {
            inWord = 0;
        }
    }

    if (ferror(fp)){
        printf("Error reading file\n");
        return -1;
    }

    return count;
}


long int getNumLines(FILE* fp){

    long int size = findSize(fp);
    long int count = 0;
    char fileData[size+1];

    rewind(fp);
    while (fgets(fileData, size+1, fp) != NULL){      // use getline from glibc instead of fgets but I can't get it working
        count++;
    }

    if (ferror(fp)){
        printf("Error reading file\n");
        return -1;
    }

    return count;
}


long int findSize(FILE* fp){

    fseek(fp, 0, SEEK_END);

    long int size = ftell(fp);

    return size;
}


bool checkOption(int argc, char* argv[]){

    char* options[] = {"-c", "-l", "-w", "-m"};
    for (int i = 0; i < 4; i++){
        if (strcmp(argv[1], options[i]) == 0){
            return true;
        }
    }

    return false;
}

bool checkFile(char* fileName){
    int fileNameLen = strlen(fileName);
    
    // check if a file was provided
    if (fileNameLen < 5){
        return false;
    }
    // retrieve the extension of the file
    char extension[5];
    extension[0] = (char) fileName[fileNameLen - 4];
    extension[1] = (char) fileName[fileNameLen - 3];
    extension[2] = (char) fileName[fileNameLen - 2];
    extension[3] = (char) fileName[fileNameLen - 1];

    // check if the file is a .txt file
    if (strncmp(extension, ".txt", 4) != 0){
        return false;
    }

    return true;
}
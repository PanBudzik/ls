#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

void displayExtendedSingleRow(char *filename){
        
    struct stat fs;

                int r = stat(filename,&fs);
                if( r==-1 )
                {
                    fprintf(stderr,"File error\n");
                    printf("%s",filename);
                    exit(1);
                }
                if( S_ISREG(fs.st_mode) )//file
                    printf("-");
                else if(S_ISDIR(fs.st_mode))//direcory
                    printf("d");
                else if(S_ISBLK(fs.st_mode))//block device( hard discs, flipflobisc, cd-rom etc)
                    printf("b");
                else if(S_ISCHR(fs.st_mode))//character device( keyboard, mouse, soundcard, controler etc )
                    printf("c");
                else if(S_ISFIFO(fs.st_mode))//named pipeline (fifo)
                    printf("p");
                else if(S_ISLNK(fs.st_mode))//symbolic link (pointer to another filename)
                    printf("l");
                else
                    printf("s");//socket(bi-directional transfer mechanism)

                
                /*Owner permissions*/
                if( fs.st_mode & S_IRUSR )
                    printf("r");
                else 
                    printf("-"); 
                if( fs.st_mode & S_IWUSR )
                    printf("w");
                else 
                    printf("-");
                if( fs.st_mode & S_IXUSR )
                    printf("x");
                else 
                    printf("-");


                /*Group permissions:*/
                if( fs.st_mode & S_IRGRP )
                    printf("r");
                else 
                    printf("-");
                if( fs.st_mode & S_IWGRP )
                    printf("w");
                else 
                    printf("-");
                if( fs.st_mode & S_IXGRP )
                    printf("x");
                else 
                    printf("-");

                /*Others permissions*/
                if( fs.st_mode & S_IROTH )
                    printf("r");
                else 
                    printf("-");
                if( fs.st_mode & S_IWOTH )
                    printf("w");
                else 
                    printf("-");
                if( fs.st_mode & S_IXOTH )
                    printf("x");
                else 
                    printf("-");

                printf(" ");


                /*Nuber of hard pointers (pointers to file)*/
                printf("%lu",fs.st_nlink);
                printf(" ");
                
                
                /*Owner and group*/
                struct passwd *pw = getpwuid(fs.st_uid);
                struct group  *gr = getgrgid(fs.st_gid);

                printf("%s",pw->pw_name);
                printf(" ");
                printf("%s",gr->gr_name);
                printf(" ");

                /*Size in Bytes*/
                printf("%ld",fs.st_size);
                printf(" ");

                /*Last modification of file*/
                struct tm *info;
                info = localtime(&fs.st_mtime);
                char buffer[26];
                strftime(buffer, 26, "%B %d %H:%M", info);
                printf("%s",buffer);
                printf(" ");

                /*Name of file*/
                if(strrchr(filename, '/')){
                    int i=strrchr(filename, '/')-filename+1;
                    while (filename[i]!='\0')
                    {
                        printf("%c",filename[i]);  
                        i++;
                    }
                }
                else{
                printf("%s",filename);
                }
                printf("\n");
}

void listDierctory(char *directoryName){
    DIR *pDIR;
    struct dirent *pDirEnt;
    pDIR = opendir(directoryName);
    if (pDIR == NULL) {
        fprintf(stderr, "%s %d: opendir() failed (%s)\n",
                __FILE__, __LINE__, strerror(errno));
        return;
    }
    while ((pDirEnt = readdir(pDIR)) != NULL) {
            if(pDirEnt->d_name[0]!='.'&&pDirEnt->d_name[0]!='\0'){
            printf("%s\n", pDirEnt->d_name);
            }
        }
        closedir(pDIR);
}

void listDirectoriesRecursively(char *directoryName) {
    DIR *pDIR;
    struct dirent *pDirEnt;

    pDIR = opendir(directoryName);
    if (pDIR == NULL) {
        fprintf(stderr, "%s %d: opendir() failed (%s)\n",
                __FILE__, __LINE__, strerror(errno));
        return;
    }

    while ((pDirEnt = readdir(pDIR)) != NULL) {
        // if (strcmp(pDirEnt->d_name, ".") == 0 || strcmp(pDirEnt->d_name, "..") == 0) {
        //     continue;  // Skip current directory and parent directory entries
        // }

        char fullPath[PATH_MAX];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", directoryName, pDirEnt->d_name);

        struct stat fs;
        int r = stat(fullPath, &fs);
        if (r == -1) {
            //fprintf(stderr, "File error: %s\n", fullPath);
            continue;
        }

        if (S_ISDIR(fs.st_mode)&&pDirEnt->d_name[0]!='.') {
            // It's a directory, print its contents and recurse into it
            printf("%s:\n", fullPath);

            listDierctory(fullPath);
            printf("\n");
            
            listDirectoriesRecursively(fullPath);
        }
    }

    closedir(pDIR);
}

int main(int argc, char *argv[]) {
    int c;
    char lFlag = 0;
    char RFlag = 0;
    char cFlag = 0;
    char *options = "lRc:";
    char* directoryName = "";
    DIR *pDIR;
    struct dirent *pDirEnt;

    if (argc >= 2 && argv[1][0] != '-') {
        directoryName = argv[1];
    
    } else {
        directoryName = ".";
    }
    pDIR = opendir(directoryName);

    if (pDIR == NULL) {
        printf("%s", argv[1]);
        fprintf(stderr, "%s %d: opendir() failed (%s)\n",
                __FILE__, __LINE__, strerror(errno));
        exit(-1);
    }

    while ((c = getopt(argc, argv, options)) != -1) {
        switch (c) {
            case 'l':
                lFlag = 1;
                break;
            case 'R':
                RFlag = 1;
                break;
            case 'c':
                cFlag = 1;
                printf("%s", optarg);
                break;
            case '?':
                if (optopt == 'c')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                abort();
        }
    }

    if (lFlag) {
        int dirLen = strlen(directoryName);
    int fullLen;

    while ((pDirEnt = readdir(pDIR)) != NULL) {
        if(pDirEnt->d_name[0]!='.'){
        int fileLen = strlen(pDirEnt->d_name);
        fullLen = dirLen + fileLen + 2;

        char *fullPath = (char *)malloc(fullLen * sizeof(char));
        if (fullPath == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(1);
        }

        strcpy(fullPath, directoryName);
        strcat(fullPath, "/");
        strcat(fullPath, pDirEnt->d_name);

        displayExtendedSingleRow(fullPath);

        free(fullPath);
        }
    }
    }
    else if(RFlag){
        printf(".:\n");
        listDierctory(directoryName);
        listDirectoriesRecursively(directoryName);

    } else {
        while ((pDirEnt = readdir(pDIR)) != NULL) {
            if(pDirEnt->d_name[0]!='.'){
            printf("%s\n", pDirEnt->d_name);
            }
        }
    }

    closedir(pDIR);

    return 0;
}
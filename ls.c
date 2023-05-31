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

int main(int argc, char *argv[])
{

    int c;
    struct stat fs;
    

    if (argc == 1)
    {
        DIR *pDIR;
        struct dirent *pDirEnt;
        pDIR = opendir(".");
        if (pDIR == NULL)
        {
            fprintf(stderr, "%s %d: opendir() failed (%s)\n",
                    __FILE__, __LINE__, strerror(errno));
            exit(-1);
        }
        pDirEnt = readdir(pDIR);
        while (pDirEnt != NULL)
        {
            printf("%s\n", pDirEnt->d_name);
            printf("%s\n", pDirEnt->d_name);
            printf("%s\n", pDirEnt->d_name);
            pDirEnt = readdir(pDIR);
        }
        closedir(pDIR);
    }
    else
    {
        while ((c = getopt(argc, argv, "lRc:")) != -1)
            switch (c)
            {
            case 'l':
                char *filename = argv[1];
                int r = stat(filename,&fs);
                if( r==-1 )
                {
                    fprintf(stderr,"File error\n");
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
                printf(" ");
                
                break;
                

            case 'R':
                printf("R");
                break;
            case 'c':
                printf("%s",optarg);
                break;
            case '?':
                if (optopt == 'c')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                fprintf (stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);
                return 1;
            default:
                abort();
            }
    }
    return 0;
}

// int
// main (int argc, char **argv)
// {
//   int aflag = 0;
//   int bflag = 0;
//   char *cvalue = NULL;
//   int index;
//   int c;

//   opterr = 0;

//   if(argc==1)
//   {
//   while ((c = getopt (argc, argv, "abc:")) != -1)
//     switch (c)
//       {
//       case 'a':
//         aflag = 1;
//         break;
//       case 'b':
//         bflag = 1;
//         break;
//       case 'c':
//         cvalue = optarg;
//         break;
//       case '?':
//         if (optopt == 'c')
//           fprintf (stderr, "Option -%c requires an argument.\n", optopt);
//         else if (isprint (optopt))
//           fprintf (stderr, "Unknown option `-%c'.\n", optopt);
//         else
//           fprintf (stderr,
//                    "Unknown option character `\\x%x'.\n",
//                    optopt);
//         return 1;
//       default:
//         abort ();
//       }

//   printf ("aflag = %d, bflag = %d, cvalue = %s\n",
//           aflag, bflag, cvalue);

//   for (index = optind; index < argc; index++)
//     printf ("Non-option argument %s\n", argv[index]);
//   return 0;
// }

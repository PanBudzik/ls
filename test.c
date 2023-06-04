#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int opt;
    int l_flag = 0;
    
    // Process command-line options using getopt()
    while ((opt = getopt(argc, argv, "l")) != -1) {
        switch (opt) {
            case 'l':
                l_flag = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-l] [path]\n", argv[0]);
                return 1;
        }
    }
    
    // Process non-option arguments
    if (optind < argc) {
        // If there are non-option arguments, process them here
        printf("Path: %s\n", argv[optind]);
    }
    
    // Perform actions based on the options and arguments
    if (l_flag) {
        printf("Listing with -l option\n");
    }
    
    // Rest of your ls implementation goes here
    
    return 0;
}

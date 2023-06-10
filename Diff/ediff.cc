/*************************************************************
 *
 * ediff - ediff - "Easy Diff"
 *   simple text comparison using only system calls
 *
 * PUT OTHER INFORMATION ABOUT YOU AND ETC...
 * this is the version to start with...
 *
 * Student: Zac Baker
 * Date: 02/03/23
 * Class: CS 4420
 * 
 * 
 ************************************************************/

/* include all of the header files that the manual pages say we need */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

// forward declarations for local routines 
// these are just suggestions, but organizing your programing using
// these functions will save you a LOT of time
static void Usage(const char error[]);
static void Process(void);
static int ReadChar(int df, unsigned pos);
static int SameLine(int fd1, int fd2, int pos1, int pos2);
static void PrintLine(int fd, unsigned pos);
static int NextLinePos(int fd, unsigned pos);

/* global variables */
static int debug = 0;
char *progname;

int l_fd, r_fd; /* file descriptors for the files to compare */
char *filename1 = NULL; /* file argument1 */
char *filename2 = NULL; /* file argument2 */
int pos_l = 0;//
int pos_r = 0;
int p_tmp = 0;
int l_print_ptr = 0;
int r_print_ptr = 0;



/*************************************************************
 **
 ** Main program, usage "ediff file1 file2"
 **
 *************************************************************/
int main(int argc,char *argv[]){
    
    
    progname = argv[0];

    //Arg error checking. 
    if (argc == 3){
        filename1 = argv[1];
        filename2 = argv[2];
    }else if (argc == 4){    
        if ( strcmp(argv[1],"-d") == 0){
            debug = 1;
        }else{
            fprintf(stderr, "usage: ./ediff [-d] filename1 filename2\n");
            exit(1);
        }
        filename1 = argv[2];
        filename2 = argv[3];

    }else{
        fprintf(stderr,"Unexcpected Number of arguments! Expects: ediff [-d] <file1> <files2>\n");
        exit(1);
    }  

    if(debug)
        for (int i = 1; i < argc; ++i)
        { 
            printf("Num args is: %d Argv<%d> = '%s'\n",argc, i, argv[i]);
        }

    
    
    if (debug)
        printf("Debug is set to %d\n", debug);

    // open the files specified and check for errors, etc...

    l_fd = open(filename1, O_RDONLY);
    if (l_fd == -1){
        perror(filename1);
        exit(2);
    }

    r_fd = open(filename2, O_RDONLY);
    if (r_fd == -1){
        perror(filename2);
        exit(2);
    }

    Process(); /* do the real work */

    exit(0); /* everything must have worked */

    // call the recommended subroutines so this will compile...
    // your code will be very different than these calls...
    Usage("wrong files");
    (void) ReadChar(5,15);
    (void) SameLine(1,2,3,4);
    (void) PrintLine(1,2);
    (void) NextLinePos(0,1);
}

/*************************************************************
 **
 ** Process: do the work
 **
 *************************************************************/
static void Process(){

    unsigned int line_number = 1;
    int diffs = 0;
    char c;
    while(1){ 

    if(debug) printf("This is the start of Line compare loop\n");

    int r_status = read(r_fd,&c,1);
    int l_status = read(l_fd,&c,1);
    if(debug) printf("File status L<%i> R<%i> \n", l_status, r_status);
    
    if (r_status != 1 && l_status != 1) break; //break loop condition

    int no_match = SameLine(l_fd,r_fd,pos_l,pos_r);
    if(debug){ 
            if(no_match){
                printf("Lines didn't match\n");
            }else printf("Lines matched \n");
            printf ("Left line position is <%i>\nRight line position is  <%i>\n",pos_l,pos_r);
        }
    
    if(l_status == 0 || r_status ==0){
        
        if(l_status == 1){
            diffs++;
            printf("%iA\n< ",line_number-1);
            fflush(stdout);
            PrintLine(l_fd, l_print_ptr);
            pos_l = p_tmp+1;
        }else if (r_status == 1){
            diffs++;
            printf("%iA\n> ",line_number-1);
            fflush(stdout);
            PrintLine(r_fd, r_print_ptr);
            pos_r = p_tmp+1;
        }else {
            break;
        }

    }else{
    
        if (no_match){
            diffs++;
            printf("%ic%i\n< ",line_number,line_number);
            fflush(stdout);
            PrintLine(l_fd, l_print_ptr);
            pos_l = p_tmp+1;
            printf("---\n> ");
            fflush(stdout);
            PrintLine(r_fd, r_print_ptr);
            pos_r = p_tmp+1;
        }
    }
    line_number++;
    }
    if(debug) printf("lines in file <%u>\n", line_number);
     if (diffs == 0)
        printf("Files %s and %s are identical\n", filename1, filename2);

}


/*************************************************************
 **
 ** SameLine: are two lines the same??
 ** returns 0 for lines match
 ** returns 1 for lines fi
 *************************************************************/
static int SameLine(int l_fd,int r_fd,int pos1,int pos2){

    if (debug) printf("Comparing lines at pos %d and %d\n", pos1, pos2);
    l_print_ptr = pos1;
    r_print_ptr = pos2;

    int no_match = 0;
    

    while(1){
        
        char l_chr = ReadChar(l_fd, pos1);
        char r_chr = ReadChar(r_fd, pos2);
        
        if(debug)printf("left char: <%c>\nright char:   <%c>\n", l_chr, r_chr);
        
        if (l_chr != r_chr){
            no_match = 1;
            if (pos1 != -1)pos1 = NextLinePos(l_fd, pos1);
            if (pos2 != -1)pos2 = NextLinePos(r_fd, pos2);
            if(debug) printf("Lines of been moved to next line position L<%i>R<%i>\n",pos1,pos2);
            break;
        }
        
        pos1++;
        pos2++;

        if(l_chr == '\n' && r_chr == '\n')break;

    }

    pos_l = pos1;
    pos_r = pos2;

    return (no_match); /* match */
}

/*************************************************************
 **
 ** NextLinePos: what is the file offset of the next 
 **    character beyond the newline, starting at "pos"
 ** returns EOF if there are no characters after the current line
 **
 *************************************************************/
static int NextLinePos(int fd, unsigned pos){

    if (debug )
        printf("NextLinePos(%d, %u) called\n", fd, pos);

    while(ReadChar(fd, pos) != '\n'){
        pos++;
        if(ReadChar(fd, pos) == EOF){
            if(debug) printf("Next line is EOF\n");
            return (EOF);
        }

    }

    if (debug) printf("The next line pos is <%u>\n",pos);

    return (pos);
}

/*************************************************************
 **
 ** PrintLine: print the line starting at pos
 **
 *************************************************************/
static void PrintLine(int fd, unsigned pos){

    if (debug) printf("Printing line from fd %d at position %u\n", fd, pos);

    p_tmp = pos; 

    while(1){
        char f_chr = ReadChar(fd, p_tmp);

        if (f_chr == EOF){
            if(debug) printf("EOF\n");
            break;
        }

        int w_check = write(1,&f_chr,1);
        if (w_check == -1){
            perror("./ediff");
            exit(5);
        }

        if (f_chr == '\n'){
            if(debug) printf("Line Ends\n");
            break;
        }
        p_tmp++;
        
    }
    
}


/*************************************************************
 **
 ** ReadChar: read and return the char at position pos
 **    from the given descriptor given
 ** returns constant EOF on end of file
 **
 *************************************************************/
static int ReadChar(int fd,unsigned pos){
   // do a seek, read that character, check for EOF, then return
   // the character read (or EOF if appropriate)
    char ch;
    lseek(fd,pos,SEEK_SET);
    int red =read(fd,&ch,1);
    if (red == 0) {
        if(debug) printf("EOF reached in ReadChar()\n");
        ch = EOF;
    }else if (red != 1) {
        perror("read");
        exit(3);
    }

    if(debug){
        
        if (ch == '\n')printf("New line found\n");
    }
   return(ch);
}


/*************************************************************
 **
 ** Usage: bad argument, tell how to call the program
 **
 *************************************************************/
static void Usage(const char error[])
{
    if (error) printf("%s\n", error);
    printf("usage: %s [-d] filename1 filename2\n", progname);
    exit(-1);
}

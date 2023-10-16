/**
 * Project 1
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
static inline int isNumber(char *);
//MY STRUCT
/*struct rlentry{

    char label[MAXLINELENGTH];
    char opcode[MAXLINELENGTH];
    int offset; //PC #

};*/

//RELOCATION TABLE ARRAYS
char rlabels[1000][MAXLINELENGTH];
char ropcodes[1000][MAXLINELENGTH];
int roffsets[1000];
//SYMBOL TABLE ARRAYS
char slabels[1000][MAXLINELENGTH];
char sletters[1000];
int soffsets[1000];

/*struct sentry{

    char label[MAXLINELENGTH];
    char letter;
    int offset; //PC #...?

};*/

int
main(int argc, char **argv)
{
    
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
            arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

    if (argc != 3) {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
            argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];
    int PCcounter = 0; //PC COUNTER

    inFilePtr = fopen(inFileString, "r");
    if (inFilePtr == NULL) {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    char labels[1000][MAXLINELENGTH];
    int arrsize = 0;
    int track2 = 0;
    int l = 0;
    int j = 0;
    int tcount = 0, dcount = 0, scount = 0, rcount = 0; //all counters for header
    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){
        
        //store labels in the first pass
        //do I use PC counter for strcpy?
        //char labels[10][MAXLINELENGTH];//2D char array
        if(!strcmp(opcode, "lw") | !strcmp(opcode, "sw")){
            
            if(!isNumber(arg2)){
                
                if((arg2[0] > 64) && (arg2[0] < 91)){ //Checking if uppercase

                    int track2 = 0;
                    for(int y = 0; y < scount; ++y){
                        if((!strcmp(slabels[y], arg2))){
                            track2 = 1;
                        }
                    }

                    if(track2 == 0){//if it is NOT found...
                        strcpy(slabels[j], arg2);
                        sletters[j] = 'U'; 
                        soffsets[j] = 0;
                        ++j;
                        ++scount;
                    }
                }
                
                //STORE IN RELOCATION TABLE
                strcpy(rlabels[l], arg2);
                strcpy(ropcodes[l], opcode);

                roffsets[l] = PCcounter;
                
            ++rcount;
            ++l;
            }

            ++tcount;

        }else if(!strcmp(opcode, ".fill")){

            if(!isNumber(arg0)){
                strcpy(rlabels[l], arg0);
                strcpy(ropcodes[l], opcode);
                roffsets[l] = PCcounter - tcount;
                ++rcount;
                ++l;

                if((arg0[0] > 64) && (arg0[0] < 91)){ //Checking if uppercase

                    int track3 = 0;
                    for(int y = 0; y < scount; ++y){
                        if((!strcmp(slabels[y], arg0))){
                            track3 = 1;
                            }
                    }

                    if(track3 == 0){//if it is NOT found...
                        strcpy(slabels[j], arg0);
                        sletters[j] = 'U'; 
                        soffsets[j] = 0;
                        ++j;
                        ++scount;
                    }
                }
            }

        ++dcount;

        }else{
    
            ++tcount;

        }

        if(strlen(label) != 0){
                for(int i = 0; i < arrsize; ++i){
                    if(!strcmp(labels[i], label)){
                
                        exit(1); //DUPLICATE LABEL
                    }
                }
        }
        
        int track = 0;//a tracker I'll use to see if a label has been overwritten.
        if((label[0] > 64) && (label[0] < 91)){//if uppercase and a global variable

            //check that label is not already in array. If it is, then overwrite.
            for(int s = 0; s < scount; ++s){

                if((!strcmp(slabels[s], label)) && (sletters[s] == 'U')){
                    if (!strcmp(opcode, ".fill")){
                        sletters[s] = 'D'; 
                        soffsets[s] = PCcounter - tcount;
                    //sletters[j] = 'D';
                        }else{                  
                        sletters[s] = 'T'; 
                        soffsets[s] = PCcounter;
                    //sletters[j] = 'T';
                        }
                        track = 1;
                }   
            }

            if(track == 0){
                strcpy(slabels[j], label);

                if (!strcmp(opcode, ".fill")){
                    sletters[j] = 'D'; 
                    soffsets[j] = PCcounter - tcount;
                    //sletters[j] = 'D';
                }else{                  //will handle Undefined in a different way.
                    sletters[j] = 'T'; 
                    soffsets[j] = PCcounter;
                //sletters[j] = 'T';
                }

                ++scount;
                ++j;
            }
        }



        strcpy(labels[PCcounter], label);//copies the label to a spot in the array
        
        track = 0;
        ++PCcounter;
        ++arrsize;
    }

    //OUTPUT HEADER HERE.
    fprintf(outFilePtr, "%d", tcount);
    fprintf(outFilePtr, "%s"," ");
    fprintf(outFilePtr, "%d", dcount);
    fprintf(outFilePtr, "%s"," ");
    fprintf(outFilePtr, "%d", scount);
    fprintf(outFilePtr, "%s"," ");
    fprintf(outFilePtr, "%d\n", rcount);

    //if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {


    /* this is how to rewind the file ptr so that you start reading from the
        beginning of the file */
    rewind(inFilePtr);
    PCcounter = 0; //RESET PC COUNTER

    while(readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2)){

        int offsetval = arrsize + 1;
        //first, check if offset is a number.

        //CHECK IF REGISTERS ARE INTS.
        if((!isNumber(arg0) | !isNumber(arg1)) & ((strcmp(opcode, ".fill")) | 
        strcmp(opcode, "noop") | strcmp(opcode,"halt"))){
         
            exit(1);
        }


        if((isNumber(arg0) & isNumber(arg1)) & (!strcmp(opcode, "beq")) | (!strcmp(opcode, "lw")) |
        (!strcmp(opcode, "beq")) | (!strcmp(opcode, "sw")) | (!strcmp(opcode, "add")) 
        | (!strcmp(opcode, "nor")) | (!strcmp(opcode, "jalr"))){

            if((atoi(arg0) > 7) | (atoi(arg1) > 7) | (atoi(arg0) < 0) | (atoi(arg1) < 0)){
                exit(1);
            }
            
        }



        if(!strcmp(opcode,"add") | !strcmp(opcode, "nor")){

                    if((isNumber(arg2)) & ((atoi(arg2) < 0) | (atoi(arg2)) > 7)){
                        
                            exit(1);

                    }
                }

   
        
    
        if (!strcmp(opcode, "add")) {
            int totsum = 0;// add is 000
            int arg0num = atoi(arg0) << 19;
            int arg1num = atoi(arg1) << 16;
            int arg2num = atoi(arg2);
            totsum += arg0num + arg1num + arg2num;

            fprintf(outFilePtr, "%d\n", totsum);
            /*printf("%d", totsum);
            printf("\n");*/
    
        }

        else if (!strcmp(opcode, "nor")) {
            int totsum = 0b0010000000000000000000000;//is this correct?
            int arg0num = atoi(arg0) << 19;
            int arg1num = atoi(arg1) << 16;
            int arg2num = atoi(arg2);
            totsum += arg0num + arg1num + arg2num;
            fprintf(outFilePtr, "%d\n", totsum);
            /*printf("%d", totsum); 
            printf("\n");*/
      
        }

        else if (!strcmp(opcode, "lw")) {
            int totsum = 0b0100000000000000000000000;//is this correct?
            int arg0num = atoi(arg0) << 19;
            int arg1num = atoi(arg1) << 16;
        
        
            if(isNumber(arg2)){
                //IF DOESN'T FIT IN 16 BITS
                if((atoi(arg2) < -32768) | (atoi(arg2) > 32767)){
                    exit(1);
                }

                int arg2num = atoi(arg2);
                arg2num = arg2num & 0b1111111111111111;
                totsum += arg0num + arg1num + arg2num;
                
            }else{

                for(int i = 0; i < arrsize; ++i){
                    if(!strcmp(labels[i], arg2)){

                        offsetval = i;
                    }
                }

                if(offsetval == arrsize + 1){//LABEL WAS NOT FOUND
                    if((arg2[0] > 64) && (arg2[0] < 91)){
                        offsetval  = 0;
                    }else{
                        exit(1);
                    }
                }

                totsum += arg0num + arg1num + offsetval;
            }

            //totsum += arg0num + arg1num + offsetval;
            
         
            fprintf(outFilePtr, "%d\n", totsum);
            /*printf("%d", totsum);
            printf("\n");*/
        
        }

        else if (!strcmp(opcode, "sw")) {
            int totsum = 0b0110000000000000000000000;//is this correct?
            int arg0num = atoi(arg0) << 19;
            int arg1num = atoi(arg1) << 16;

           

            if(isNumber(arg2)){
                //IF DOESN'T FIT IN 16 BITS
                if((atoi(arg2) < -32768) | (atoi(arg2) > 32767)){
                    exit(1);
                }

                int arg2num = atoi(arg2);
                arg2num = arg2num & 0b1111111111111111;
                totsum += arg0num + arg1num + arg2num;

            }else{
                for(int i = 0; i < arrsize; ++i){
                    if(!strcmp(labels[i], arg2)){

                        offsetval = i;
                    }
                }

                if(offsetval == arrsize + 1){//LABEL WAS NOT FOUND

                    if((arg2[0] > 64) && (arg2[0] < 91)){
                        offsetval  = 0;
                    }else{
                        exit(1);
                    }
                }

                totsum += arg0num + arg1num + offsetval;
            }

            fprintf(outFilePtr, "%d\n", totsum);
            /*printf("%d", totsum); //not sure abt this
            printf("\n");*/
     
        
        }

        else if (!strcmp(opcode, "beq")) { //NOT DONE
            int totsum = 0b1000000000000000000000000;
            int arg0num = atoi(arg0) << 19;
            int arg1num = atoi(arg1) << 16;
           
            if(isNumber(arg2)){
                //IF DOESN'T FIT IN 16 BITS
                if((atoi(arg2) < -32768) | (atoi(arg2) > 32767)){
                    exit(1);
                }
                int arg2num = atoi(arg2);
                arg2num = arg2num & 0b1111111111111111;//to chop off 16 bits
                totsum += arg0num + arg1num + arg2num;
            }else{
                for(int i = 0; i < arrsize; ++i){
                    if(!strcmp(labels[i], arg2)){

                        offsetval = i;
                    }
                }

                if(offsetval == arrsize + 1){//LABEL WAS NOT FOUND
                    exit(1);
                }

                uint16_t newoff = offsetval - PCcounter - 1;

                newoff = ~newoff;//need to XOR.
        
                newoff = ~newoff;
         
                totsum += arg0num + arg1num + newoff;
            }

            fprintf(outFilePtr, "%d\n", totsum);
            /*printf("%d", totsum);
            printf("\n");*/
        
        }

        else if (!strcmp(opcode, "noop")) {
            int totsum = 0b1110000000000000000000000;
            fprintf(outFilePtr, "%d\n", totsum);
            /*printf("%d", totsum);
            printf("\n");*/
   
        }

        else if (!strcmp(opcode, "halt")) {
            int totsum = 0b1100000000000000000000000;
            fprintf(outFilePtr, "%d\n", totsum);
            /*printf("%d", totsum);
            printf("\n");*/
        }

        else if(!strcmp(opcode, "jalr")){
            int totsum = 0b1010000000000000000000000;
            int arg0num = atoi(arg0) << 19;
            int arg1num = atoi(arg1) << 16;

            totsum += arg0num + arg1num;

            fprintf(outFilePtr, "%d\n", totsum);
        }

        else if (!strcmp(opcode, ".fill")) {

            if(isNumber(arg0)){
             

                fprintf(outFilePtr, "%d\n", atoi(arg0));
                /*printf("%d", atoi(arg0));
                printf("\n");*/

            }else{
                int offy = arrsize + 1;
                
                for(int i = 0; i < arrsize; ++i){//watch out for arrsize here
                    if(!strcmp(labels[i], arg0)){

                        if(offy != arrsize + 1){
                                exit(1);
                        }

                        offy = i;
    
                }
            }

            if(offy == arrsize + 1){//LABEL WAS NOT FOUND
                if((arg0[0] > 64) && (arg0[0] < 91)){
                        offy = 0;
                    }else{
                        exit(1);
                    }
            }

            fprintf(outFilePtr, "%d\n", offy);
            //printf("%d", offsetval);
            //printf("\n");
            }
        }else{//UNRECOGNIZED OPCODE

            exit(1);
        }

        ++PCcounter;
    }

    //PRINT STABLE & RTABLE STUFF HERE
    for(int k = 0; k < scount; ++k){
        fprintf(outFilePtr, "%s", slabels[k]);
        fprintf(outFilePtr, "%s"," ");
        fprintf(outFilePtr, "%c", sletters[k]);
        fprintf(outFilePtr, "%s"," ");
        fprintf(outFilePtr, "%d\n", soffsets[k]);
    }

    for(int m = 0; m < rcount; ++m){
        fprintf(outFilePtr, "%d", roffsets[m]);
        fprintf(outFilePtr, "%s"," ");
        fprintf(outFilePtr, "%s", ropcodes[m]);
        fprintf(outFilePtr, "%s"," ");
        fprintf(outFilePtr, "%s\n", rlabels[m]);
    }

    return(0);
}




/*
* NOTE: The code defined below is not to be modifed as it is implimented correctly.
*/

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int
readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
    char *arg1, char *arg2)
{
    char line[MAXLINELENGTH];
    char *ptr = line;

    /* delete prior values */
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    /* read the line from the assembly-language file */
    if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
	/* reached end of file */
        return(0);
    }

    /* check for line too long */
    if (strlen(line) == MAXLINELENGTH-1) {
	printf("error: line too long\n");
	exit(1);
    }

    // Treat a blank line as end of file.
    // Arguably, we could just ignore and continue, but that could
    // get messy in terms of label line numbers etc.
    char whitespace[4] = {'\t', '\n', '\r', ' '};
    int nonempty_line = 0;
    for(size_t line_idx = 0; line_idx < strlen(line); ++line_idx) {
        int line_char_is_whitespace = 0;
        for(int whitespace_idx = 0; whitespace_idx < 4; ++ whitespace_idx) {
            if(line[line_idx] == whitespace[whitespace_idx]) {
                ++line_char_is_whitespace;
                break;
            }
        }
        if(!line_char_is_whitespace) {
            ++nonempty_line;
            break;
        }
    }
    if(nonempty_line == 0) {
        return 0;
    }

    /* is there a label? */
    ptr = line;
    if (sscanf(ptr, "%[^\t\n ]", label)) {
	/* successfully read label; advance pointer over the label */
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line.  Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]",
        opcode, arg0, arg1, arg2);

    return(1);
}

static inline int
isNumber(char *string)
{
    int num;
    char c;
    return((sscanf(string, "%d%c",&num, &c)) == 1);
}


// maximum size of the 2 dimensional path matrix
#define MAXDIM 200

// how much of the path matrix we're actually using
extern int dimension;

// variables and data structures shared by everybody
extern int debug;
extern int num_procs, proc_rank;
extern bool g[MAXDIM][MAXDIM];

// global routines
bool hamCycle();



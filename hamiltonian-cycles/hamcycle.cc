#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <mpi.h> 
#include "hamcycle.h"

using namespace std;

// shared global path for efficiency
int *path = NULL;

bool isSafe(int n, int pos) {
   if (g [path[pos-1]][n] == 0) {
      return false;
   }

   for (int i = 0; i < pos; i++) {
      if (path[i] == n)
         return false;
   }
   return true;
}

bool hamiltonianCycle(int pos) {
   //If all vertices are included in Hamiltonian Cycle
   
   
   
   if (pos == dimension) {
      return(g[ path[pos-1] ][ path[0] ] == 1);  // can I get from end of path back to beginning?
   }
   for (int n = 1; n < dimension; n++) {
      int sum_at4 = proc_rank;
         if (pos == 4){
            sum_at4 =0;
            for(int i = 0; i < pos; i++){
               sum_at4 += path[i];

            }
            sum_at4 = sum_at4 % num_procs;
         }
      

      if (isSafe(n, pos)) { //Check if this vertex can be added to Hamiltonian Cycle   
         path[pos] = n;
         
         //recur to construct rest of the path
         if(sum_at4 == proc_rank){
            if (hamiltonianCycle (pos+1) == true )
               return true;
            path[pos] = -1; //remove vertex if it doesn’t lead to the solution
         }
         
      }
      }
   
   return false;
}

void displaytheSolution() {
   //cout<<"Cycle Exists: ";
   for (int i = 0; i < dimension; i++)
      cout<<path[i]<<" ";
   cout<< path[0]<<endl;
}


void printMatrix() {
   for (int row=0; row < dimension; ++row) {
      for (int col=0; col < dimension; ++col) {
         if (g[row][col])
            cout << row << " <--> " << col << endl;
      }
   }
}


bool hamCycle() {
   path = new int[dimension];
   for (int i = 0; i < dimension; i++) {
      path[i] = -1;
   }
   
   //put vertex 0 as the first vertex in the path. If there is a Hamiltonian Cycle, then the path can be 
   //started from any point
   //of the cycle as the graph is undirected
   path[0] = 0;
   if (debug && proc_rank == 0) printMatrix();

   if (hamiltonianCycle(1) == false) {
      if (proc_rank == 0){
          path[0]=1;
         
      }else {
         path[0]=1;
          MPI_Send(path, dimension, MPI_INT, 0, 0, 
            MPI_COMM_WORLD);
      }
      //return false;
   }

   if(proc_rank == 0){
      cout<<proc_rank << "/" << num_procs << ": ";
      if(path[0] == 1) cout << "Cycle does not exist\n";
      else displaytheSolution();
      
      for (int i = 1; i < num_procs; i++ ){
      MPI_Recv(path,dimension,MPI_INT,i,0,
         MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      cout<< i << "/" << num_procs << ": ";
      if(path[0] == 1) cout << "Cycle does not exist\n";
      else displaytheSolution();
      }

      }else{
         MPI_Send(path, dimension, MPI_INT, 0, 0, 
            MPI_COMM_WORLD);
      }
   
   return true;
   }
   

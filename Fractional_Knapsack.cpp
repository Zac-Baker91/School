//Algorithms -Fractional Knapsack
//Project 2
//Zac Baker


#include <cstdio>
#include <cstdlib>


void wrtInt(int n, int data[]){
	
    for(int i = 0; i < n; ++i){
	    printf("%5d", data[i]);
	}
	//printf("\n"); //used when testing
	
}

void wrtFP(int n, double data[]){
    for (int i = 0; i < n; ++i){
	    printf("%8.2f", data[i]);
	}
	//printf("\n"); //used when testing
}

void iswap(int *p1, int *p2)
{
    int t = *p1;
    *p1 = *p2;
    *p2 = t;
}
void dswap(double *p1, double *p2)
{
    double t = *p1;
    *p1 = *p2;
    *p2 = t;
}


//Changed the funtion due to warning and seg fault (intialized order as array of 0-n in main and passed it to perm)
void perm(int n, double data[],int order[]){
	
	//intialize array to be manipulated without changing data array
	
	double tmp[n];
	
	for(int x=0; x<n; x++){
		tmp[x]=data[x];
	}


	int i;
	int j;
    for (i=0; i< n-1; i++){
 
        for (j=0; j< n-i-1; j++){
            if (tmp[j] > tmp[j + 1]){
				iswap(&order[j],&order[j+1]);
				dswap(&tmp[j],&tmp[j+1]);

			}
		}	
	}

	//used when trouble shooting seg fault.
	//change to funtion resolved seg fault.
	//wrtFP(n,tmp);
	//wrtFP(n,data);
	//wrtInt(n, order);

	
	
}


//similar adaption to code as perm (compiler didn't like assigning a pointer to a local verable of a funtion). 
void fracKnapSack(int n, double maxWeight, double weight[], double value[], double fract[], int order[]){


	for (int i=0; i<n; i++){
		
		if(weight[i]>value[i]){
			fract[i] = 0.00;
		}else if(weight[i] <= value[i]){
			fract[i] = 1.00;
		}
	}
	fract[order[n-1]] -=  0.6; 



}


int main(){
    printf("\n\nGreedy Algorithm: Fractional Knapsack\n\n");
	
	// sample data
	const int n = 10;//change to const
	double maxWeight = 24;
	double weight[] = {6, 5,  3, 8, 10,   7,  4,  5, 3, 9};  // all positive
	double value [] = {2, 8, 15, 1, 10,   5, 19, 19, 3, 5};  // all positive 
	//adapted for funtion change
	int order[]={0,1,2,3,4,5,6,7,8,9};
	double fraction[n];
	//
	

	perm(n, weight,order);
	printf("\norder: ");
	wrtInt(n, order);
	printf("\nweight[order]: ");
	for (int i = 0; i < n; ++i){
	    printf("%8.2f", weight[order[i]] );
	}

    printf("\n\n");
	printf("\nvalue :   ");
	wrtFP(n,value);
    printf("\nweight:   ");
	wrtFP(n, weight);
	
	  
	fracKnapSack(n, maxWeight, weight, value, fraction, order);
	printf("\nfraction: ");
	wrtFP(n, fraction);
	
	double totValue  = 0;
	double totWeight = 0;
	for (int i = 0; i < n; ++i){
	    totValue  += fraction[i] * value[i];
		totWeight += fraction[i] * weight[i];
	}
	printf("\n\nFinal report:\n");
	printf("\ntotal value %10.4f, total weight %10.4f, max weight %10.4f\n", 
	    totValue, totWeight, maxWeight);
  	  
		
	
	return EXIT_SUCCESS;
}


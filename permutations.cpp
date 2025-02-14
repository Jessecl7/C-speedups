#include <algorithm>
#include <string>
#include <iostream>
#include <complex>
#include <iterator>
#include <vector>
#include <cmath>
#include <stdint.h>
using namespace std;

const int n = 7; //The size of K_n we're working with.
const int ncycles = 120;  //The size of the full cycles, i.e. (n-2)!.
const int nsymbols = 42;  //The size of symmetric group, which is (n-1)*n.

int numcycles(int perm[nsymbols]) { //Counts the number of cycles in the permutation
    int current = 0;
    bool checked[nsymbols] = {false};
    int num = 0;
    for (int i=0; i<nsymbols; i++) {
        if (checked[i]==false) {
            num++;
            current = i;
            do {
                checked[current] = true;
                current = perm[current];
            }
            while(checked[current]==false);
        }   
    }
    return num;
} 

void mult(int a[nsymbols],int b[nsymbols])   { //Outputs a*b as a, multiplying left to right with permutations.
    for (int t = 0; t<nsymbols ; t++) {
        if (a[t] > 41) {
            cout << a[t] << " is being used ";
        }
        a[t] = b[a[t]];
    }
}

void allcycles(int cyclecount[nsymbols])    {
    int64_t k = 0;
    int64_t kprev = 0;
    int a[n-1] = {0};
    int fullcycles[ncycles][n-1];
    int addingindex = 0;
    for (int i = 0; i<pow(n-2,n-2); i++) {
        int pows[n-2] = {0};
        for (int j = 0; j<=n-3; j++) {
            pows[j] = (int)pow(n-2,n-j-3); //Initialises all the powers, to avoid repeating this calculation.
        }
        bool used[n] = {false};
        for (int j = 0; j<=n-3; j++) {      
            kprev = k;
            k = (i / pows[j]) % (n-2);
            if (j == 0) {
                a[n-2] = k;
            }
            else {
                if (used[k] == false) {
                    a[kprev] = k;
                } 
                else {
                    break;
                }
            }
            used[k] = true;
            if (j == n-3) {
                a[k] = n-2;  //Last element of the full cycle.
                for (int l=0; l<n-1; l++) {
                    fullcycles[addingindex][l] = a[l];
                }
                addingindex++;
            }
        }
    }
    //We now have all the full cycles stored in the fullcycles list. 
    //We can now cycle through tuples of them.
    int edgeperm[nsymbols] = {0};
    for (int i=0; i<n-1; i++) {
        for (int j = i; j<n-1; j++) {
            edgeperm[(n-1)*i+j] = (j+1)*(n-1)+i;
            edgeperm[(j+1)*(n-1)+i] = (n-1)*i+j;
        }
    }
    //Generates the edge permutation we want.
    int cycleperm[nsymbols] = {0};
    int64_t bigpower = pow(ncycles,n-1);
    int64_t smallpowers[n-1] = {0};
    for (int j=0; j<n-1; j++) { //Does the powers first to avoid recalculating
        smallpowers[j] = (int64_t)pow(ncycles,n-2-j);
    }
    for (int64_t i=0 ; i<bigpower ; i=i+10000) { //Sets the other n-1 cycles
        for (int m=0 ; m<n-2; m++) {
            cycleperm[m] = m+1;
        }
        cycleperm[n-2] = 0;
        for (int j=0; j<n-1; j++) {
            k = (i/smallpowers[j]) % ncycles;
            for (int l=0; l<n-1; l++) {
                cycleperm[(n-1)*(j+1) + l] = fullcycles[k][l]+(n-1)*(j+1);
                if (fullcycles[k][l]+(n-1)*(j+1) >41) {
                    cout << k << "\n" << i << "\n" << smallpowers[j];
                }
            }
        }
        //Multiplies this with the edge permutation
        mult(cycleperm, edgeperm);
        cyclecount[numcycles(cycleperm)]++;
    }
}

int main()  {
    int cyclecount[nsymbols] = {0};
    allcycles(cyclecount);
    for (int i=0; i<nsymbols; i++) {
        cout << cyclecount[i] << "\n";
    } 
    return 0;
}
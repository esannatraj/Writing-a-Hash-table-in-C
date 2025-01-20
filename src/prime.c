#include <math.h>

#include "prime.h"

/*
* Return whether x is prime or not
* 
* Returns:
* 1 - Prime
* 0 - Not prime
* -1 - undefined (x < 2)
*/

int is_prime(const int x) {
    if (x < 2) {return -1;}
    if (x < 4) {return 1;}
    if (x % 2 == 0) {return 0;}
    for (int i = 3; i <= floor(sqrt(x)); i+=2) {
        if (x % i == 0) {
            return 0;
        }
    } 

    return 1;
}

/*
* Return next prime after x or x if it is a prime
*/
int next_prime(int x) {
    while(is_prime(x) != 1) {
        x++;
    }
    return x;
}
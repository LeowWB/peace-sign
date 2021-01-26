/*************************************
* Lab 1 Exercise 3
* Name     : Daryl Tan
* Matric No: A0132812R
* Lab Group: B17
*************************************/
/*************************************
* Lab 1 Exercise 3
* Name     : Leow Wen Bin
* Matric No: A0184415E
* Lab Group: B13
*************************************/

#include <stdio.h>

//Datatype Declarations
typedef int (*arithFuncPtr)(int, int);


//Function Prototypes
int sumBetween(int x, int y);
int power(int x, int y);
int expel(int x, int y);
int gcd(int x, int y);


int main() {
    arithFuncPtr spells[] = {sumBetween, gcd, expel, power};
    char *spellNames[] = {"lumos", "alohomora", "expelliarmus", "sonorus"};

    int spell, x, y;
    while (scanf("%i%i%i", &spell, &x, &y) == 3) {
        spell = spell - 1; // change to one based indexing
        printf("%s %i\n", spellNames[spell], (*spells[spell])(x, y));
    }

    return 0;
}

int sumBetween(int x, int y) {
    return (y - x + 1) * (y + x) / 2;
}

int gcd(int a, int b) {
    int r, i;
    while (b != 0) {
        r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int expel(int x, int y) {
    while (x % y == 0) {
        x /= y;
    }
    return x;
}

int power(int x, int y) {
    int result = 1;
    for (int i = 0; i < y; i++) {
        result *= x;
    }
    return result;
}
#ifndef INDEXOFC
#define INDEXOFC
// returns the index of coincidence given cipherLength and character counts
double actualIC(double cipherLength, int *charCount);
// Returns the index of coincidence given cipher/key length.  
double expectedIC(double cipherLength, double keyLength);
#endif

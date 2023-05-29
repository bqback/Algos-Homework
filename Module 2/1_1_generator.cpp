#include <iostream>
#include <fstream>
 
// The main recursive method
// to print all possible
// strings of length k
void printAllKLengthRec(std::ostream& os, char set[], std::string prefix,
                                    int n, int k)
{
     
    // Base case: k is 0,
    // print prefix
    if (k == 0)
    {
        os << (prefix) << std::endl;
        return;
    }
 
    // One by one add all characters
    // from set and recursively
    // call for k equals to k-1
    for (int i = 0; i < n; i++)
    {
        std::string newPrefix;
         
        // Next character of input added
        newPrefix = prefix + set[i];
         
        // k is decreased, because
        // we have added a new character
        printAllKLengthRec(os, set, newPrefix, n, k - 1);
    }
 
}
 
void printAllKLength(std::ostream& os, char set[], int k, int n)
{
    printAllKLengthRec(os, set, "+ ", n, k);
}
 
// Driver Code
int main()
{
    std::ofstream file;
    file.open("data.txt");

    char set[] = {'a', 'b', 'c', 'd', 'e'};
    int k = 5;
    printAllKLength(file, set, k, 5);
    
    file.close();

    return 0;
}
#include "main.c" // Include the specific implementation of BST and Splay Tree 
#include <stdio.h>  // Standard input/output
#include <stdlib.h> // Memory allocation and random numbers
#include <time.h>   // Timing function clock()
double MeasureBST(const int *in, const int *del, int N, int times) // Measure the average time cost of insertion and deletion for Binary Search Tree (BST)
{
    clock_t start = clock(); // Record start time
    int k; // Loop counter
    for (k = 0; k < times; k++) // Repeat tests multiple times to take the average and reduce errors
    {
        TreeNode *Root = NULL; // Initialize an empty tree
        int i; // Inner loop counter
        for (i = 0; i < N; i++) // Loop to insert N elements
        {
            Root = InsertNode(Root, in[i]);
        }
        for (i = 0; i < N; i++) // Loop to delete N elements
        {
            Root = DeleteNode(Root, del[i]);
        }
        FreeTree(Root); // Free the memory of the entire tree to prevent memory leaks
    }
    // Calculate total time cost and divide by repeat times, return single average time (unit: seconds)
    return (double)(clock() - start) / CLOCKS_PER_SEC / times;
}
double MeasureSplay(const int *in, const int *del, int N, int times) // Measure the average time cost of insertion and deletion for Splay Tree
{
    clock_t start = clock(); // Record start time
    int k; // Loop counter
    for (k = 0; k < times; k++) // Repeat tests multiple times to take the average
    {
        SplayTree *Root = NULL; // Initialize an empty tree
        int i; // Inner loop counter
        for (i = 0; i < N; i++) // Loop to insert N elements
        {
            Root = Splay_Insert(Root, in[i]);
        }
        for (i = 0; i < N; i++) // Loop to delete N elements
        {
            Root = Splay_Delete(Root, del[i]);
        }
        Splay_Free(Root); // Free the memory of the entire tree
    }
    return (double)(clock() - start) / CLOCKS_PER_SEC / times;     // Calculate total time cost and divide by repeat times, return single average time (unit: seconds)
}
void Shuffle(int *a, int N) // Classic Fisher-Yates shuffle algorithm, used to shuffle arrays to generate random test data
{
    int i, j, tmp; // Loop variables and temporary swap variable
    for (i = N - 1; i > 0; i--) // Traverse from back to front
    {
        j = rand() % (i + 1); // Randomly generate an index between 0 and i
        tmp = a[i];           // Start swapping a[i] and a[j]
        a[i] = a[j];
        a[j] = tmp;
    }
}
void RunCase(const char *name, const int *in, const int *del, int N, int bst_times, int splay_times) // Run a test case and print the time cost comparison between BST and Splay
{
    double t1, t2; // Store the time cost of the two trees respectively
    t1 = MeasureBST(in, del, N, bst_times); // Test BST performance
    t2 = MeasureSplay(in, del, N, splay_times); // Test Splay performance
    printf("%s\n", name); // Print test case name
    printf("  BST:   %.9f s  (avg of %4d)\n", t1, bst_times); // Print BST time cost and average times
    printf("  Splay: %.9f s  (avg of %4d)\n", t2, splay_times); // Print Splay time cost and average times
}
int main() // Main function, program entry point
{
    int N, i, *inc, *rev, *rnd; // N is data size, inc/rev/rnd are increasing/decreasing/random arrays
    int fast_times; // Used to control the number of test loops
    do // Loop to read user input until a valid N value is entered
    {
        printf("Enter N (1000-20000): "); // Prompt the user to enter a size between 1000 and 20000
        if (scanf("%d", &N) != 1) // Read an integer, if reading fails
        {
            return 1; // Exit the program abnormally
        }
    } while (N < 1000 || N > 20000); // Restrict the range of N
    inc = (int*)malloc((size_t)N * sizeof(int));    // Dynamically allocate memory space for the three arrays, size is N integers
    rev = (int*)malloc((size_t)N * sizeof(int));
    rnd = (int*)malloc((size_t)N * sizeof(int));
    if (!inc || !rev || !rnd) // Check if memory allocation is successful (prevent malloc failure)
    {
        return 1; // Exit if allocation fails
    }
    for (i = 0; i < N; i++) // Initialize the three arrays
    {
        inc[i] = i; // Increasing array: 0, 1, 2, ...
        rev[i] = N - 1 - i; // Decreasing array: N-1, N-2, ...
        rnd[i] = i; // Generate an increasing array first, then shuffle it below
    }
    srand(114514); // Set random number seed to ensure results are reproducible every run (or a prank seed)
    Shuffle(rnd, N); // Shuffle the rnd array into a truly random array
    // Dynamically adjust the number of tests based on data size N. The larger the data size, the fewer loops to prevent running too long
    fast_times = (N <= 2000) ? 3000 : (N <= 5000) ? 1000 : 300; // don't ask,ask is too many tries))))
    // Run test case 1: increasing insertion, increasing deletion (triggers BST worst case degenerating into a linked list, Splay adapts)
    RunCase("Case 1: increasing insertion, increasing deletion", inc, inc, N, 1, fast_times);
    // Run test case 2: increasing insertion, reverse deletion (also an extreme case)
    RunCase("Case 2: increasing insertion, reverse deletion", inc, rev, N, 1, fast_times);
    // Run test case 3: random insertion, random deletion (reflects average performance)
    RunCase("Case 3: random insertion, random deletion", rnd, rnd, N, 50, 50);
    // Free the previously allocated dynamic memory to prevent memory leaks
    free(inc);
    free(rev);
    free(rnd);
    return 0; // Program ends normally
}
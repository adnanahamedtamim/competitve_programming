#include <iostream>
using namespace std;

void towerOfHanoi(int n, char source, char helper, char destination)
{
    // Base case
    if (n == 1)
    {
        cout << "Move disk 1 from "
             << source << " to "
             << destination << endl;
        return;
    }

    // Step 1:
    // Move n-1 disks from source to helper
    towerOfHanoi(n - 1, source, destination, helper);

    // Step 2:
    // Move largest disk to destination
    cout << "Move disk " << n
         << " from " << source
         << " to " << destination << endl;

    // Step 3:
    // Move n-1 disks from helper to destination
    towerOfHanoi(n - 1, helper, source, destination);
}

int main()
{
    int n = 3;

    towerOfHanoi(n, 'A', 'B', 'C');

    return 0;
}

//2022.03.13
//Every day Code
//interview 17.14
//Accomplishment：AC in one time
//Notes：some O(n) not got ,and the sample of return 

//AC code
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

#define MIN(a, b) a < b ? a : b
#define MAXN 100100

int cmp(const void * a, const void * b){
    int * _a = (int *)a;
    int * _b = (int *)b;
    return *_a - *_b;
}

int* smallestK(int* arr, int arrSize, int k, int* returnSize){
    int * ans = (int *)malloc(sizeof(int) * k);
    qsort(arr, arrSize, sizeof(int), cmp);
    int i;
    for(i = 0; i < k; i++){
        
        ans[i] = arr[i];
    }
    for(i = 0; i < k; i++){
        printf("%d", ans[i]);
    }
    * returnSize = k;
    return ans;
}

//some other O(n) way
class Solution {
public:
    vector<int> smallestK(vector<int>& arr, int k) {
    vector<int>ans(k);
    if(k == 0)
        return ans;
    nth_element(arr.begin(),arr.begin() + k - 1,arr.end());
    int count= 0;
    for(int i = 0; i< arr.size(); i++){
        if(arr[i]<arr[k-1])
            ans[count++] = arr[i];
    }
    for(int i = count; i < k; i++)
        ans[i] = arr[k-1];
    return ans;
    }
};

//C-BFPRT code
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <algorithm>
 
using namespace std;
const int N = 10005;
 
int a[N];
 
void InsertSort(int a[], int l, int r)
{
    for(int i = l + 1; i <= r; i++)
    {
        if(a[i - 1] > a[i])
        {
            int t = a[i];
            int j = i;
            while(j > l && a[j - 1] > t)
            {
                a[j] = a[j - 1];
                j--;
            }
            a[j] = t;
        }
    }
}
 
int FindMid(int a[], int l, int r)
{
    if(l == r) return l;
    int i = 0;
    int n = 0;
    for(i = l; i < r - 5; i += 5)
    {
        InsertSort(a, i, i + 4);
        n = i - l;
        swap(a[l + n / 5], a[i + 2]);
    }
 
    //deal with the left element
    int num = r - i + 1;
    if(num > 0)
    {
        InsertSort(a, i, i + num - 1);
        n = i - l;
        swap(a[l + n / 5], a[i + num / 2]);
    }
    n /= 5;
    if(n == l) return l;
    return FindMid(a, l, l + n);
}
 
int Partion(int a[], int l, int r, int p)
{
    swap(a[p], a[l]);
    int i = l;
    int j = r;
    int pivot = a[l];
    while(i < j)
    {
        while(a[j] >= pivot && i < j)
            j--;
        a[i] = a[j];
        while(a[i] <= pivot && i < j)
            i++;
        a[j] = a[i];
    }
    a[i] = pivot;
    return i;
}
 
int BFPRT(int a[], int l, int r, int k)
{
    int p = FindMid(a, l, r);    //find mid's mid
    int i = Partion(a, l, r, p);
 
    int m = i - l + 1;
    if(m == k) return a[i];
    if(m > k)  return BFPRT(a, l, i - 1, k);
    return BFPRT(a, i + 1, r, k - m);
}
 
int main()
{
    int n, k;
    scanf("%d", &n);
    for(int i = 0; i < n; i++)
        scanf("%d", &a[i]);
    scanf("%d", &k);
    printf("The %d th number is : %d\n", k, BFPRT(a, 0, n - 1, k));
    for(int i = 0; i < n; i++)
        printf("%d ", a[i]);
    puts("");
    return 0;
}
 
/**
10
72 6 57 88 60 42 83 73 48 85
5
*/

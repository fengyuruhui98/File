#include <stdio.h>

int trap(int* height, int heightSize){
    int ans = 0, i, j, k = 0, temp = height[0], temp1 = 0;
    int left = 0, right = 0;
    for(i = 0; i < heightSize; i++){
        if(temp > height[i]){
            right++;
        }
        else if(temp <= height[i]){
            for(j = left; j <= right; j++){
                temp1 = temp - height[j];
                ans += temp1;
            }
            temp = height[i];
            left = right;
        }
    }
    return ans;
}
int main(){
    int a[1] = {0};
    int b = trap(a, 1);
    printf("\nzdd:%d\n", b);
}

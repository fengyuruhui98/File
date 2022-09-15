//2022.03.06
//Every day Code
//Leetcode165
//Accomplishment: Copy!!! little man 
//Notes:dichotomy

//copy C++
class Solution{
public:
    int compareVersion(string version1, string version2) {
        int i = 0, j = 0;
        while(i < version1.size() || j < version2.size()){
            int temp1 = 0, int temp2 = 0;
            while(i < version1.size() && version1[i] != '.') temp1 = temp1 * 10 + version1[i] - '0';
            while(j < version2.size() && version2[j] != '.') temp2 = temp2 * 10 + version2[j] - '0'; // so fucking classic
            if(temp1 > temp2) return 1;
            else if(temp1 < temp2) return -1;
            i++, j++;            //so motherfucker classic
        }
        return 0;
    } 
};



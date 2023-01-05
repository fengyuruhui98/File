//2022.03.06
//Every day Code
//Leetcode1818
//Accomplishment: big shit
//Notes:dichotomy

//C-my own so fucking big shit!!!!
#define MAX(a, b) (a > b) ? a : b
#define MIN(a, b) (a > b) ? b : a
int cmp(const void * a, const void * b){
    int * _a = (int *)a;
    int * _b = (int *)b;
    return *_a - *_b;
}

int findcloseest(int a, int* nums, int left, int right){
    int mid = left + (right - left)/2 ;   //shit, bitches use the /2, good shit use >>1
    if((left + 1) == right){            //so many motherfucker branches, that you can not tell whether it is all in or not
        return MIN(nums[right] - a, a - nums[left]);
    }
    else if(nums[left] == a || nums[right] == a || nums[mid] == a) return 0;
    else if(nums[mid] > a) return findcloseest(a, nums, left, mid);
    else return findcloseest(a, nums, mid, right);
}

int minAbsoluteSumDiff(int* nums1, int nums1Size, int* nums2, int nums2Size){
    int i, place = -1, ans = 0, max = -1, min = 0x3f3f3f3f,  temp = 0, mod = 1000000007;
    int abs[nums2Size];
    for(i = 0; i < nums1Size; i++){
        temp = (nums1[i]) > (nums2[i]) ? (nums1[i] - nums2[i]) : (nums2[i] - nums1[i]);
        abs[i] = temp;
        ans += temp;
    }
    qsort(nums1, nums1Size, sizeof(int), cmp);
    printf("%d", ans);
    for(i = 0; i < nums2Size; i++){
        max = MAX(max, (abs[i] - findcloseest(nums2[i], nums1, 0, nums1Size - 1)));
    }
    ans -= max;
    return ans % mod;
}



//C-god damn right answer, stupid dichotomy way to answer, I just always ignore many little piece of shits, something so fucking little.
int lower_bound(int* a, int n, int x) {
    int l = 0, r = n;
    while (l < r) {
        int mid = (l + r) >> 1;
        if (a[mid] < x) {
            l = mid + 1;
        } else {
            r = mid;
        }
    }
    return l;
}

int cmp(int* a, int* b) {
    return *a - *b;
}

const int mod = 1000000007;

int minAbsoluteSumDiff(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    int n = nums1Size;
    int rec[n];
    memcpy(rec, nums1, sizeof(int) * n);
    qsort(rec, n, sizeof(int), cmp);
    int sum = 0, maxn = 0;
    for (int i = 0; i < n; i++) {
        int diff = abs(nums1[i] - nums2[i]);
        sum = (sum + diff) % mod;
        int j = lower_bound(rec, n, nums2[i]);
        if (j < n) {
            maxn = fmax(maxn, diff - (rec[j] - nums2[i]));    //why this son of bitch make this no use thing, it can not be understand, emmmmm
        // it's offical stories says that we need first find the closeest element in sorted tem[], while this element may bigger than nums2[i],
        //and may smaller than nums2[i], so we need to check twice.
        }
        if (j > 0) {
            maxn = fmax(maxn, diff - (nums2[i] - rec[j - 1]));
        }
    }
    return (sum - maxn + mod) % mod;
}

//C-myself shit plus code 
#define MAX(a, b) (a > b) ? a : b
#define MIN(a, b) (a > b) ? b : a
int cmp(const void * a, const void * b){
    int * _a = (int *)a;
    int * _b = (int *)b;
    return *_a - *_b;
}

int findcloseest(int a, int* nums, int left, int right){
    while(left < right){
        int mid = (left + right) >>1;
        if(nums[mid] < a){
            left = mid + 1;
        }
        else right = mid ;
    }
    return a > nums[left] ? (a - nums[left]) : (nums[left] - a); 
}

int minAbsoluteSumDiff(int* nums1, int nums1Size, int* nums2, int nums2Size){
    int i, ans = 0, max = -1,  temp = 0, mod = 1000000007;
    int abs[nums2Size];
    for(i = 0; i < nums1Size; i++){
        temp = (nums1[i]) > (nums2[i]) ? (nums1[i] - nums2[i]) : (nums2[i] - nums1[i]);
        abs[i] = temp;
        ans += temp;
    }
    qsort(nums1, nums1Size, sizeof(int), cmp);
    //printf("%d", ans);
    for(i = 0; i < nums2Size; i++){
        temp = findcloseest(nums2[i],nums1,0,nums1Size-1);
        temp = abs[i] > temp ? abs[i] - temp : temp - abs[i];
        max = MAX(max, temp);
    }
    printf("%d", max);
    ans -= max;
    return (ans + mod) % mod;
}

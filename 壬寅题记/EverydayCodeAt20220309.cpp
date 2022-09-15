//2022.03.03
//Every day Code
//Leetcode1750
//Accomplishment：No AC to day~~~~
//Notes：A man named Zhengdong never give up!!!

//ben bi
#define MAXN 10000

int weight(const int b){
    int weight = 0, a = b;
    while(a >= 10){
        a /= 10;
        weight++;
    }
    return weight;
}

int sqart(const int a){
    int  b = a, ans = 1;
    while(b > 0){
        ans *= 10;
        b--;
    }
    return ans;
}

char * getcyclestr(int numerator, int denominator, char * ans, int * shang){
    int weight1 = weight(numerator), weight2 = weight(denominator);
    int shangtemp[2] = {0};
    int high1 = numerator / sqart(weight1), high2 = denominator / sqart(weight2);
    if(high1 > high2){
        int i = high2 - high1, j;
        for(j = 0; j < i - 1; j++){
            ans[j] = '0';
        }
        shangtemp[0] = numerator * sqart(i) / denominator;
        shangtemp[1] = numerator * sqart(i) % denominator;
        if(shang[0] == shangtemp[0] && shang[1] == shangtemp[1]){
            return ans;
        }
        else {
            ans[j] = shangtemp[0] - '0';
            ans[j + 1] = getcyclestr(shangtemp[1], denominator, ans[j + 1], shang);
        }
    }
    else if(high1 = high2){
        
    }
}

char * fractionToDecimal(int numerator, int denominator){
    char *oper = (char *)malloc(sizeof(char) * MAXN);
    char *oper1 = (char *)malloc(sizeof(char) * MAXN);
    int bigger = 0, lower = 0, dot = 0, i = 0, j = 0;
    int shang[2] = {0};
    if(numerator > denominator) {
        bigger = numerator / denominator;
        lower = numerator - bigger * denominator;
        int weightbigger = weight(bigger);
        for(j = weightbigger; j >= 0; j--){
            int temp = bigger / (sqart(j));
            bigger -= temp * (sqart(j));
            oper[i++] = temp + '0';
        }
        oper[i] = '.';
        dot = i;
    }else{
        oper[0] = 0 + '0';
        oper[1] = '.';
        lower = numerator;
        dot = 1;
    }
    int dote = dot;    //dot is dot'splace
    ret = getcyclestr(lower, denominator, oper1, shang);
    oper[dot + 1] = '\0';
    return oper; 
}

//finally small zheng decide to leave it alone
#define MAXN 10000

int weight(const int b){
    int weight = 0, a = b;
    while(a >= 10){
        a /= 10;
        weight++;
    }
    return weight;
}

int sqart(const int a){
    int  b = a, ans = 1;
    while(b > 0){
        ans *= 10;
        b--;
    }
    return ans;
}

char * getcyclestr(int numerator, int denominator, char * ans, int * shang){
    int weight1 = weight(numerator), weight2 = weight(denominator), dot = 0, i;
    int shangtemp[2] = {0};
    int temp = weight1 - weight2;
    for(i = 0; i < temp - 1; i ++){
        ans[i] = '0';
        dot++;
    }
    numerator *= sqart(temp);
    shangtemp[0] = numerator / denominator;
    shangtemp[1] = numerator % denominator;
    if(shangtemp[0] == shang[0] && shangtemp[1] == shang[1]){
        return ans;
    }
    else return getcyclestr(shangtemp[1], denominator, &ans[dot + 1], shang);
}

char * fractionToDecimal(int numerator, int denominator){
    char *oper = (char *)malloc(sizeof(char) * MAXN);
    char *oper1 = (char *)malloc(sizeof(char) * MAXN);
    int bigger = 0, lower = 0, dot = 0, i = 0, j = 0;
    int shang[2] = {0};
    if(numerator > denominator) {
        bigger = numerator / denominator;
        lower = numerator - bigger * denominator;
        int weightbigger = weight(bigger);
        for(j = weightbigger; j >= 0; j--){
            int temp = bigger / (sqart(j));
            bigger -= temp * (sqart(j));
            oper[i++] = temp + '0';
        }
        oper[i] = '.';
        dot = i;
    }else{
        oper[0] = 0 + '0';
        oper[1] = '.';
        lower = numerator;
        dot = 1;
    }
    int dote = dot;    //dot is dot'splace

    oper1 = getcyclestr(lower, denominator, oper1, shang);
    i = 0;
    while(i < 10) printf("%d", oper1[i++]);
    oper[dot + 1] = '\0';
    return oper; 
}

//clear-C++
class Solution{
public:
    typedef long long ll;
    string fractionToDecimal(int numerator, int denominator) {
        int num = numerator, den = denominator;
        string ans = "";
        ll bigger = abs(num / den);
        ll temp = (num * den);
        if(temp == 0) return "0";
        if(temp < 0) ans += '-';
        ans += to_string(bigger);
        ll lower = num % den;
        if(lower == 0) return ans;
        ans += ".";
        unordered_map<ll, int> hash;
        ll index = 0;
        string s;
        printf("%0.4lf", lower);
        while(lower && !hash.count(lower)){
            hash[lower] = index++;
            lower *= 10;
            s.push_back(lower/den + '0');
            lower %= den;
        }
        if(lower != 0){
            ans += s.substr(0, hash[lower]) + "(" + s.substr(hash[lower]) + ")";
        }
        else{
            for(int i = 0; i < s.size(); i++) printf("%c", s[i]);
            ans += s;
        }
        return ans;
    }
};

class Solution {
public: 
    typedef long long ll;
    string fractionToDecimal(int numerator, int denominator) {
         
        ll num=numerator,den=denominator;
        string ans="";
        ll re=abs(num/den);
        if(num * den<0) ans+="-";
        num=abs(num),den=abs(den);   //fucking importent
        ll n=num%den;
        ans+=to_string(re);
        if(n==0)  return ans;//if can be divided complete
        ans+=".";///plus "."
        unordered_map<ll,int> mp;
        ll idx=0;//can not use int will overflow
        string  s;
        while(n&&!mp.count(n)){
            mp[n]=idx++;//as the index of the cycle number
            n*=10;
            s.push_back(n/den+'0');
            n%=den;

        }
        if(n!=0){//if there has cycles plus "("
            ans+=s.substr(0,mp[n])+"("+s.substr(mp[n])+")";
        }
        else{
            ans+=s;
        }
        return ans;

    }
};
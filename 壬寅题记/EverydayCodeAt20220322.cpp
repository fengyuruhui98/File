//2022.03.22
//Every day Code
//Leetcode2038
//Accomplishment：So fucking stupid code
//Notes：C-so easy count

//stupid C out of time limit
int findo(char * colors, char flag){
    int i = 1;
    char colorstem[strlen(colors) + 1];
    memcpy(colorstem, colors, strlen(colors));
    colorstem[strlen(colors)] = '\0';
    if(flag == 'A'){
        for(i = 1; i < strlen(colors) - 1; i++){
            if(colorstem[i] == 'A' && colors[i - 1] == colors[i] && colors[i] == colors[i + 1]){
                memcpy(&colors[i], &colorstem[i + 1], strlen(colorstem) - i - 1);
                colors[strlen(colors) - 1] = '\0';
                
                return 1;
            }
        }
        return 2;
    }
    else {
        for(i = 1; i < strlen(colors) - 1; i++){
            if(colors[i] == 'B' && colors[i - 1] == colors[i] && colors[i] == colors[i + 1]){
                memcpy(&colors[i], &colorstem[i + 1], strlen(colorstem) - i - 1);
                colors[strlen(colors) - 1] = '\0';
                return 3;
            }
        }
        return 4;
    }
}

bool winnerOfGame(char * colors){
    char tem = 'A';
    int ans = 3;
    int n = strlen(colors);
    while(n--){
        if(ans == 1){
            tem = 'B';
        }
        else if(ans == 2){
            break;
        }
        else if(ans == 3){
            tem = 'A';
        }
        else if(ans == 4) break;
        ans = findo(colors, tem);
    }
    if(ans == 4) return true;
    else return false;
}

//C
bool winnerOfGame(char * colors){
    int freq[2] = {0, 0};
    char cur = 'C';
    int cnt = 0;
    int len = strlen(colors);
    for(int i = 0; i < len; i++){
        char c = colors[i];
        if(c != cur){
            cur = c;
            cnt = 1
        }else {
            cnt += 1;
            if(cnt >= 3){
                freq[cur - 'A'] += 1;
            }
        }
    }
    return freq[0] > freq[1];
}

//C++
class Solution{
public:
    bool winnerOfGame(string colors){
        int freq[2] = {0, 0};
        char cur = 'C';
        int cnt = 0;
        for(char c : colors){
            if(c != cur){
                cur = c;
                cnt = 1;
            }else if(cnt >= 3){
                ++freq[cur - 'A'];
            }
        }
        return freq[0] > freq[1];
    }
};

//python
class Solution:
    def winnerOfGame(self, colors:str) -> bool:
        freq = [0, 0]
        cur, cnt = 'C', 0
        for c in colors:
            if c != cur:
                cur = c
                cnt = 1
            else:
                cnt += 1
                if cnt >= 3:
                    freq[ord(cur) - ord(A)] += 1
        return freq[0] > freq[1]


//so easy code in python
class Solution:
public:        //Just go straight find the deleteable A and deleteable B
    def winnerOfGame(self, colors:str) -> bool:
        a, b = 0, 0
        for i in range(1, len(colors) - 1):
            if colors[i] == colors[i - 1] == colors[i + 1] == 'A':
                a += 1
            elif colors[i] == colors[i - 1] == colors[i + 1] == 'B':
                b += 1
        return a > b
//2022.06.21
//Every day Code
//Leetcode1108
//Accomplishment：AC in a lot time
//Notes：base C

char * defangIPaddr(char * address){     //so fucking awsome answer.
	int len = strlen(address);
	int pos = 0;
	char * res = (char *)malloc(sizeof(char) * (len + 7));
	for(int i = 0; i < len; i++){
		if(address[i] == '.'){
			pos += sprintf(res + pos, "%s", "[.]");
		}else{
			res[pos++] = address[i];
		}
	}
	res[pos] = '\0';
	return res;
}

char * defangIPaddr(char * address){           //uglly own code
	int len = strlen(address);
	char * add = (char *)malloc(2 * len + 1);
	add[2 * len] = '\0';
	memcpy(add, address, len);
	for(int i = 0; i < len; i++){
		if(add[i] == '.'){
			for(j = len - 1; j > i; j--) add[j + 2] = add[j];
			len += 2;
			add[i + 1] = add[i];
			add[i] = '[';
			add[i + 2] = ']';
			i += 3;
		}
	}
	char * ans = (char *)malloc(sizeof(char) * len + 1);
	ans[len] = '\0';
	memcpy(ans, add, len);
	return ans;
}

//C++ standard answer
class Solution{
public:
		string defangIPaddr(string address){
			string ans;
			for(auto & c : address){
				if(c == '.') ans.append("[.]");
				else ans.pushback(c); 
			}
			return ans;
		}
};

//Python standard answer
class Solution:
	def defangIPaddr(self, address:str) -> str:
		return address.replace('.', '[.]')
		

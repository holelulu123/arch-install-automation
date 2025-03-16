#include <iterator>
#include <map>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

bool isAnagram(string first, string second){
    map<char, int> check_map_first;
    map<char, int> check_map_second;
    if (first.size() != second.size()){
        return false;
    }
    for (int i = 0; i < first.size(); i++){ 
        check_map_first[first[i]]++;
        check_map_second[second[i]]++;
    }
    for (const auto& pair : check_map_first){
        if (check_map_second.find(pair.first) != check_map_second.end()){
            if (check_map_second[pair.first] != pair.second){
                return false;
            }
        }
        
        else{
            return false;
        }
    }   
    return true;
}

int return_max_in_vector(vector<int> nums){
    
    int temp_max = 0;
    for (int i = 0; i < nums.size(); i++){
        if (nums[i] > temp_max){
            temp_max = nums[i];
        }
    }
    return temp_max;
}

vector<string> decode(string s){
    vector<string> output;
    string delimiter = "____";
    string temp_str = "";
    string rest_str;
    rest_str = s; 
    int to_index = rest_str.find(delimiter);
    while(to_index != -1){
        temp_str = rest_str.substr(0, to_index);
        rest_str = rest_str.substr(to_index+4, s.size());
        output.push_back(temp_str);
        to_index = rest_str.find(delimiter);
        cout << "string is: " << temp_str << endl;
    }
    return output;
    
}

string encode(vector<string>& strs){
    // vector<int> sizes_vector;
    string str = "";
    for (int i = 0; i < strs.size(); i++){
        str += strs[i];
        str += "____";
        // sizes_vector.push_back(strs[i].size());

    }
    return str;
}

vector<int> productExceptSelf(vector<int>& nums) {
    int n = nums.size();
    vector<int> res(n);
    vector<int> pref(n);
    vector<int> suff(n);

    pref[0] = 1;
    suff[n - 1] = 1;
    for (int i = 1; i < n; i++) {
        pref[i] = nums[i - 1] * pref[i - 1];
    }
    for (int i = n - 2; i >= 0; i--) {
        suff[i] = nums[i + 1] * suff[i + 1];
    }
    for (int i = 0; i < n; i++) {
        res[i] = pref[i] * suff[i];
    }
    return res;
}
    


int main(){
    vector<int> input_vector = {-1, 5, 1, 2, 3};
    vector<int> output_vector;
    output_vector = productExceptSelf(input_vector);
    for(int i = 0; i < output_vector.size(); i++){
        cout << output_vector[i] << endl;
    }


    return 0;
}   
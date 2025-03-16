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
    string delimiter = "_";
    string temp_str = "";
    string rest_str;
    rest_str = s; 
    int to_index = rest_str.find(delimiter);
    while(to_index != -1){
        temp_str = rest_str.substr(0, to_index);
        rest_str = s.substr(to_index+1, s.size());
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
        str += "_";
        // sizes_vector.push_back(strs[i].size());

    }
    return str;
}


int main(){
    vector<string> str_vector = {"hello", "world", "iam", "yuval"};
    string str_word;
    str_word = encode(str_vector);
    cout << "Fully encoded string vector is: " << str_word << endl;
    vector<string> str_output = decode(str_word);



    return 0;
}   
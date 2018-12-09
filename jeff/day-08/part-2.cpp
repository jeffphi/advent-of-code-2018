#include <iostream>
#include <vector>
#include <string>
#include "jefflib.h" 
#include <boost/tokenizer.hpp>
#include <map>
#include <set>

using namespace std;
using namespace boost;

struct node_t{
    map<int, node_t*> children;
    vector<int> metadata;
    int value = 0;
};

void buildTree(node_t* node, vector<int>& data, int& index);

int main()
{
    vector<string> vect;
    if(GetStringInput(vect)){
        cout << "Got data!" << endl;
        cout << endl;
    }
    else {
        cout << "Failed to read input :( " << endl;
        return -1;
    }

    char_separator<char> sep(" ");
    tokenizer< char_separator<char> > tokens(vect[0], sep);
    vector<string> v_temp(tokens.begin(), tokens.end());

    vector<int> data;
    for(auto s : v_temp){
        int val = stoi(s);
        data.push_back(val);
    }

    int index = 0;

    node_t* root = new node_t();
    index = 0;
    buildTree(root, data, index);
    cout << "Value: " << root->value << endl;
}

void buildTree(node_t* node, vector<int>& data, int& index){
    int numChildren = data[index];
    int numMeta = data[++index];
    
    for(int i = 0; i < numChildren; i++){
        node_t* tempNode = new node_t{};
        node->children[i+1] = tempNode;
        buildTree(tempNode, data, ++index);
    }

    for(int i = 0; i < numMeta; i++){
        node->metadata.push_back(data[++index]);
    }

    if(numChildren == 0){
        for(int val : node->metadata){
            node->value += val;
        }
    }
    else{
        for(int md : node->metadata){
            if(node->children.find(md) != node->children.end()){
               node->value += node->children[md]->value;
            }
        }
    }
}

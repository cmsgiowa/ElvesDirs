#include <iostream>
#include <vector>
#include <iterator>
#include <list>
#include <numeric>
#include <fstream>
#include <functional>
#include <stack>
#include <queue>
#include <string.h>

using namespace std;

std::vector<std::string> split(const std::string& s, const std::string& delimiter, const bool removeEmptyEntries = false)
{
    std::vector<std::string> tokens;

    for (size_t start = 0, end; start < s.length(); start = end + delimiter.length())
    {
         size_t position = s.find(delimiter, start);
         end = position != std::string::npos ? position : s.length();

         std::string token = s.substr(start, end - start);
         if (!removeEmptyEntries || !token.empty())
         {
             tokens.push_back(token);
         }
    }

    if (!removeEmptyEntries &&
        (s.empty()))
    {
        tokens.push_back("");
    }

    return tokens;
}

struct Node
{
    string key;
    string parent;
    int nodelevel;
    vector<Node *>child;
};
  
Node *newNode(string key, string _parent, int level)
{
    Node *temp = new Node;
    temp->key = key;
    temp->parent = _parent;
    temp->nodelevel = level;
    return temp;
}

Node* search(Node* nodeSearching, Node *t){
  if(t == NULL)
  {
    return t;
  }
  if(t->key.compare(nodeSearching->key) == 0 && t->nodelevel == nodeSearching->nodelevel && t->parent.compare(nodeSearching->parent) == 0)
  {
    return t;
  }

  Node* result = NULL;
  if (t->child.size() != 0) {
    for (int i = 0; i < t->child.size(); i++){
        result = search(nodeSearching, t->child[i]);
        if (result->key.compare(nodeSearching->key) == 0 && result->nodelevel == nodeSearching->nodelevel && result->parent.compare(nodeSearching->parent) == 0) {
            return result;
        }
    }
  }
}

long int PrintAll(Node* root, long int& totalVol){
    if (root->child.size() == 0 && isdigit(root->key[0])) {
        return stol(root->key);
    } else if ((root->child.size() == 0) && !isdigit(root->key[0])) {
        return 0;
    }
       
    long int dirsize = 0;
    for (int i = 0; i < root->child.size(); i++){
        dirsize += PrintAll(root->child[i], totalVol);
    }

    //if (dirsize <= 100000) {
    if (root->nodelevel == 0) {
        totalVol = totalVol + dirsize;
        cout << root->key << "  volume:" << dirsize << endl;
    }
    return dirsize;
}

int main()
{
    stack<string> parents;
    std::fstream inputfile;
    inputfile.open("inputfilesystem.txt",ios::in); 
    if (inputfile.is_open()){   
      string dir;
   
    int level = 0;
    Node* currentNode;
    Node* root;
    string parent;
    int i = 0;
    while(getline(inputfile, dir)){
        i = i + 1; 
        vector<string> commands = split(dir, " ");
        if (commands[0].compare("$") == 0 && commands[1].compare("cd") == 0 && std::strcmp(commands[2].c_str(),"/") == 0){
            root = newNode("/", "/", 0);
            currentNode = root;
            parents.push(root->key);
            parent = parents.top();
            level = level + 1;
        } else if (commands[0].compare("$") == 0 && commands[1].compare("cd") == 0 && commands[2].compare("..") ==0){
            level = level - 1;
            parents.pop();
            parent = parents.top();
        } else if (commands[0].compare("$") == 0 && commands[1].compare("cd") == 0 && !commands[2].empty()) {
            parent = parents.top();
            parents.push(commands[2]);
            currentNode = search(newNode(commands[2], parent, level), root);
            parent = parents.top();
            level = level + 1;
        } else if (commands[0].compare("dir") == 0 && !commands[1].empty()) {
            currentNode->child.push_back(newNode(commands[1], parent,level));
        } else if (commands[0].compare("$") == 0 && commands[1].compare("ls") == 0){
        } else if (stol(commands[0]) > 0){
            currentNode->child.push_back(newNode(commands[0], parent, level));
        }
    }
    long int volume = 0;
    PrintAll(root, volume);
    cout << "total:" << volume;
    }
    
    inputfile.close();

    return 0;
}
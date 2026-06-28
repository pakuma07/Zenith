# Tries -- NeetCode 150

---

## Problem 61: Implement Trie (Prefix Tree)
**LeetCode #208** | insert, search, startsWith in O(L).

### Brute Force -- O(N*L) search using a set of strings
```cpp
class Trie {
    unordered_set<string> words, prefixes;
public:
    void insert(string w){words.insert(w);for(int i=1;i<=(int)w.size();i++)prefixes.insert(w.substr(0,i));}
    bool search(string w){return words.count(w);}
    bool startsWith(string p){return prefixes.count(p);}
};
```

### Optimal -- O(L) per operation, O(ALPHA * L * N) space
```cpp
class Trie {
    struct Node { Node* ch[26]={}; bool end=false; };
    Node* root;
public:
    Trie():root(new Node()){}
    void insert(string w){ Node* n=root; for(char c:w){int i=c-'a';if(!n->ch[i])n->ch[i]=new Node();n=n->ch[i];}n->end=true; }
    bool search(string w){ Node* n=root; for(char c:w){int i=c-'a';if(!n->ch[i])return false;n=n->ch[i];}return n->end; }
    bool startsWith(string p){ Node* n=root; for(char c:p){int i=c-'a';if(!n->ch[i])return false;n=n->ch[i];}return true; }
};
```

---

## Problem 62: Design Add and Search Words Data Structure
**LeetCode #211** | addWord, search (where '.' matches any letter).

### Brute Force -- O(N*L^2) search through all words
```cpp
class WordDictionary {
    vector<string> words;
public:
    void addWord(string w){words.push_back(w);}
    bool search(string w){
        for(auto& s:words){if(s.size()!=w.size())continue;bool ok=true;for(int i=0;i<(int)w.size();i++)if(w[i]!='.'&&w[i]!=s[i]){ok=false;break;}if(ok)return true;}
        return false;
    }
};
```

### Optimal -- O(26^M) worst case for '.', O(L) average (Trie with DFS on '.')
```cpp
class WordDictionary {
    struct Node{Node* ch[26]={};bool end=false;};
    Node* root;
public:
    WordDictionary():root(new Node()){}
    void addWord(string w){Node* n=root;for(char c:w){int i=c-'a';if(!n->ch[i])n->ch[i]=new Node();n=n->ch[i];}n->end=true;}
    bool search(string w){
        function<bool(Node*,int)> dfs=[&](Node* n,int i)->bool{
            if(i==(int)w.size())return n->end;
            if(w[i]=='.'){for(int c=0;c<26;c++)if(n->ch[c]&&dfs(n->ch[c],i+1))return true;return false;}
            if(!n->ch[w[i]-'a'])return false;
            return dfs(n->ch[w[i]-'a'],i+1);
        };
        return dfs(root,0);
    }
};
```

---

## Problem 63: Word Search II
**LeetCode #212** | Find all words from a list that exist in a board.

### Brute Force -- O(W * M*N * 4^L) run word search for each word separately
```cpp
bool dfs(vector<vector<char>>& b,string& w,int i,int r,int c){
    if(i==(int)w.size())return true;
    if(r<0||r>=(int)b.size()||c<0||c>=(int)b[0].size()||b[r][c]!=w[i])return false;
    char tmp=b[r][c]; b[r][c]='#';
    bool ok=dfs(b,w,i+1,r+1,c)||dfs(b,w,i+1,r-1,c)||dfs(b,w,i+1,r,c+1)||dfs(b,w,i+1,r,c-1);
    b[r][c]=tmp; return ok;
}
vector<string> findWords_bf(vector<vector<char>>& board, vector<string>& words){
    vector<string> res;
    for(auto& w:words){bool found=false;for(int r=0;r<(int)board.size()&&!found;r++)for(int c=0;c<(int)board[0].size()&&!found;c++)if(dfs(board,w,0,r,c)){res.push_back(w);found=true;}}
    return res;
}
```

### Optimal -- O(M*N*4*3^(L-1)) build Trie of all words, single board DFS
```cpp
vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
    struct Node{Node* ch[26]={};string word="";};
    Node* root=new Node();
    for(auto& w:words){Node* n=root;for(char c:w){int i=c-'a';if(!n->ch[i])n->ch[i]=new Node();n=n->ch[i];}n->word=w;}
    int m=board.size(),n=board[0].size();
    vector<string> res;
    function<void(Node*,int,int)> dfs=[&](Node* node,int r,int c){
        if(r<0||r>=m||c<0||c>=n||board[r][c]=='#') return;
        char ch=board[r][c]; Node* next=node->ch[ch-'a'];
        if(!next) return;
        if(!next->word.empty()){res.push_back(next->word);next->word="";}
        board[r][c]='#';
        dfs(next,r+1,c);dfs(next,r-1,c);dfs(next,r,c+1);dfs(next,r,c-1);
        board[r][c]=ch;
    };
    for(int r=0;r<m;r++) for(int c=0;c<n;c++) dfs(root,r,c);
    return res;
}
```
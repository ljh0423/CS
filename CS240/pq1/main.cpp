#include <iostream>
#include <memory>
#include <queue>
#include <vector>

//used for implementation 1
class Node {
public:
    std::pair<int,int> kv;
    std::shared_ptr<Node> nxt;
};

//implementation 1 insert function
void pq1in(std::shared_ptr<Node>& cur,int p, int t) {
  std::shared_ptr<Node> newn = std::make_shared<Node>();
  newn->kv.first = p;
  newn->kv.second = t;
  newn->nxt = nullptr;
  std::shared_ptr<Node> prev = cur;
  std::shared_ptr<Node> og = cur;
  if (cur == nullptr) { cur = newn; }
  else if (p > cur->kv.first) {
    newn->nxt = cur;
    cur = newn;
  }
  else {
    while (cur != nullptr){
      if (p > cur->kv.first) {
        newn->nxt = cur;
        prev->nxt = newn;
        cur = og;
        break;
      }
      else if (cur->nxt == nullptr) {
        cur->nxt = newn;
        cur = og;
        break;
      }
      else {
        prev = cur;
        cur = cur->nxt;
      }
    }
  }
}
//implementation 1 delete/findMax function
void pq1df(std::shared_ptr<Node>& cur, int isd){
  if (cur != nullptr){
    std::cout << cur->kv.first << " " << cur->kv.second << std::endl;
    if (isd){ cur = cur->nxt; }
  }
}
//implementation 1 lookup function
void pq1l(std::shared_ptr<Node> cur, int ith){
  for(int i=0;i<=ith;i++){
    if (i != ith){ cur = cur->nxt; }
    else { std::cout << cur->kv.second << std::endl; }
  }
}
//implementation 2 insert function
void pq2in(std::pair<int,int>*& ar,int p, int t, int& n, int& s){
  if (s == 0) {
    ar = (std::pair<int,int>*) malloc(1 * sizeof(std::pair<int,int>));
    ar[0] = std::make_pair (p,t);
    n = 1;
    s = 1;
  }
  else if (n == s){
    ar = (std::pair<int,int>*) realloc(ar, 2 * s * sizeof(std::pair<int,int>));
    ar[n] = std::make_pair (p,t);
    n++;
    s = 2 * s;
  }
  else {
    ar[n] = std::make_pair (p,t);
    n++;
  }
  int it = n-1;
  while (it != 0 && ar[(it-1) / 2].first < ar[it].first){
    std::pair<int,int> tmp = ar[(it-1) / 2];
    ar[(it-1) / 2] = ar[it];
    ar[it] = tmp;
    it = (it-1)/2;
  }
}
//implementation 2 delete/findMax function
void pq2df(std::pair<int,int>*& ar, int& n, int isd){
  if (ar != NULL && n != 0){
    std::cout << ar[0].first << " " << ar[0].second << std::endl;
    if (isd){
      ar[0] = ar[n-1];
      n--;
      if (n == 0) { ar = (std::pair<int,int>*) realloc(ar, 1 * sizeof(std::pair<int,int>)); }
      int i=0;
      int j = 2*i+1;
      while (j < n){
        if (j+1 < n){
          if (ar[j+1].first > ar[j].first){ j++; }
        }
        if (ar[i].first < ar[j].first){
          std::pair<int,int> tmp = ar[j];
          ar[j] = ar[i];
          ar[i] = tmp;
          i = j;
        }
        else { break; }
      }
    }
  }
}
//implementation 3 insert function
void pq3in(std::vector<std::queue<int>*>& v,int p, int t){
  if (p >= v.size()) {
    for(int i=v.size();i<=p;i++){
      if(i != p){ v.push_back(nullptr); }
      else {
        std::queue<int>* q = new std::queue<int>();
        q->push(t);
        v.push_back(q);
      }
    }
  }
  else {
    if (v[p] != nullptr){ v[p]->push(t); }
    else {
      std::queue<int>* q = new std::queue<int>();
      q->push(t);
      v[p] = q;
    }
  }
}
//implementation 3 delete/findMax function
void pq3df(std::vector<std::queue<int>*>& v, int isd){
  if (v.size() != 0){
    std::cout << v.size()-1 << " " << v.back()->front() << std::endl;
    if (isd){
      v.back()->pop();
      if (v.back()->empty()){
        delete v.back();
        v.pop_back();
        for(int i=v.size()-1;i>=0;i--){
          if (v.back() == nullptr) { v.pop_back(); }
          else { break; }
        }
      }
    }
  }
}
//implementation 3 lookup function
void pq3l(std::vector<std::queue<int>*> v, int ith){
  if (v[ith] != nullptr){
    std::queue<int> v2 = *(v[ith]);
    int n = v2.size();
    for(int i=0;i<n;i++){
      std::cout << v2.front() << " ";
      v2.pop();
    }
    std::cout << std::endl;
  }
}

int main() {
  char c;
  int ts=1, num, pr, i, size=0, ne=0;
  std::shared_ptr<Node> h (nullptr);
  std::pair<int,int>* dar = NULL;
  std::vector<std::queue<int>*> vec;
  while (std::cin >> c){
    if (c == 'i') {
      std::cin >> num >> pr;
      if (num == 1){ pq1in(h,pr,ts); }
      else if (num == 2){ pq2in(dar, pr, ts, ne, size); }
      else if (num == 3){ pq3in(vec, pr, ts); }
      ts++;
    }
    else if (c == 'd'){
      std::cin >> num;
      if (num == 1){ pq1df(h, 1); }
      else if (num == 2){ pq2df(dar, ne, 1); }
      else if (num == 3){ pq3df(vec, 1); }
    }
    else if (c == 'f'){
      std::cin >> num;
      if (num == 1){ pq1df(h,0); }
      else if (num == 2){ pq2df(dar, ne, 0); }
      else if (num == 3){ pq3df(vec, 0); }
    }
    else if (c == 'l'){
      std::cin >> num >> i;
      if (num == 1){ pq1l(h,i); }
      else if (num == 2){ std::cout << dar[i].second << std::endl; }
      else if (num == 3){ pq3l(vec, i); }
    }
    else if (c == 'r'){
      h = nullptr;
      free(dar);
      dar = NULL;
      size = 0;
      ne = 0;
      for(int k=0;k<vec.size();k++){
        delete vec[k];
      }
      vec.clear();
    }
    else if (c == 'x'){ 
      free(dar);
      for(int k=0;k<vec.size();k++){
        delete vec[k];
      }
      break;
    }
  }
  return 0;
}
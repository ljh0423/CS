#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>

struct node{
  std::string tok;
  std::string lex;
  std::string type;
  std::vector<node> cn;
};

void ptree(node n){
  if (n.cn.empty()) {
    std::cout<<n.tok<<" "<<n.lex;
    if(!n.type.empty()){std::cout<<" : "<<n.type;}
    std::cout<<std::endl;
  }
  else {
    std::cout<<n.tok<<" ";
    for(int i=0;i<n.cn.size();i++){      
      if (i==n.cn.size()-1) { std::cout<<n.cn[i].tok;}
      else {std::cout<<n.cn[i].tok<<" ";}
    }
    if(!n.type.empty()){std::cout<<" : "<<n.type;}
    std::cout<<std::endl;
    for(int i=0;i<n.cn.size();i++){      
      ptree(n.cn[i]);
    }
  }
}

node mkt(){
    std::string s;
    getline(std::cin,s);
    std::stringstream ss;
    std::string t;
    ss << s;
    ss >> t;
    if(toupper(t[0]) == t[0]){
        node n;
        n.tok = t;
        ss >> s;
        n.lex = s;
        return n;
    } else {
        node n;
        n.tok = t;
        while(ss >> s){
            if(s.compare(".EMPTY")!=0){
                n.cn.push_back(mkt());
            }
            else {
                n.lex = ".EMPTY";
            }
        }
        return n;
    }
}

void mst(node n, std::vector<std::string> &ar){
  if(n.cn.size()==1){
    ar.push_back(n.cn[0].type);
  }
  else {
    ar.push_back(n.cn[0].type);
    mst(n.cn[2],ar);
  }
}

int tchk(node &n, std::vector<node> &v, std::vector<std::pair<std::string,std::vector<std::string>>> &p){
  if((n.tok).compare("procedure")==0){
    std::vector<node> st;
    std::vector<std::string> arg;
    tchk(n.cn[3],st,p);
    if(!n.cn[3].cn.empty()){
      mst(n.cn[3],arg);
    }
    p.push_back(std::make_pair(n.cn[1].lex,arg));
    for(int i=4;i<n.cn.size();i++){
      if(tchk(n.cn[i],st,p)) {return 1;}
    }
    return 0;
  }
  else if((n.tok).compare("dcl")==0) {
    for(int i=0;i<v.size();i++){
      if(n.cn[1].lex==v[i].lex){
        std::cerr << "ERROR"<<std::endl;
        return 1;
      }
    }
    if(n.cn[0].cn.size()==1){n.cn[1].type = "int";}
    else {n.cn[1].type = "int*";}
    v.push_back(n.cn[1]);
  }
  /*else if((n.tok).compare("main")==0){}*/
  else {
    for(int i=0;i<n.cn.size();i++){
      if(tchk(n.cn[i],v,p)) {return 1;}
    }
  }
  if((n.tok).compare("NUM")==0) { n.type = "int"; }
  else if((n.tok).compare("NULL")==0) { n.type = "int*"; }
  else if((n.tok).compare("dcls")==0) {
    if(!n.cn.empty()){
      if((n.cn[1].cn[1].type).compare(n.cn[3].type)!=0) {std::cerr << "ERROR"<<std::endl; return 1;}
      if((n.cn[1].cn[1].type).compare("int*")==0){
        if((n.cn[3].tok).compare("NULL")!=0) {std::cerr << "ERROR"<<std::endl; return 1;}
      }
    }
  }
  else if((n.tok).compare("ID")==0){
    for(int i=0;i<v.size();i++){
      if((n.lex).compare(v[i].lex)==0){
        n.type = v[i].type;
        return 0;
      }
    }
    for(int i=0;i<p.size();i++){
      if((n.lex).compare(p[i].first)==0){ return 0; }
    }
    std::cerr << "ERROR"<<std::endl;
    return 1;
  }
  else if((n.tok).compare("expr")==0){
    if(n.cn.size()==1){ n.type = n.cn[0].type; }
    else if(n.cn.size()==3) {
      if((n.cn[1].tok).compare("PLUS")){
        if((n.cn[0].type).compare("int")==0 || (n.cn[2].type).compare("int")==0){ n.type = "int"; }
        else { std::cerr << "ERROR"<<std::endl; return 1;}
      }
      else if((n.cn[1].tok).compare("MINUS")){
        if((n.cn[0].type).compare(n.cn[2].type)==0){ n.type = "int"; }
        else if((n.cn[0].type).compare("int*")==0 || (n.cn[2].type).compare("int")==0){ n.type = "int*"; }
        else { std::cerr << "ERROR"<<std::endl; return 1;}
      }
    }
  }
  else if((n.tok).compare("term")==0){
    if(n.cn.size()==1){ n.type = n.cn[0].type; }
    else if(n.cn.size()==3) {
      if((n.cn[0].type).compare("int")==0 && (n.cn[2].type).compare("int")==0){ n.type = "int"; }
      else { std::cerr << "ERROR"<<std::endl; return 1;}
    }
  }
  else if((n.tok).compare("factor")==0 || (n.tok).compare("lvalue")==0){
    if(n.cn.size()==1){
      n.type = n.cn[0].type;
    }
    else if(n.cn.size()==2){
      if((n.cn[0].tok).compare("AMP")==0){
        if((n.cn[1].type).compare("int")==0) { n.type = "int*"; }
        else {std::cerr << "ERROR"<<std::endl; return 1; }
      }
      else if((n.cn[0].tok).compare("STAR")==0){
        if((n.cn[1].type).compare("int*")==0) { n.type = "int"; }
        else {std::cerr << "ERROR"<<std::endl; return 1; }
      }
    }
    else if((n.cn[0].tok).compare("LPAREN")==0){
      n.type = n.cn[1].type;
    }
    else if((n.cn[0].tok).compare("NEW")==0){
      if((n.cn[3].type).compare("int")==0) { n.type = "int*"; }
      else {std::cerr << "ERROR"<<std::endl; return 1; }
    }
    else if((n.cn[0].tok).compare("ID")==0){
      int e=1;
      std::vector<std::string> n1,n2;
      for(int i=0;i<v.size();i++){
        if((n.cn[0].lex).compare(v[i].lex)==0){ std::cerr << "ERROR"<<std::endl; return 1; }
      }
      for(int i=0;i<p.size();i++){
        if((n.cn[0].lex).compare(p[i].first)==0){ e=0; n1 = p[i].second; }
      }
      if(e) {std::cerr << "ERROR"<<std::endl; return 1;}
      if(n.cn.size()==4){
        mst(n.cn[2],n2);
        if(n1.size() == n2.size()){
          for(int k=0;k<n1.size();k++){
            if((n1[k]).compare(n2[k])!=0) {std::cerr << "ERROR"<<std::endl; return 1;}
          }
        } else {std::cerr << "ERROR"<<std::endl; return 1;}
      }
      n.type = "int";
    }
  }
  else if((n.tok).compare("test")==0){
    if((n.cn[0].type).compare(n.cn[2].type)!=0){ std::cerr << "ERROR"<<std::endl; return 1; }
  }
  return 0;
}

int main() {
    std::string s;
    node rn;
    std::vector<node> st;
    rn = mkt();
    std::vector<std::pair<std::string,std::vector<std::string>>> pro;
    tchk(rn,st,pro);
    ptree(rn);
}

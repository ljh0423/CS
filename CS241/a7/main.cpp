#include <iostream>
#include <vector>
#include <sstream>
#include <tuple>

struct node{
  std::string tok;
  std::string lex;
  std::string type;
  std::vector<node*> cn;
};

int ifc=0, wh=0;

void ptree(node* n){
  if (n->cn.empty()) {
    std::cout<<n->tok<<" "<<n->lex;
    if(!n->type.empty()){std::cout<<" : "<<n->type;}
    std::cout<<std::endl;
  }
  else {
    std::cout<<n->tok<<" ";
    for(int i=0;i<n->cn.size();i++){      
      if (i==n->cn.size()-1) { std::cout<<n->cn[i]->tok;}
      else {std::cout<<n->cn[i]->tok<<" ";}
    }
    if(!n->type.empty()){std::cout<<" : "<<n->type;}
    std::cout<<std::endl;
    for(int i=0;i<n->cn.size();i++){
      ptree(n->cn[i]);
    }
  }
}


node* mkt(){
    std::string s;
    getline(std::cin,s);
    std::stringstream ss;
    std::string t;
    ss << s;
    ss >> t;
    if(toupper(t[0]) == t[0]){
        node* n = new node;
        n->tok = t;
        ss >> s;
        n->lex = s;
        if(ss >> s) {ss >> s; n->type = s;}
        return n;
    } else {
        node* n = new node;
        n->tok = t;
        while(ss >> s){
            if (s.compare(":")==0){
              ss >> s;
              n->type = s;
            }
            else if(s.compare(".EMPTY")!=0){
                n->cn.push_back(mkt());
            }
            else {
                n->lex = ".EMPTY";
            }
        }
        return n;
    }
}
void del(node* n){
  for(int i=0;i<n->cn.size();i++){ del(n->cn[i]); }
  delete n;
}

void gen(node* n, std::vector<std::tuple<std::string,std::string,int>>& v, int& o){
  std::string s = n->tok;
  if (s.compare("start")==0){ gen(n->cn[1],v,o); }
  else if (s.compare("procedures")==0 || s.compare("statements")==0){
    for(int i=0;i<n->cn.size();i++){ gen(n->cn[i],v,o); }
  }
  else if (s.compare("main")==0){
    std::cout << "wain:\n";
    std::cout << "sub $29, $30, $4\n";
    std::cout << "sw $1, -4($30)\nsub $30, $30, $4\n";
    std::cout << "sw $2, -4($30)\nsub $30, $30, $4\n";
    v.push_back(std::make_tuple(n->cn[3]->cn[1]->lex,n->cn[3]->cn[1]->type,o));
    o-=4;
    v.push_back(std::make_tuple(n->cn[5]->cn[1]->lex,n->cn[5]->cn[1]->type,o));
    o-=4;
    if(n->cn[3]->type.compare("int")==0){std::cout<<"add $2, $0, $0\n";}
    std::cout<<"sw $31, -4($30)\nsub $30, $30, $4\nlis $5\n.word init\njalr $5\nadd $30, $30, $4\nlw $31, -4($30)\n";
    for(int i=8;i<12;i++){ gen(n->cn[i],v,o); }
    o = o / -4;
    std::cout << "lis $5\n.word " << o << "\nmult $4, $5\nmflo $5\nadd $30, $30, $5\njr $31" << std::endl;
  }
  else if(s.compare("procedure")==0){
    
  }
  else if(s.compare("dcls")==0){
    if(n->cn.size()!=0){
      node* id = n->cn[1]->cn[1];
      if(n->cn[3]->lex.compare("NULL")==0){std::cout << "add $3, $0, $11\n"; }
      else {std::cout << "lis $3\n.word " << n->cn[3]->lex <<std::endl;}
      std::cout << "sw $3, -4($30)\nsub $30, $30, $4\n";
      v.push_back(std::make_tuple(id->lex,id->type,o));
      o-=4;
      gen(n->cn[0],v,o);
    }
  }
  else if(s.compare("expr")==0 || s.compare("term")==0){
    if(n->cn.size()==1){ gen(n->cn[0],v,o); }
    else {
      gen(n->cn[0],v,o);
      std::cout << "sw $3, -4($30)\nsub $30, $30, $4\n";
      gen(n->cn[2],v,o);
      std::cout << "add $30, $30, $4\nlw $5, -4($30)\n";
      std::string op = n->cn[1]->tok;
      if(op.compare("PLUS")==0){ 
        if(n->cn[0]->type.compare("int*")==0){std::cout<<"mult $3, $4\nmflo $3\n";}
        else if(n->cn[2]->type.compare("int*")==0){std::cout<<"mult $5, $4\nmflo $5\n";}
        std::cout << "add $3, $5, $3\n";
      }
      else if(op.compare("MINUS")==0){
        if(n->cn[0]->type.compare("int*")==0 && n->cn[2]->type.compare("int")==0){
          std::cout<<"mult $3, $4\nmflo $3\n"; }
        std::cout << "sub $3, $5, $3\n";
        if(n->cn[0]->type.compare("int*")==0 && n->cn[2]->type.compare("int*")==0){
          std::cout<<"div $3, $4\nmflo $3\n"; }
      }
      else if(op.compare("STAR")==0) { std::cout << "mult $5, $3\nmflo $3\n"; }
      else if(op.compare("SLASH")==0) { std::cout << "div $5, $3\nmflo $3\n"; }
      else if(op.compare("PCT")==0) { std::cout << "div $5, $3\nmfhi $3\n"; }
    }
  }
  else if(s.compare("lvalue")==0){
    if(n->cn.size()==1){
      int off;
      for(int i=0;i<v.size();i++){
        if(n->cn[0]->lex.compare(std::get<0>(v[i]))==0) {off = std::get<2>(v[i]);break;}
      }
      std::cout<< "lis $3\n.word "<<off<<"\nadd $3, $3, $29\n";
    }
    else { gen(n->cn[1],v,o); }
  }
  else if(s.compare("factor")==0){
    if(n->cn.size()==1){
      if(n->cn[0]->tok.compare("ID")==0){
        int off;
        for(int i=0;i<v.size();i++){
          if(n->cn[0]->lex.compare(std::get<0>(v[i]))==0) {off = std::get<2>(v[i]);break;}
        }
        std::cout<<"lw $3, "<<off<<"($29)\n";
      }
      else if(n->cn[0]->tok.compare("NUM")==0){
        std::cout << "lis $3\n.word " << n->cn[0]->lex <<std::endl;
      }
      else {
        std::cout << "add $3, $0, $11\n";
      }
    }
    else if(n->cn[0]->tok.compare("AMP")==0 || n->cn[0]->tok.compare("LPAREN")==0){ gen(n->cn[1],v,o); }
    else if(n->cn[0]->tok.compare("STAR")==0){
      gen(n->cn[1],v,o);
      std::cout<<"lw $3, 0($3)\n";
    }
  }
  else if(s.compare("test")==0){
    gen(n->cn[0],v,o);
    std::cout << "sw $3, -4($30)\nsub $30, $30, $4\n";
    gen(n->cn[2],v,o);
    std::cout << "add $30, $30, $4\nlw $5, -4($30)\n";
    std::string op = n->cn[1]->tok;
    if(op.compare("EQ")==0 || op.compare("NE")==0) {
      if(n->cn[0]->type.compare("int")==0) {
        std::cout << "slt $6, $3, $5\nslt $7, $5, $3\nadd $3, $6, $7\n";}
      else {
        std::cout << "sltu $6, $3, $5\nsltu $7, $5, $3\nadd $3, $6, $7\n";}
    }
    else if(op.compare("LT")==0 || op.compare("GE")==0) { 
      if(n->cn[0]->type.compare("int")==0){
        std::cout << "slt $3, $5, $3\n";}
      else {
        std::cout << "sltu $3, $5, $3\n";}
    }
    else if(op.compare("GT")==0 || op.compare("LE")==0) {
      if(n->cn[0]->type.compare("int")==0){
        std::cout << "slt $3, $3, $5\n";}
      else {
        std::cout << "sltu $3, $3, $5\n";}
    }
    if(op.compare("EQ")==0 || op.compare("LE")==0 || op.compare("GE")==0){
      std::cout<<"sub $3, $11, $3\n";
    }
  }
  else if(s.compare("statement")==0){
    std::string t = n->cn[0]->tok;
    if(t.compare("lvalue")==0){
      gen(n->cn[2],v,o);
      std::cout << "sw $3, -4($30)\nsub $30, $30, $4\n";
      gen(n->cn[0],v,o);
      std::cout << "add $30, $30, $4\nlw $5, -4($30)\nsw $5, 0($3)\n";
    }
    else if(t.compare("IF")==0){
      gen(n->cn[2],v,o);
      std::cout<<"beq $3, $0, else"<<ifc<<std::endl;
      gen(n->cn[5],v,o);
      std::cout<<"beq $0, $0, endif"<<ifc<<"\nelse"<<ifc<<":\n";
      gen(n->cn[9],v,o);
      std::cout<<"endif"<<ifc<<":\n";
      ifc++;
    }
    else if(t.compare("WHILE")==0){
      std::cout<<"loop"<<wh<<":\n";
      gen(n->cn[2],v,o);
      std::cout<<"beq $3, $0, endWhile"<<wh<<std::endl;
      gen(n->cn[5],v,o);
      std::cout<<"beq $0, $0, loop"<<wh<<"\nendWhile"<<wh<<":\n";
    }
    else if(t.compare("PRINTLN")==0){
      std::cout << "sw $1, -4($30)\nsub $30, $30, $4\n";
      gen(n->cn[2],v,o);
      std::cout<<"add $1, $3, $0\nsw $31, -4($30)\nsub $30, $30, $4\n";
      std::cout<<"sw $31, -4($30)\nsub $30, $30, $4\nlis $5\n.word print\njalr $5\nadd $30, $30, $4\nlw $31, -4($30)\n";
      std::cout << "add $30, $30, $4\nlw $1, -4($30)\n";
    }
  }
}

int main() {
  node* rn;
  std::vector<std::tuple<std::string,std::string,int>> v;
  int i=0;
  rn = mkt();
  //ptree(rn);
  std::cout << ".import print\n.import init\n.import new\n.import delete\n";
  std::cout << "lis $4\n.word 4\n";
  std::cout << "lis $10\n.word print\n";
  std::cout << "lis $11\n.word 1\nbeq $0, $0, wain\n";
  gen(rn,v,i);
  del(rn);
}
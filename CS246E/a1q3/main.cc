#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void egrep(istream &f, string &p, char &op, int pos, int fla, int &ex) {
	f >> noskipws;
	string line;
  int c=0, k=0, l=0, print=0;
	while (getline(f, line)) {
    l++;
    c=0;
    k=0;
    print=0;
    if (fla%100>=10){
      for (int s=0;s<line.length();++s){
        line[s] = tolower(line[s]);
      }
      for (int t=0;t<p.length();++t){
        p[t] = tolower(p[t]);
      }
    }
		for (int i=0;i<line.length();++i){
      if(p.length() < c+1) {
        print=1;
        break;
      }
      else if(p[c]=='\\'){
        if (p[c+1]==line[i]) c+=2;
        else break;
      }
      else if(p[c]=='|'){
        k++;
        print=1;
        break;
      }
      else if(op=='$' && pos==-1){
        break;
      }
      else if(p[c]=='.'){
        if(p.length() == c+1) {
          print=1;
          break;
        }
        else c++;
      }
      else if(p[c]=='^'){
        if(c==0) {
          c++;
          i--;
        }
        else break;
      }
      else if(p[c]=='['){
        int n=0,m=0;
        for (int b=c+1;b<p.length();++b){
          if (p[b]==line[i]) n=1;
          if (p[b]==']'){
            m=b;
            break;
          }
        }
        if (op=='['){
          if (n==1) {
            if(p.length() == m+1) {
              print=1;
              break;
            }
            else c=m+1;
          }
          else if(c!=0) {
            c=0;
            i--;
          }
        }
        else if(op=='{'){
          if (n==0) {
            if(p.length() == m+1) {
              print=1;
              break;
            }
            else c=m+1;
          }
          else if(c!=0) {
            c=0;
            i--;
          }
        }
      }
      else if(p[c] == line[i] && c!=pos){
        if(p.length() == c+1) {
          if(op=='$' && line.length()!=i+1) break;
          print=1;
          break;
        }
        else c++;
      }
      else if(op=='|' && i+1==line.length() && k==0){
        k++;
        i = -1;
        c = pos+1;
      }
      else if (op=='|' && k>0){
        if (c!=pos+1) c=pos+1;
      }
      else if(c!=0) {
        if (op=='^' && pos==0) break;
        else c=0;
        i--;
      }
    }
    if(p[c]=='|' && k==0) {
      print=1;
    }
    if(p.length() < c+1) {
      print=1;
    }
    if(fla%10>=1){
      if(!print){
        if(fla>=100) {ex=0; cout<<l<<":"<<line<<endl;}
        else {ex=0; cout<<line<<endl;}
      }
    }
    else {
      if(print){
        if(fla>=100) {ex=0; cout<<l<<":"<<line<<endl;}
        else {ex=0; cout<<line<<endl;}
      }
    }
	}
}

int main(int argc, char *argv[]) {
	int fl = 0, fc = 0, po=-1, e=1;
  string pat="";
  char o=' ';
	for (int i = 1; i < argc; ++i) {
		if (string(argv[i]) == "-n") {
			fl += 100;
			fc++;
		}
    else if (string(argv[i]) == "-i") {
			fl += 10;
			fc++;
		}
    else if (string(argv[i]) == "-v") {
			fl += 1;
			fc++;
		}
    else {
      if (i-fc==1) {
        pat = string(argv[i]);
        for (int a=0;a<pat.length();++a){
          if (pat[a]=='|'){
            if (pat[a-1]!='\\') {
              o='|';
              po = a;
            }
          }
          if (pat[a]=='*'){
            if (pat[a-1]!='\\') {
              if(a<=1) {
                if(pat.length()==a+1) pat="";
                else pat = pat.substr(a+1);
              }
              else {
                if(pat.length()==a+1) pat=pat.substr(0,a-1);
                else pat = pat.substr(0,a-1)+pat.substr(a+1);
              }
              o='*';
            }
          }
          if (pat[a]=='+'){
            if (pat[a-1]!='\\') {
              if(a<=1) {
                if(pat.length()==a+1) pat=pat[a-1];
                else pat = pat[a-1]+pat.substr(a+1);
              }
              else {
                if(pat.length()==a+1) pat=pat.substr(0,a);
                else pat = pat.substr(0,a)+pat.substr(a+1);
              }
              o='+';
            }
          }
          if (pat[a]=='?'){
            if (pat[a-1]!='\\') {
              if(a<=1) {
                if(pat.length()==a+1) pat="";
                else pat = pat.substr(a+1);
              }
              else {
                if(pat.length()==a+1) pat=pat.substr(0,a-1);
                else pat = pat.substr(0,a-1)+pat.substr(a+1);
              }
              o='?';
            }
          }
          if (pat[a]=='.'){
            if (pat[a-1]!='\\') {
              o='.';
              po = a;
            }
          }
          if (pat[a]=='^'){
            if (pat[a-1]!='\\' && pat[a-1]!='[') {
              o='^';
              po = a;
            }
          }
          if (pat[a]=='$'){
            if (pat[a-1]!='\\') {
              o='$';
              if (a+1==pat.length()){
                pat = pat.substr(0,a);
                po = a;
              }
            }
          }
          if (pat[a]=='['){
            if (pat[a-1]!='\\'){
              if (pat[a+1]=='^'){
                pat = pat.substr(0,a+1)+pat.substr(a+2);
                o='{';
                po=a;
              }
              else {
                o='[';
                po=a;
              }
            }
          }
        }
      }
      else{
        ifstream f{argv[i]};
			  egrep(f, pat, o, po, fl, e);
      }
		}
	}
  if (argc - fc == 2) egrep(cin, pat, o, po, fl, e);
  exit(e);
}

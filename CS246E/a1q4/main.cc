#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <sys/wait.h>
#include <string>

using namespace std;

void run(string t, string p) {
	string temp, tt;
  int a,i,o,ret;
  ret = system(("test -f " + t + ".args").c_str());
  a = ret;
  ret = system(("test -r " + t + ".args").c_str());
  a += ret;
  ret = system(("test -f " + t + ".in").c_str());
  i = ret;
  ret = system(("test -r " + t + ".in").c_str());
  i += ret;
  ret = system(("test -f " + t + ".out").c_str());
  o = ret;
  ret = system(("test -r " + t + ".out").c_str());
  o += ret;
  if (o!=0){
    cerr << t << " missing or unreadable .out file"<<endl;
    exit(1);
  }
  FILE* k = popen( "mktemp", "r" );
  char tf[100];
  fscanf(k,"%s", tf);
  pclose(k);
  if (a==0){
    if (i==0){
      temp = p + " $(cat "+ t + ".args) < " + t + ".in > " + string(tf);
    }
    else temp = p + " $(cat "+ t + ".args) > " + string(tf);
  }
  else {
    if (i==0){
      temp = p + " < " + t + ".in > " + string(tf);
    }
    else {cerr << t << " missing both .args and .in files"<<endl; exit(1);}
  }
  string l, l1;
  int em=1;
  ret = system(temp.c_str());
  ret = system(("cmp " + t + ".out " + string(tf).c_str() + " > /dev/null").c_str());
  if (ret!=0){
    cout << "Test failed: " << t << endl;
    cout << "Args:" <<endl;
    if (a==0){
      ifstream args((t + ".args").c_str());
      while (getline(args, l)){
        if (l=="" && em==1) {
          int len = args.tellg();
          if (getline(args, l1)){
            cout <<l<<endl;
            args.seekg(len, args.beg);
          }
        }
        else {em=0; cout <<l<<endl;}
      }
    }
    em=1;
    cout << "Input:" <<endl;
    if (i==0){
      ifstream inp((t + ".in").c_str());
      while (getline(inp, l)){
        if (l=="" && em==1) {
          int len = inp.tellg();
          if (getline(inp, l1)){
            cout <<l<<endl;
            inp.seekg(len, inp.beg);
          }
        }
        else {em=0; cout <<l<<endl;}
      }
    }
    em=1;
    cout << "Expected:"<<endl;
    ifstream exp((t + ".out").c_str());
    while (getline(exp, l)){
        if (l=="" && em==1) {
          int len = exp.tellg();
          if (getline(exp, l1)){
            cout <<l<<endl;
            exp.seekg(len, exp.beg);
          }
        }
        else {em=0; cout <<l<<endl;}
      }
    cout << "Actual:"<<endl;
    ifstream act(string(tf).c_str());
    cout << act.rdbuf();
  }
  system(("rm " + string(tf)).c_str());
}

int main(int argc, char *argv[]) {
	if (argc!=3) {
    cerr << "incorrect number of command line arguments"<<endl;
    return 1;
  }
  ifstream suite{argv[1]};
  if (!suite.good()) {
    cerr << string(argv[1]) << " missing or unreadable" << endl;
    return 1;
  }
  string program = string(argv[2]);
  string test;
  while (suite >> test){
    run(test, program);
  }
  return 0;
}

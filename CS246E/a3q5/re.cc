/*
This file contains the parsing functions which parse a regex expression along with containsMatch which return a boolean value of whether the string s contains a match to the regex expression, and << function.
*/
#include <string>
#include <memory>
#include <iostream>
#include "re.h"

namespace CS246E {
    std::unique_ptr<RegExp> parseDisjunction(const std::string &s) {
      int a = 0,b=0;
      size_t i;
      for (i=0;i<s.length();++i){
        if (s[i]=='|' && a==0) {b=1;break;}
        else if (s[i]=='(') {++a;}
        else if (s[i]==')') {--a;}
      }
      if (b==0) {return parseConcatenation(s);}
      else {
        return std::make_unique<Disjunction>  (
          parseDisjunction(s.substr(0,i)),
          parseDisjunction(s.substr(i+1))
        );
      }
    }
    std::unique_ptr<RegExp> parseConcatenation(const std::string &s) {
      if (s.empty()) {
        auto r = parseWord(s);
        return std::move(r);
      }
      int a = 0;
      size_t i;
      if (s[0]=='(') {
        for(i=0;i<s.length();++i) {
          if (s[i]=='(') ++a;
          else if (s[i]==')') {
            if (a==1) {
              if (s[i+1]=='*') {
                size_t b=i+2;
                auto r = std::make_unique<Star> (parseDisjunction(s.substr(1,i-1)));
                for (;b<s.length();++b){
                  if (s[b]!='*') break;
                  r = std::make_unique<Star> (std::move(r));
                }
                if (b==s.length()) return std::move(r);
                else return std::make_unique<Concatenation> (
                  std::move(r),
                  parseDisjunction(s.substr(b))
                );
              }
              if (i==s.length()-1) return parseDisjunction(s.substr(1,i-1));
              else return std::make_unique<Concatenation> (
                parseDisjunction(s.substr(1,i-1)),
                parseDisjunction(s.substr(i+1))
              );
            }
            --a;
          }
        }
      }
      else if (s[0]=='*') {
        auto r = std::make_unique<Star> (parseDisjunction(s.substr(0,0)));
        size_t b=1;
        for (;b<s.length();++b){
          if (s[b]!='*') break;
          r = std::make_unique<Star> (std::move(r));
        }
        if (b==s.length()) return std::move(r);
        return std::make_unique<Concatenation> (
          std::move(r),
          parseDisjunction(s.substr(b))
        );
      }
      else if (s[1]=='*') {
        auto r = std::make_unique<Star> (parseDisjunction(s.substr(0,1)));
        size_t b=2;
        for (;b<s.length();++b){
          if (s[b]!='*') break;
          r = std::make_unique<Star> (std::move(r));
        }
        if (b==s.length()) return std::move(r);
        return std::make_unique<Concatenation> (
          std::move(r),
          parseDisjunction(s.substr(b))
        );
      }
      else {
        if (s.length()==1) return parseWord(s);
        else return std::make_unique<Concatenation> (
          parseWord(s.substr(0,1)),
          parseDisjunction(s.substr(1))
        );
      }
    }
  std::unique_ptr<RegExp> parseWord(const std::string &s) {
    if (s=="^") {return std::make_unique<Begin>();}
    else if (s=="$") {return std::make_unique<End>();}
    else if (s==".") {return std::make_unique<Dot>();}
    else return std::make_unique<Word> (s);
  }
  std::ostream &operator<<(std::ostream &out, RegExp &re) {
    return re.print(out);
  }
}

bool containsMatch(CS246E::RegExp *re, const std::string &s) {
  std::string s2 = '\200' + s + '\201';
  for (size_t i=0;i<s2.length();++i) {
    for (size_t t=0;t<s2.length()-i+1;++t) {
      if (re->matches(s2.substr(i,t))) return 1;
    }
  }
  return false;
}

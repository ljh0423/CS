#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "scanner.h"

/*
 * C++ Starter code for CS241 A3
 * All code requires C++14, so if you're getting compile errors make sure to
 * use -std=c++14.
 *
 * This file contains the main function of your program. By default, it just
 * prints the scanned list of tokens back to standard output.
 */

bool oir(int i, bool isHex) {
  if (isHex) {
    // at most 4 hex digits fit in the offset string
    // 4 digits = 16 bits, stored in a 32-bit int
    // so if the offset is in hex, it is non-negative
    return 0 <= i && i <= 65535;
  }
  // decimal case (number can be negative)
  return -32768 <= i && i <= 32767;
}

void hl(std::vector<Token> &t, std::map<std::string,int> &m, int l){
  for (int i=0;i<t.size();i++){
    if(t[i].getKind() == Token::LABEL) {
      std::string str = t[i].getLexeme();
      str.pop_back();
      //check if the label does not already exist
      m[str] = l;
      t.erase(t.begin()+i);
      i--;
    }
  }
}

bool tal(std::vector<Token> t){
  for (int i=0;i<t.size();i++){
    if(t[i].getKind() != Token::LABEL) return 0;
  }
  return 1;
}

int main() {
  std::string line;
  int pc=0;
  std::vector<std::vector<Token>> input;
  std::map<std::string,int> st;
  try {
    while (getline(std::cin, line)) {
      std::vector<Token> temp = scan(line);
      hl(temp,st,pc);
      if (tal(temp)) {continue;}
      input.push_back(temp);
      pc+=4;
    }
    pc=0;
    for (int in=0; in<input.size(); in++) {
      for (auto &token: input[in]){
        std::cout<< token;
      }
      std::cout<<std::endl;
    }
    for(const auto& elem : st){
       std::cout << elem.first << " " << elem.second << "\n";
    }
    /*for (int in=0; in<input.size(); in++) {
      // This example code just prints the scanned tokens on each line.
      std::vector<Token> tokenLine = input[in];
      int size = tokenLine.size();

      pc+=4;
      // This code is just an example - you don't have to use a range-based
      // for loop in your actual assembler. You might find it easier to
      // use an index-based loop like this:
      for(int i=0; i<size; i++) {
        Token cur = tokenLine[i];
        std::string lex = cur.getLexeme();
        if (tokenLine[i].getKind() == Token::WORD){
          if (size-i != 2) {throw 1;}
          Token t = tokenLine[++i];
          int64_t num = t.toNumber();
          if (t.getKind()==Token::HEXINT){
            if(num > 0xffffffff) {throw 1;}
            else {unsigned char c=num;std::cout << c;}
          }
          else if (t.getKind()==Token::INT){
            if(num < -2147483648 || num > 4294967295) {throw 1;}
            else {unsigned char c=num;std::cout << c;}
          }
          else if (t.getKind()==Token::LABEL){
            //access the map
          }
          else {throw 1;}
        }
        else if (cur.getKind() == Token::ID){
          if (lex == "add" || lex == "sub" || lex == "slt" || lex == "sltu") {
            if (size-i != 6) {throw 1;}
            int op=0,s,t,d,func=0;
            if (lex == "add") {func = 32;}
            else if (lex == "sub") {func = 34;}
            else if (lex == "slt") {func = 42;}
            else if (lex == "sltu") {func = 43;}
            if (tokenLine[++i].getKind() != Token::REG) {throw 1;}
            int64_t num = tokenLine[i].toNumber();
            if (num < 0 || num > 31) {throw 1;}
            d = num;
            if (tokenLine[++i].getKind() != Token::COMMA) {throw 1;}
            if (tokenLine[++i].getKind() != Token::REG) {throw 1;}
            num = tokenLine[i].toNumber();
            if (num < 0 || num > 31) {throw 1;}
            s = num;
            if (tokenLine[++i].getKind() != Token::COMMA) {throw 1;}
            if (tokenLine[++i].getKind() != Token::REG) {throw 1;}
            num = tokenLine[i].toNumber();
            if (num < 0 || num > 31) {throw 1;}
            t = num;
            int instr = (op << 26) | (s << 21) | (t << 16) | (d << 11) | func;
            unsigned char c = instr;
            std::cout<< c;
          }
          else if (lex == "beq" || lex == "bne") {
            if (size-i != 6) {throw 1;}
            int op=0,s,t,offset;
            if (lex == "beq") {op = 4;}
            else if (lex == "bne") {op = 5;}
            if (tokenLine[++i].getKind() != Token::REG) {throw 1;}
            int64_t num = tokenLine[i].toNumber();
            if (num < 0 || num > 31) {throw 1;}
            s = num;
            if (tokenLine[++i].getKind() != Token::COMMA) {throw 1;}
            if (tokenLine[++i].getKind() != Token::REG) {throw 1;}
            num = tokenLine[i].toNumber();
            if (num < 0 || num > 31) {throw 1;}
            t = num;
            if (tokenLine[++i].getKind() != Token::COMMA) {throw 1;}
            if (tokenLine[++i].getKind() == Token::HEXINT) {
              num = tokenLine[i].toNumber();
              if (oir(num,1)) {offset = num;}
              else {throw 1;}
            }
            else if (tokenLine[i].getKind() == Token::INT) {
              num = tokenLine[i].toNumber();
              if (oir(num,0)) {offset = num;}
              else {throw 1;}
            }
            else if (tokenLine[i].getKind() == Token::LABEL) {
              num = 0;//access from map - loc/4
              if (oir(num,0)) {offset = num;}
              else {throw 1;}
            }
            else {throw 1;}
            int instr = (op << 26) | (s << 21) | (t << 16) | (offset & 0xFFFF);
            unsigned char c = instr;
            std::cout<< c;
          }
          else {throw 1;}
        }
      }*/
      /*for (auto &token: tokenLine){
        std::cout<< token;
      }
      std::cout<<std::endl;*/
    //}
  } catch (ScanningFailure &f) {
    std::cerr << f.what() << std::endl;
    return 1;
  } catch (int i){
    std::cerr << "ERROR" << std::endl;
    return 1;
  }
  // You can add your own catch clause(s) for other kinds of errors.
  // Throwing exceptions and catching them is the recommended way to
  // handle errors and terminate the program cleanly in C++. Do not
  // use the std::exit function, which may leak memory.
  
  return 0;
}

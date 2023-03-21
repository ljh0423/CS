/*
This code constructs the RegExp class with subclasses Disjunction, Concatenation, Star, Word, Begin, End and Dot.
I have implemented the function matches, which takes in a string and returns a boolean value indicating whether the string is an exact match of the regex expression.
There are also parsing functions which parse a regex expression along with print and << function that prints out the regex expression in readable format.
The containsMatch function return a boolean value of whether the string s contains a match to the regex expression
*/
#ifndef RE_H
#define RE_H
#include <iostream>
#include <string>
#include <memory>

namespace CS246E {
  class RegExp {
   public:
    bool matches(std::string s) {
      size_t i=0;
      bool b = matchp(s,nullptr,i);
      return (b && i==s.length());
    }
    bool matchp(std::string &s, std::unique_ptr<RegExp>&& re, size_t &i) {
      return match(s,std::move(re),i);
    }
    std::unique_ptr<RegExp> copyp() {
      return this->copy();
    }
    std::ostream &print(std::ostream &out) {
      printc(out);
      return out;
    }
    virtual ~RegExp() {}
   protected:
    virtual std::unique_ptr<RegExp> copy()=0;
    virtual bool match(std::string &s, std::unique_ptr<RegExp>&& re, size_t &i) = 0;
    virtual void printc(std::ostream &out)=0;
  };
  class Disjunction final : public RegExp {
    std::unique_ptr<RegExp> left,right;
   public:
    Disjunction(std::unique_ptr<RegExp>&& left, std::unique_ptr<RegExp>&& right): left{std::move(left)}, right{std::move(right)} {}
    std::unique_ptr<RegExp> copy() override {
      return std::make_unique<Disjunction> (left->copyp(),right->copyp());
    }
    bool match(std::string &s, std::unique_ptr<RegExp>&& re, size_t &i) override {
      size_t i0 = i,i1;
      bool b1,b2;
      if (re!=nullptr) b1 = left->matchp(s,re->copyp(),i);
      else b1 = left->matchp(s,nullptr,i);
      i1 = i;
      i = i0;
      if (re!=nullptr) b2 = right->matchp(s,re->copyp(),i);
      else b2 = right->matchp(s,nullptr,i);
      if (!b1 && !b2) i = i0;
      else if (b1) {
        if (b2) i = std::max(i1,i);
        else i = i1;
      }
      return (b1 || b2);
    }
    void printc(std::ostream &out) override {
      if (left!=nullptr) {
      out << "Disjunction (";
      left->print(out);
      out << ",";
      right->print(out);
      out << ")";
      }
    }
    ~Disjunction() {}
  };
  class Concatenation final : public RegExp {
    std::unique_ptr<RegExp> left,right;
   public:
    Concatenation(std::unique_ptr<RegExp>&& left, std::unique_ptr<RegExp>&& right): left{std::move(left)}, right{std::move(right)} {}
    std::unique_ptr<RegExp> copy() override {
      return std::make_unique<Concatenation> (left->copyp(),right->copyp());
    }
    bool match(std::string &s, std::unique_ptr<RegExp>&& re, size_t &i) override {
      size_t i0=i;
      bool b;
      if (re!=nullptr) {
        re = std::make_unique<Concatenation> (right->copyp(),re->copyp());
        b = left->matchp(s,re->copyp(),i);
      }
      else {
        b = left->matchp(s,right->copyp(),i);
      }
      if (!b) {i=i0;return 0;}
      return 1;
    }
    void printc(std::ostream &out) override {
      out << "Concatenation (";
      left->print(out);
      out << ",";
      right->print(out);
      out << ")";
    }
    ~Concatenation() {}
  };
  class Star final : public RegExp {
    std::unique_ptr<RegExp> exp;
   public:
    Star(std::unique_ptr<RegExp>&& exp): exp{std::move(exp)} {}
    std::unique_ptr<RegExp> copy() override {
      return std::make_unique<Star> (exp->copyp());
    }
    bool match(std::string &s, std::unique_ptr<RegExp>&& re, size_t &i) override {
      size_t i0=i,i1=i;
      if (re!=nullptr) {
        if (re->matchp(s,nullptr,i)) return 1;
        else {
          auto r = exp->copyp();
          while (r->matchp(s,nullptr,i) && i!=i0) {
          i0 = i;
            i = i1;
            if (r->matchp(s,re->copyp(),i)) {return 1;}
            r = std::make_unique<Concatenation> (exp->copyp(),std::move(r));
          }
          bool b=re->matchp(s,nullptr,i);
          if (!b) i=i1;
          return b;
        }
      }
      else {
        while ((exp->matchp(s,nullptr,i)) && i0!=i && i!=s.length()) {i0 = i;}
      }
      return 1;
    }
    void printc(std::ostream &out) override {
      out << "Star (";
      exp->print(out);
      out << ")";
    }
    ~Star() {}
  };
  class Word final : public RegExp {
    std::string w;
   public:
    Word(std::string w): w{w} {}
    std::unique_ptr<RegExp> copy() override {
      return std::make_unique<Word> (w);
    }
    bool match(std::string &s, std::unique_ptr<RegExp>&& re, size_t &i) override {
      size_t i0=i;
      if (s.find(w,i) == i) {
        i=i+w.length();
        bool b=1;
        if (re!=nullptr) {b = re->matchp(s,nullptr,i);}
        if (!b) {i=i0;return 0;}
        return 1;
      }
      else return 0;
    }
    void printc(std::ostream &out) override {
      out << w;
    }
    ~Word() {}
  };
  class Begin : public RegExp {
   public:
    Begin() {}
    std::unique_ptr<RegExp> copy() override {
      return std::make_unique<Begin> ();
    }
    bool match(std::string &s, std::unique_ptr<RegExp>&& re, size_t &i) override {
      size_t i0=i;
      if (s.find('\200') == i) {
        ++i;
        bool b=1;
        if (re!=nullptr) {b = re->matchp(s,nullptr,i);}
        if (!b) {i=i0;return 0;}
        return 1;
      }
      else return 0;
    }
    void printc(std::ostream &out) override {
      out << '^';
    }
  };
  class End : public RegExp {
   public:
    End() {}
    std::unique_ptr<RegExp> copy() override {
      return std::make_unique<End> ();
    }
    bool match(std::string &s, std::unique_ptr<RegExp>&& re, size_t &i) override {
      size_t i0=i;
      if (s.find('\201') == i) {
        ++i;
        bool b=1;
        if (re!=nullptr) {b = re->matchp(s,nullptr,i);}
        if (!b) {i=i0;return 0;}
        return 1;
      }
      else return 0;
    }
    void printc(std::ostream &out) override {
      out << '$';
    }
  };
  class Dot : public RegExp {
   public:
    Dot() {}
    std::unique_ptr<RegExp> copy() override {
      return std::make_unique<Dot> ();
    }
    bool match(std::string &s, std::unique_ptr<RegExp>&& re, size_t &i) override {
      size_t i0=i;
      if (!(s.substr(i)).empty()) {
        if (s[i]=='\200' || s[i]=='\201') return 0;
        ++i;
        bool b=1;
        if (re!=nullptr) {b = re->matchp(s,nullptr,i);}
        if (!b) {i=i0;return 0;}
        return 1;
      }
      else return 0;
    }
    void printc(std::ostream &out) override {
      out << '.';
    }
  };
  std::unique_ptr<RegExp> parseDisjunction(const std::string &s);
  std::unique_ptr<RegExp> parseConcatenation(const std::string &s);
  std::unique_ptr<RegExp> parseWord(const std::string &s);

  std::ostream &operator<<(std::ostream &out, RegExp &re);
}

bool containsMatch(CS246E::RegExp *re, const std::string &s);

#endif

#include <iostream>
#include <sstream>
#include <vector>
#include <forward_list>
#include <cmath>

int find_first_bigger_prime(int start) {
// pre: start > 10
// post: returns smallest prime that is at least start
	if (start%2 == 0) { start++; }  // no need to test even numbers
	for (int i=start; i<=(2*start); i+=2) {
//By Bertrand's postulate, a prime has to exist in (start, 2*start) 
		bool isPrime = true;
		for (int j=3; j*j <= i; j+=2) {
			if (i%j == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime==true) {
			return i;
		}
	}
}

int hash0(long key, int tableSize) {
	return (int) (key%tableSize);
}

int hash1(long key, int tableSize) {
	double phi = (sqrt(5)-1)/2;
	double val = key * phi;
	return (int) floor(tableSize*(val - floor(val)));
}
//translate phone number string input to long
long numtol(std::string s){
  s = s.substr(1,3) + s.substr(5,3) + s.substr(9,4);
  return stol(s);
}
//translate name string input to key using flattening technique and Horner’s Rule 
long namtol(std::string s, int m){
  long key=s[0];
  for(int i=1;i<s.size();i++){
    key = (key*255 + s[i]) % m;
  }
  return key;
}
//insert function for D_name
void ninsert(std::vector<std::forward_list<std::pair<std::string,std::string>>>& ht,std::string name,std::string num, int m){
  long key = namtol(name,m);  //translate the name and number input to long
  long val = numtol(num);
  std::forward_list<std::pair<std::string,std::string>>& cur = ht[key]; //fetch correspondin foward_list in hashtable for key
  if(cur.empty()){   //if the chain is empty insert the new pair input
    cur.push_front(std::make_pair(name,num));
    return;
  }
  auto it = cur.before_begin();
  auto nit = cur.begin();
  for(;nit!=cur.end();it++){ //iterate elements in forwards list from the start
    if(val < numtol((*nit).second)) { //if the new value is less than the number in current iterator, insert the new pair before it
      cur.insert_after(it,std::make_pair(name,num));
      return;
    }
    nit++;
  }
  cur.insert_after(it,std::make_pair(name,num)); //if the new value is not less than all number values in the list, insert it at the end of list
}
//insert function for D_phone
void pinsert(std::vector<std::vector<std::pair<std::string,std::string>>>& hts,std::string name,std::string num,int m,int htnum){
  int key;   //calculate the correspondin key using htnum
  if(htnum==0) { key = hash0(numtol(num),m); }
  else if(htnum==1) { key = hash1(numtol(num),m); }
  if((hts[htnum][key]).first.empty()){   //if the slot in the hashtable is empty, then insert it into the slot
    hts[htnum][key] = std::make_pair(name,num);
    return;
  }
  else { //if the slot is not empty, insert it to the slot and run insert on the other hashtable with the original pair in the slot
    std::pair<std::string,std::string> temp = hts[htnum][key];
    hts[htnum][key] = std::make_pair(name,num);
    pinsert(hts,temp.first,temp.second,m,(1-htnum));
  }
}

int main() {
  std::vector<std::forward_list<std::pair<std::string,std::string>>> dname;
  std::vector<std::vector<std::pair<std::string,std::string>>> dps;
  int m1=11,m2=11;
  
	std::string line;
	while (getline(std::cin, line)) {
		std::stringstream conv(line);
		std::string s;
    conv >> s;
    if(s.compare("i")==0){ //run both insert functions
      std::string name, num;
      conv >> name >> num;
      ninsert(dname,name,num,m1);
      pinsert(dps,name,num,m2,0);
    }
    else if(s.compare("l")==0){ //use D_phone to check 1 slot in each hash table with the corresponding key
      std::string num;
      conv >> num;
      int key = hash0(numtol(num),m2);
      if((dps[0][key].second).compare(num)==0){
        std::cout<<dps[0][key].first<<std::endl;
      }
      else {
        key = hash1(numtol(num),m2);
        if((dps[1][key].second).compare(num)==0){
          std::cout<<dps[1][key].first<<std::endl;
        }
        else{
          std::cout<<"not found"<<std::endl; //print "not found" if not at both slots
        }
      }
    }
    else if(s.compare("s")==0){ //use D_name to get the corresponding forward list with key=namtol(name,m1), and search the list for same name (print if match)
      bool found=0,first=1;
      std::string name;
      conv >> name;
      std::forward_list<std::pair<std::string,std::string>> fl = dname[namtol(name,m1)];
      for(auto it=fl.begin();it!=fl.end();++it){
        if(name.compare((*it).first)==0) {
          found = 1;
          if(!first) { std::cout<<" "; }
          first=0;
          std::cout<<(*it).second;
        }
      }
      if(found){ std::cout<<std::endl; }
      else { std::cout<<"not found"<<std::endl; }
    }
    else if(s.compare("rh")==0){ //rehash
      int a;
      conv >> a;
      if(a==0){ //rehash D_name by inserting all current pairs to the new increased hash table
        std::vector<std::forward_list<std::pair<std::string,std::string>>> og = dname;
        m1 = find_first_bigger_prime(2*m1+1); //next size of hash table
        dname.clear();
        for(int i=0;i<m1;i++) { //initialize to empty lists
          std::forward_list<std::pair<std::string,std::string>> nf;
          dname.push_back(nf);
        }
        for(int i=0;i<og.size();i++){ //insert all pairs currently stored
          for(auto it=og[i].begin();it!=og[i].end();++it){
            std::pair<std::string,std::string> cp = (*it);
            ninsert(dname,cp.first,cp.second,m1);
          }
        }
      }
      else if(a==1){  //rehash D_phone by inserting all current pairs to the new hash table in order, starting at index 0 of first table followed by the second table
        std::vector<std::vector<std::pair<std::string,std::string>>> og = dps;
        m2 = find_first_bigger_prime(2*m2+1);
        dps[0].clear();
        dps[1].clear();
        for(int i=0;i<m2;i++) { //initialize to empty slots
          std::pair<std::string,std::string> np;
          dps[0].push_back(np);
          dps[1].push_back(np);
        }
        for(int i=0;i<og.size();i++){ //insert all current pairs in order
          for(int k=0;k<og[i].size();k++){
            std::pair<std::string,std::string> cp = og[i][k];
            if(!cp.first.empty()){
              pinsert(dps,cp.first,cp.second,m2,0);
            }
          }
        }
      }
    }
    else if(s.compare("p")==0){ //print
      int a;
      conv >> a;
      if(a==0){
        std::cout<<m1;
        for(int i=0;i<dname.size();i++){ //print out the length of each list in order
          std::cout<<" "<<distance(dname[i].begin(),dname[i].end());
        }
        std::cout<<std::endl;
      }
      else if(a==1){
        std::cout<<m2;
        for(int i=0;i<dps.size();i++){ //iterate each table and print 0 if empty 1 otherwise
          for(int k=0;k<dps[i].size();k++){
            if((dps[i][k]).first.empty()){
              std::cout<<" 0";
            }
            else {
              std::cout<<" 1";
            }
          }
        }
        std::cout<<std::endl;
      }
    }
    else if(s.compare("r")==0){ //reset
      dname.clear();  //clear current table and insert 11 empty lists
      for(int i=0;i<11;i++) { 
        std::forward_list<std::pair<std::string,std::string>> nf;
        dname.push_back(nf);
      }
      m1=11;
      dps.clear();  //clear current two tables and insert two new tables of size 11
      std::vector<std::pair<std::string,std::string>> nt;
      for(int i=0;i<11;i++) { 
        std::pair<std::string,std::string> np;
        nt.push_back(np);
      }
      dps.push_back(nt);
      dps.push_back(nt);
      m2=11;
    }
    else if(s.compare("x")==0){ return 0; } //exit
	}
}


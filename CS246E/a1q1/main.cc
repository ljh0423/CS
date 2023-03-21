#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void wc(istream &f, int &t1, int &t2, int &t3, int flag) {
	char k, m = ' ';
	f >> noskipws;
	int c = 0, l = 0, w = 0;
	while (f >> k) {
		c++;
		if (isspace(k)) {
			if (!isspace(m))
				w++;
			if (k == '\n')
				l++;
		}
		m = k;
	}
	t1 += c;
	t2 += w;
	t3 += l;
	if (flag >= 100)
		cout << l << " ";
	if (flag % 100 >= 10)
		cout << w << " ";
	if (flag % 10 >= 1)
		cout << c << " ";
	if (flag == 0)
		cout << l << " " << w << " " << c << " ";
}

int main(int argc, char *argv[]) {
	int tc = 0, tw = 0, tl = 0, fl = 0, fc = 0;
	for (int i = 1; i < argc; ++i) {
		if (string(argv[i]) == "-l") {
			fl += 100;
			fc++;
		}
    else if (string(argv[i]) == "-w") {
			fl += 10;
			fc++;
		}
    else if (string(argv[i]) == "-c") {
			fl += 1;
			fc++;
		}
    else {
			ifstream f{argv[i]};
			wc(f, tc, tw, tl, fl);
			cout << argv[i] << endl;
		}
	}
  if (argc - fc == 1) wc(cin, tc, tw, tl, fl);
	if (argc - fc >= 3) {
		if (fl >= 100)
			cout << tl << " ";
		if (fl % 100 >= 10)
			cout << tw << " ";
		if (fl % 10 >= 1)
			cout << tc << " ";
		if (fl == 0)
			cout << tl << " " << tw << " " << tc << " ";
		cout << "total" << endl;
	}
}

// ceng580hw3.cpp : Defines the entry point for the application.
//

#include "ceng580hw3.h"
#include <iostream>
#include "Map.h"
#include <fstream>
#include "Hunter.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
	int n, d, E;
	float epsilon,rd,ro,rc,rg;	fstream f;
	f.open(string("env.inp"));
	f >> n >> d >> E >> epsilon;

	f >> rd >> ro >> rc >> rg;
	Map m(n, d, rd, ro, rc, rg, epsilon, E);
	Hunter::lr = 0.1;
	Hunter::df = 0.9;
	Hunter::rd = rd;
	int x, y;
	char t;
	while (f >> t >> x >> y	) {
		m.add_entity(x, y, t);
	}
	m.run();
	return 0;
}

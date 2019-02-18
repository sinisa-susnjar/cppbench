#include <iostream>
#include <iomanip>
#include <cstring>
#include <cerrno>

#include "cppbench.h"

using namespace std;

int main()
{
	string val = "3.141592653589793238462643383279502884";
	int count = 1000000;

	// determine the runtimes of various string to double conversions
	auto rt = cppbench::time(count, {
		 { "atof",		[val]() { double d = atof(val.data()); }				}
		,{ "strtod",	[val]() { double d = strtod(val.data(), nullptr); }	}
		,{ "sscanf",	[val]() { double d; sscanf(val.data(), "%lf", &d); }	}
		,{ "stod",		[val]() { double d = stod(val, nullptr); }			}
	});

	// compare the runtimes and create relative speed matrix
	auto comp = cppbench::compare(rt);

	// print runtimes and comparison matrix to console
	cppbench::print(comp);

	cout << endl << "runtimes in milliseconds:" << endl;

	// default is to print runtimes in microseconds, use milliseconds instead
	cppbench::print<chrono::milliseconds>(comp);

	cout << endl << "runtimes with min, max, mean, variance and standard deviation" << endl;
	cout << endl << "nanoseconds" << endl;
	cppbench::print<chrono::nanoseconds>(rt);
	cout << endl << "microseconds" << endl;
	cppbench::print<chrono::microseconds>(rt, 3);
	cout << endl << "milliseconds" << endl;
	cppbench::print<chrono::milliseconds>(rt, 5);

	if (cppbench::write("sample", rt))
		cout << endl << "successfully wrote runtimes to sample-*.txt files" << endl << endl;
	else
		cerr << endl << "there were problems: " << strerror(errno) << endl << endl;
}

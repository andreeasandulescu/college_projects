#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <algorithm>
using namespace std;

#define SOBEL 1				//filter tag
#define MEAN_REMOVAL 2
#define FINAL 3

class Image_info{
public:
	char filter;
	char *img_in, *img_out;
	Image_info();
	Image_info(char, char*, char*);
	static vector <Image_info> read_imagini_in(char *);
};

class File{
public:
	char *img_in, *img_out, filter;
	vector <char> to_parse;
	vector <char> header;
	vector <char> special_ws;
	vector <vector<char> > pixels;
	int width, height, max_val, header_index;
	void read_file();
	void parse_file();
	bool parse_comm(int*);
	int parse_comments(int);
	int parse_ws(int);
	int parse_and_return_ws(int);
	int get_int(int*);
};

class Processed_lines{
public:
	int rank;
	int nr_lines;
	Processed_lines();
	Processed_lines(int, int);
};
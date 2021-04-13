#include "parse.h"

Image_info::Image_info()
{

}

Image_info::Image_info(char f, char*in, char*out)
{
	filter = f;
	img_in = (char*) calloc(strlen(in), sizeof(char));
	img_out = (char *) calloc(strlen(out), sizeof(char));

	strcpy(img_in, in);
	strcpy(img_out, out);
}

vector <Image_info> Image_info::read_imagini_in(char *name)
{
	int i, nr_lines;
	char str[70], *p, *in, *out, filter;
	FILE *f;
	vector <Image_info> images;

	f = fopen(name, "r");
	if (f == NULL) {
	       perror("Error");
	}
	fgets( str, 10, f);				//citire numar linii
    p = strchr(str, '\n');
    if(p != NULL)
		*p = '\0';
	else
		printf("Err\n");

	nr_lines = atoi(str);
	images.reserve(nr_lines);

	for(i = 0;i < nr_lines; i++)
	{
		fgets(str, 70, f);
		p = strtok(str, " ");
	   	if(strcmp(p, "mean_removal") == 0)
			filter =  MEAN_REMOVAL;
		else if(strcmp(p, "sobel") == 0)
	    	filter = SOBEL;

	    in = strtok(NULL, " ");
		out = strtok(NULL, " ");
		p = strchr(out, '\n');
		if(p != NULL)
			*p = '\0';
		else
			printf("Err\n");
		images.push_back(Image_info(filter, in, out));
	}
	fclose(f);
	return images;
}


void File::read_file()
{
	int c;
	FILE *pgm_in;
	pgm_in = fopen(this->img_in, "r");
	if (pgm_in == NULL) {
       perror("Error");
    }
    to_parse.clear();
	while ((c = fgetc(pgm_in)) != EOF) {
		this->to_parse.push_back(c);
 	}
	fclose(pgm_in);
}


void File::parse_file()
{
	int index = 0, aux = 0, i = 0, j = 0, val_int;
	char str[10];

	this->parse_comments(index);												//1.magic number
	if(this->to_parse[index] == 'P' && isdigit(this->to_parse[index + 1])!= 0)	
	{
		index = index + 2;
		index = this->parse_ws(index);
	}
	else
	{
		printf("errorrrr!\n");
		return;
	}
	index = this->parse_comments(index);

	this->width = this->get_int(&index);											//3.width
	index = this->parse_ws(index);
	index = this->parse_comments(index);

	this->height = this->get_int(&index);											//5.height
	index = this->parse_ws(index);
	index = this->parse_comments(index);

	this->max_val = this->get_int(&index);											//7.maxval
	index = this->parse_ws(index);
	index = this->parse_comments(index);
	this->header_index = index;

	header.clear();
	header.reserve(header_index + 1);
	pixels.clear();
	pixels.resize(this->height + 2);
	for(aux = 0; aux < this->height; aux++)
	{
		pixels[aux] = vector <char> ();
		pixels[aux].reserve(this->width + 2);
	}

	for(aux = 0;aux < header_index;aux++)
	{
		header.push_back(this->to_parse[aux]);
	}
	for(j = 0; j < this->width + 2; j++)					//bordarea primei linii cu 0
	{
		pixels[0].push_back(0);
	}
	for(i = 1; i < this->height + 1; i++)					//bordarea primei coloane cu 0
	{
		pixels[i].push_back(0);
	}
	for(i = 1 ; i < this->height + 1; i++)
		for(j = 1 ; j < this->width + 1; j++)
		{
			val_int = this->get_int(&index);
			if(i == 1 && j == 1)
				index = this->parse_and_return_ws(index);			//pastrez doar primele whitespace-uri dintre pixeli, din moment ce whitespace-urile
			else													//dintre toti pixelii vor fi aceleasi
				index = this->parse_ws(index);
			pixels[i].push_back( (char) val_int);
		}
	aux = this->height + 1; 
	for(j = 0; j < this->width + 2; j++)					//bordarea ultimei linii cu 0
	{
		pixels[aux].push_back(0);
	}
	for(i = 1; i < this->height + 1; i++)					//bordarea ultimei coloane cu 0
	{
		pixels[i].push_back( 0);
	}
}

bool File::parse_comm(int* index)
{
	int i = *index;
	if(this->to_parse[i] == '#')
	{
		for(i = i+ 1;i < this->to_parse.size();i++)
		{
			if(this->to_parse[i] == '\n')
			{
				*index = i;
				return true;
			}
		}
	}
	return false;
}

int File::parse_comments(int index)
{
	int aux = index;
	while(this->parse_comm(&aux))
		aux = this->parse_ws(aux + 1);	
	return aux;
}

int File::parse_ws(int index)
{
	int i;
	for(i = index;i < this->to_parse.size();i++)
	{
		if(isspace(this->to_parse[i]) == 0)
			return i;
		if(i + 1 == this->to_parse.size())						//cazul in care ultimul caracter din fisier este whitespace
			return i+1;
	}
}

int File::parse_and_return_ws(int index)
{
	int i;
	this->special_ws.clear();
	for(i = index;i < this->to_parse.size();i++)
	{
		if(isspace(this->to_parse[i]) == 0)
			return i;
		if(i + 1 == this->to_parse.size())						//cazul in care ultimul caracter din fisier este whitespace
			return i+1;
		this->special_ws.push_back(this->to_parse[i]);
	}
}

int File::get_int(int* index)
{
	char str[10];
	int i, aux = 0;

	for(i = *index;i < this->to_parse.size();i++)
	{
		if(isdigit(this->to_parse[i]))
		{
			str[aux] = this->to_parse[i];
			aux++;
		}
		else
			break;
	}
	*index = *index + aux;
	str[aux] = '\0';

	if(aux != 0)
		return atoi(str);
	else
		return -1;
}

Processed_lines::Processed_lines()
{

}

Processed_lines::Processed_lines(int r, int nr)
{
	rank = r;
	nr_lines = nr;
}
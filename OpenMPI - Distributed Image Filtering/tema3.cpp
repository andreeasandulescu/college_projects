#include "parse.h"

vector <int> send_to_children(vector <int> neighb, vector <vector<char> > & matrix, int start, int tag)
{
	int i, j, aux, first, last, recv_lines = 0, nr_lines;						//recv_lines este numarul de linii primite de fiecare copil
	char *p;
	vector <int> lines_per_child = vector <int> ();
	lines_per_child.reserve(neighb.size());

	first = start;
	aux = matrix[0].size();
	nr_lines = matrix.size() - 2;

	for(i = 0; i < neighb.size();i++ )
	{
		if(nr_lines < neighb.size())
		{
			if(i < nr_lines)
				recv_lines = 3;
			else
				recv_lines = 0;
		}
		else
		{
			if(i != neighb.size() - 1)
			{
				recv_lines = nr_lines / neighb.size() + 2;
			}
			else
			{
				j = nr_lines / neighb.size();
				recv_lines = nr_lines - ((neighb.size() - 1) * j) + 2;
			}
		}
		
		lines_per_child.push_back(recv_lines);
		MPI_Ssend( &recv_lines, 1, MPI_INT, neighb[i], tag, MPI_COMM_WORLD);			//numar linii
		MPI_Ssend( &aux, 1, MPI_INT, neighb[i], tag, MPI_COMM_WORLD);					//dimensiunea unei linii

		if(i != 0)
			first = last - 2;
		last = first + recv_lines;

		for(j = first ; j < last; j++)
		{
			p = matrix[j].data();
			MPI_Ssend( p, aux, MPI_CHAR, neighb[i], tag, MPI_COMM_WORLD);
		}
	}
	return lines_per_child;
}

void send_to_parent(vector <vector<char> > & matrix, int parent, int tag)
{
	int i;
	char *p;

	for(i = 0; i < matrix.size(); i++ )
	{
			p = matrix[i].data();
			MPI_Ssend( p, matrix[0].size(), MPI_CHAR, parent, tag, MPI_COMM_WORLD);
	}
	return;
}

vector <vector<char> > recv_from_parent(int* tag, int* parent)
{
	int i = 0, aux = 0;
	int aux_nr_lines = 0, aux_width;
	vector <vector<char> > lines;
	MPI_Status status;

	MPI_Recv( &aux_nr_lines, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	*tag = status.MPI_TAG;
	if(*tag == FINAL)
		return vector <vector <char> > ();

	MPI_Recv( &aux_width, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	*parent = status.MPI_SOURCE;

	lines = vector < vector<char> > (aux_nr_lines);
	for(i = 0; i < aux_nr_lines; i++)

		lines[i] = vector <char> (aux_width);
	for(i = 0 ; i < aux_nr_lines; i++)
		MPI_Recv( lines[i].data(), aux_width, MPI_CHAR, *parent, *tag, MPI_COMM_WORLD, &status);
	return lines;
}

vector <vector<char> > recv_from_children(vector <int> neighb, int nr_lines, int width, vector <int> lines_per_child, int tag)
{
	int i, j, index = 0;
	MPI_Status status;
	vector <vector<char> > lines_from_children = vector <vector <char> > (nr_lines);

    for(i = 0 ; i < lines_from_children.size(); i++)
		lines_from_children[i] = vector <char> (width);

	for(i = 0 ; i < neighb.size(); i++)
	{
		for(j = 0;j < lines_per_child[i] - 2; j++)										//frunzele vor sterge cele doua linii auxiliare primite
		{
			MPI_Recv(lines_from_children[index].data(), width, MPI_CHAR, neighb[i], tag, MPI_COMM_WORLD, &status);
			index++;
		}
	}
	return lines_from_children;
}

int matrix_conv(char filter[3][3], vector <vector<char> > &matrix, int index_row, int index_col)
{
	int i, j, aux_row, aux_col, sum = 0;
	for(i = index_row ; i < index_row + 3 ; i++)
	{
		aux_row = i - index_row;
		for(j = index_col ; j < index_col + 3 ; j++)
		{
			aux_col = j - index_col;
			sum += ( (unsigned char) matrix[i][j]) * filter[aux_row][aux_col];
		}
	}
	return sum;
}

void apply_filter(vector <vector<char> > &matrix, char filter[3][3], int div, int to_add)
{
	int i, j, m, n, sum;

	vector <vector<char> > aux_matrix = vector <vector<char> > (matrix.size());
	for(i = 0 ; i < matrix.size(); i++)
		aux_matrix[i] = vector<char> (matrix[i]);

	for(i = 0 ; i < matrix.size()-2; i++)
	{
		for(j = 0 ; j < matrix[i].size()-2; j++)
		{
			sum = 0;
			for(m = 0; m < 3 ; m++)
			{
				for(n = 0; n < 3 ; n++)
					sum += ( (unsigned char) aux_matrix[m + i][n + j]) * filter[m][n];
			}
			sum = sum / div + to_add;
			if(sum < 0)
				matrix[i+1][j+1] = 0;			
			else if(sum > 255)
				matrix[i+1][j+1] = 255;
			else
				matrix[i+1][j+1] = sum;
		}
	}

}


int main( int argc, char **argv )
{
	int rank, size, i, j = 0,index;
	int tag = 0, parent = 0, aux, lines_processed, nr_elem, *nodes;
	char *p, *ptr, *line, *pixels_ws;
	char str[20], c;
	vector <vector<char> > lines_from_parent, lines_from_children;
	vector <int> lines_per_child;
	vector <int> neighb = vector <int> ();
	vector <Image_info> images;
	vector <Processed_lines> stat = vector <Processed_lines> ();					//statistica
	vector <Processed_lines> aux_stat = vector <Processed_lines> ();					//statistica
	FILE *pgm_out, *topologie_in, *statistica_out;
	File f =  File();
	MPI_Status status;

	char sobel[3][3] = {{1,0,-1},{2,0,-2},{1,0,-1}};
	char mean_removal[3][3] = {{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};

	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );

	line = (char *)calloc(2000, sizeof(char));

	topologie_in = fopen(argv[1], "r");
	if (topologie_in == NULL) {
       perror("Error");
    }
    for(i = 0;i < rank + 1;i++)											//fiecare proces MPI isi citeste lista de adiacenta din fisier
    	fgets(line, 2000, topologie_in);
	p = strtok(line, " \n");
	p = strtok(NULL, " \n");
	while(p!=NULL)
	{
		neighb.push_back(atoi(p));
		p = strtok(NULL, " \n");
	}
    fclose(topologie_in);

    lines_processed = 0;
    index = 0;

	if(rank == 0 )																		//radacina
	{
		nodes = (int*) calloc(size, sizeof(int));
		images = Image_info::read_imagini_in( (char*)argv[2]);

		for(index = 0; index < images.size(); index++)
		{
			f.img_in = images[index].img_in;
			f.img_out = images[index].img_out;
			f.filter = images[index].filter;

			if(index > 0)
			{
				if(strcmp(images[index-1].img_in,images[index].img_in) != 0)		//daca trebuie aplicat filtru pe aceeasi imagine pgm pe care
				{																	//am citit-o la pasul anterior, nu mai citesc inca o data fisierul
					f.read_file();
					f.parse_file();
				}
			}
			else if(index == 0)
			{
				f.read_file();														//citirea fisierului pgm de intrare
				f.parse_file();														//parsarea fisierului pgm de intrare
			}

			pixels_ws = (char *)calloc(f.special_ws.size(), sizeof(char));			//in special_ws pastrez whitespace-urile dintre pixeli
			for(i = 0 ; i < f.special_ws.size(); i++)
				pixels_ws[i] = f.special_ws[i];

	    	tag = f.filter;
	    	lines_per_child = send_to_children(neighb, f.pixels, 0, tag);
	    	lines_from_children = recv_from_children(neighb, f.height, f.width + 2, lines_per_child, tag);

			for(i = 0; i < lines_from_children.size(); i++)
			{
				j = lines_from_children[i].size() - 1;
				lines_from_children[i].erase(lines_from_children[i].begin() + j);	//sterg prima si ultima coloana de 0 (din matricea bordata)
				lines_from_children[i].erase(lines_from_children[i].begin());
			}

			pgm_out = fopen(f.img_out, "w");
			if (pgm_out == NULL) {
		       perror("Error");
		    }
			for(i = 0 ;i < f.header_index;i++)
				fputc(f.header[i], pgm_out);
			for(i = 0; i < lines_from_children.size(); i++)
			{
				for(j = 0; j < lines_from_children[i].size(); j++)
				{
					aux = (unsigned char) lines_from_children[i][j];
					sprintf(str,"%d", aux);
					fputs(str, pgm_out );
					fputs(pixels_ws, pgm_out );
				}
			}
			fclose(pgm_out);
		}
		
		tag = FINAL;
		for(i = 0; i < neighb.size();i++ )
			MPI_Ssend( &aux, 1, MPI_INT, neighb[i], tag, MPI_COMM_WORLD);						//trimit mesajul cu tag-ul de terminare fiilor
		for(i = 0; i < neighb.size();i++ )
		{
			MPI_Recv(&nr_elem, 1, MPI_INT, neighb[i], tag, MPI_COMM_WORLD, &status);			//numarul de elemente din vectorul de structuri
			aux_stat.resize(nr_elem);
			MPI_Recv(aux_stat.data(), nr_elem * sizeof(Processed_lines), MPI_CHAR, neighb[i], tag, MPI_COMM_WORLD, &status);
			aux += nr_elem;
			stat.reserve(aux);
			stat.insert(stat.end(), aux_stat.begin(), aux_stat.end());
		}
		for(i=0 ; i < stat.size(); i++)
			nodes[stat[i].rank] = stat[i].nr_lines;

		statistica_out = fopen(argv[3], "w");
		if (statistica_out == NULL) {
	       perror("Error");
	    }
		for(i=0 ; i < size - 1; i++)
		{
			sprintf(str,"%d: %d\n", i, nodes[i]);
			fputs(str, statistica_out);
		}
		sprintf(str,"%d: %d\n", i, nodes[i]);
		fputs(str, statistica_out);
		fclose(statistica_out);
    }
	else if(neighb.size() != 1)												//noduri intermediare
	{	
		while(1)
		{
			lines_from_parent = recv_from_parent(&tag, &parent);

			if(tag == FINAL)
				break;
			neighb.erase(remove(neighb.begin(), neighb.end(), parent), neighb.end());			//sterge din lista de fii parintele

	    	lines_per_child = send_to_children(neighb, lines_from_parent, 0, tag);
	  		lines_from_children =recv_from_children(neighb, lines_from_parent.size() - 2, lines_from_parent[0].size(), lines_per_child, tag);

			send_to_parent(lines_from_children, parent, tag);
		}

		tag = FINAL;
		aux = 0;
		for(i = 0; i < neighb.size();i++ )
			MPI_Ssend( &aux, 1, MPI_INT, neighb[i], tag, MPI_COMM_WORLD);
		for(i = 0; i < neighb.size();i++ )
		{
			MPI_Recv(&nr_elem, 1, MPI_INT, neighb[i], tag, MPI_COMM_WORLD, &status);
			aux_stat.resize(nr_elem);
			MPI_Recv(aux_stat.data(), nr_elem * sizeof(Processed_lines), MPI_CHAR, neighb[i], tag, MPI_COMM_WORLD, &status);
			aux += nr_elem;
			stat.reserve(aux);
			stat.insert(stat.end(), aux_stat.begin(), aux_stat.end());						//concateneaza vectorii primiti de la fii	
		}
		MPI_Ssend( &aux, 1, MPI_INT, parent, tag, MPI_COMM_WORLD);
		MPI_Ssend( stat.data(), aux * sizeof(Processed_lines), MPI_CHAR, parent, tag, MPI_COMM_WORLD);
	}
	else																			//frunze
	{
		while(1)
		{
			lines_from_parent = recv_from_parent(&tag, &parent);
			if(tag == FINAL)
					break;
			lines_processed += lines_from_parent.size() - 2;

			if(tag == SOBEL)
				apply_filter(lines_from_parent, sobel,1,127);
			if(tag == MEAN_REMOVAL)
				apply_filter(lines_from_parent, mean_removal,1,0);

			lines_from_parent.erase(lines_from_parent.begin() + lines_from_parent.size() - 1);
			lines_from_parent.erase(lines_from_parent.begin());
			
			send_to_parent(lines_from_parent, parent,tag);
		}
		tag = FINAL;
		aux = 1;
		stat.push_back(Processed_lines(rank,lines_processed));
		MPI_Ssend( &aux, 1, MPI_INT, parent, tag, MPI_COMM_WORLD);
		MPI_Ssend( stat.data(), sizeof(Processed_lines), MPI_CHAR, parent, tag, MPI_COMM_WORLD);
		}
	   

	MPI_Finalize();
	return 0;
}
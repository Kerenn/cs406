#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <chrono>  // for high_resolution_clock

//DON'T CHANGE THIS FUNCTION
void get_constraints(std::vector<std::pair<std::pair<int,int>,std::pair<int,int> > > &constraints, std::ifstream &file){

  int ctr = 1;
  std::string constraint;
  while(!file.eof()){
    std::getline(file, constraint);
    if(constraint != ""){
      std::cout << "Constraint " << ctr++ << ": " << constraint << std::endl;
      std::stringstream ss(constraint);
      int val1, val2, val3, val4; // Coordinate (val1, val2) > (val3, val4)
      ss >> val1 >> val2 >> val3 >> val4;
      constraints.push_back(std::pair<std::pair<int,int>, std::pair<int,int> >(std::pair<int,int>(val1-1,val2-1),std::pair<int,int>(val3-1,val4-1)));
    }
  }
}


//DON'T CHANGE THIS FUNCTION
void read_matrix(int** &matrix, std::ifstream &file, int size){
  matrix = new int*[size];

  for(int i = 0; i < size; i++){
    matrix[i] = new int[size];
  }

  int val;
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      file >> val;
      matrix[i][j] = val;
    }
  }
}

//USE THIS FUNCTION WHILE CHECKING FINAL RESULT
//YOU MAY FURTHER OPTIMIZE THIS FUNCTION TO USE FOR CALCULATION
bool solved(int** &matrix, std::vector<std::pair<std::pair<int, int>,std::pair<int,int> > > constraints, int size){
  for(int i = 0; i < constraints.size(); i++){
    if(matrix[constraints[i].first.first][constraints[i].first.second] < matrix[constraints[i].second.first][constraints[i].second.second])
      return false;
  }


  std::vector<int> rows;
  std::vector<int> cols;
  for(int rc = 0; rc < size; rc++){
    for(int s = 0; s < size; s++){
      rows.push_back(matrix[rc][s]);
      cols.push_back(matrix[s][rc]);
    }

    std::sort(rows.begin(), rows.end());
    std::sort(cols.begin(), cols.end());

    if((rows[0] == -1) || (cols[0] == -1))
      return false;

    for(int i = 0; i < size-1; i++){
      if((rows[i] == rows[i+1]) || (cols[i] == cols[i+1])){
return false;
      }
    }

    rows.clear();
    cols.clear();
  }

  return true;

}


void print(int** &matrix,int size)
{
for(int i = 0 ; i < size; i++){
    for(int j = 0; j < size; j++){
      if(matrix[i][j] < 0)
	std::cout<<-matrix[i][j]<<" ";
	else
	std::cout<<matrix[i][j]<< " ";
    }
    std::cout << std::endl;
  }

}
int abs(int x)
{
	return (x<0)? -x : x;
}
void solve(int ** & matrix, std::vector < std::pair < std::pair < int, int > , std::pair < int, int > > > constraints, int size) {
  int si = size * size;

  for (int s = 0; s < si; ++s) {
    int i = s / size;
    int j = s % size;

    if (matrix[i][j] < 0)
      matrix[i][j] = 0;

  }

  int s = 0;
bool lr = 0; // 0 for next 1 for prev
  while (s != si) {
    int i = s / size;
    int j = s % size;


    if (matrix[i][j] < 1) {

      --matrix[i][j];

      if (matrix[i][j] == -(size + 1)) {
        matrix[i][j] = 0;

        --s;	// başka ihtimal yok
	lr=1;
      } else {



        bool function2tocheck = false;

        int sizeofconstraint = constraints.size();
        for (int f = 0; f < sizeofconstraint; ++f) {
          if (constraints[f].second.first == i && constraints[f].second.second == j) {
            if (matrix[constraints[f].first.first][constraints[f].first.second] != 0 && abs(matrix[constraints[f].first.first][constraints[f].first.second]) < abs(matrix[i][j])) {
              function2tocheck = true;
            }
          }

          if (constraints[f].first.first == i && constraints[f].first.second == j) {
            if (abs(matrix[constraints[f].second.first][constraints[f].second.second]) > abs(matrix[i][j])) {
              function2tocheck = true;
            }
          }

	/**ELIMINATION**/
	if ((constraints[f].first.first == i && constraints[f].first.second == j) && matrix[i][j]==-1){
    --matrix[i][j];
  }
  }

        bool function1tocheck = false;
        for (int r = 0; r < size; ++r) //her satırı ele
        {
          if (r != i && (matrix[r][j] == matrix[i][j] || matrix[r][j] == -matrix[i][j]))
            function1tocheck = true;

        }

        for (int k = 0; k < size; ++k) //her sutunu ele
        {
          if (k != j && (matrix[i][k] == matrix[i][j] || matrix[i][k] == -matrix[i][j]))
            function1tocheck = true;
        }



        if (!(function1tocheck || function2tocheck)) {
          ++s;	// güzel aday ileri
	lr=0;

        }
      }
    } else {

if(lr)--s;
else ++s;	// positife hesap yapma

    }
//if(s<0)s=0;
  }
  print(matrix, size);

}


int main(int argc, char** argv){
  std::string filename(argv[1]);

  std::ifstream file;
  file.open(filename.c_str());
  int size;

  file >> size;
  std::cout << "Size: " << size << std::endl;

  int** matrix;

  read_matrix(matrix, file, size);

  std::vector<std::pair<std::pair<int,int>,std::pair<int,int> > > constraints;
  get_constraints(constraints, file);

  for(int i = 0 ; i < size; i++){
    for(int j = 0; j < size; j++){
      std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }


auto start = std::chrono::high_resolution_clock::now();

solve(matrix, constraints, size);

auto finish = std::chrono::high_resolution_clock::now();

if (solved(matrix, constraints, size)){
std::cout << "Correct!\n";

}else{
  std::cout << "Incorrect!\n";

}

std::chrono::duration<double> elapsed = finish - start;
std::cout<<elapsed.count()<<'\n';


  //DELETE//
  for(int i = 0; i < size; i++){
    delete matrix[i];
  }

  delete[] matrix;
  //DELETE//
}

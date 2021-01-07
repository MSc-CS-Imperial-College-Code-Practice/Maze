#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include "maze.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

// I) MAIN FUNCTIONS

bool find_marker(const char ch, char **maze, const int height, 
  const int width, int &row, int &column){

  for (int i = 0; i<height; i++){
    for (int j=0; j<width; j++){
      if(maze[i][j] == ch){
        row = i;
        column = j;
        return true;
      }
      // else if(ch == '-' || ch == '|' || ch == '+'){
      //   row = -1;
      //   column = -1;
      //   return false;
      // }
    }
  }
  
  row = -1;
  column = -1;
  return false;
}
bool valid_solution(const char *path, char **maze, int height, int width){

  // Auxiliar variables
  int number_of_steps = strlen(path);
  int row_current, col_current;

  // Init current position in entrance 
  find_marker('>', maze, height, width, row_current, col_current);

  for (int i = 0; i<number_of_steps; i++){
    update_position(path[i],row_current,col_current);
    if(!is_valid_step(maze, height, width, row_current, col_current))
      return false;
  }

  // Check last position is exit
  if(maze[row_current][col_current] != 'X')
    return false;
  
  // If all check are correct then a solution was found
  return true;
}


char* find_path(char **maze, int height, int width, char start, char end){
  
  char* path = new char[255];
  Position start_pos, end_pos;
  find_marker(start, maze, height, width, start_pos.row, start_pos.col);
  find_marker(end, maze, height, width, end_pos.row, end_pos.col);
  if(!recursive_flood(maze, height, width, start_pos, end_pos, path))
    strcpy(path,"no solution");
  return path;
}

bool recursive_flood(char **maze, int height, int width, Position start_pos, 
  Position end_pos, char* path){

  char temp;
  Position current_pos = {start_pos.row, start_pos.col};
  

  // Base Case 1 -> Current Position is End Position
  if(current_pos.row == end_pos.row && current_pos.col == end_pos.col){
    maze[current_pos.row][current_pos.col] = '#';
    return true;
  }
  
  // Recursive Case -> Flooding

  // Check if movement is valid
  if(is_valid_step(maze, height, width, current_pos.row, current_pos.col)){
    
    temp= maze[current_pos.row][current_pos.col];
    maze[current_pos.row][current_pos.col] = '#';

    // Move one row down
    current_pos.row++;
    strncat(path,"S",1);
    if(recursive_flood(maze, height, width, current_pos, end_pos, path)){
        return true;
    }
    current_pos.row--; // bactracking
    path[strlen(path)-1]='\0'; // bactracking

    // Move one column right
    current_pos.col++;
    strncat(path,"E",1);
    if(recursive_flood(maze, height, width, current_pos, end_pos, path)){
        return true;
    }
    current_pos.col--; // bactracking
    path[strlen(path)-1]='\0'; // bactracking

    // Move one row up
    current_pos.row--;
    strncat(path,"N",1);
    if(recursive_flood(maze, height, width, current_pos, end_pos, path)){
        return true;
    }
    current_pos.row++; // bactracking
    path[strlen(path)-1]='\0'; // bactracking

    // Move one column left
    current_pos.col--;
    strncat(path,"W",1);
    if(recursive_flood(maze, height, width, current_pos, end_pos, path)){
        return true;
    }
    current_pos.col++; // bactracking
    path[strlen(path)-1]='\0'; // bactracking

    // Backtrack 'symbol' in maze if proposed floodings don't work
    maze[current_pos.row][current_pos.col] = temp;
  }
  return false;
}

bool is_valid_step(char** maze, int height, int width, int row, int col){

  // Check out of bounds
  if(row < 0 || col < 0 || row > (height-1) || col > (width-1)){
    return false;
  }
  // Check for a hedge
  else if(maze[row][col] == '-' || maze[row][col] == '+' ||
          maze[row][col] == '|' || maze[row][col] == '#'){
    return false;
  }
  
  return true;
}

void update_position(char direction, int &row, int &column){
  if(direction == 'N')
    row--;
  else if(direction == 'W')
    column--;
  else if(direction == 'S')
    row++;
  else if(direction == 'E')
    column++;
}


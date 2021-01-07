struct Position{
   int row; 
   int col; 
};

/* You are pre-supplied with the functions below. Add your own
   function prototypes to the end of this file. */

/* helper functions to allocate and deallocate dynamic 2D arrays */
char **allocate_2D_array(int rows, int columns);
void deallocate_2D_array(char **m, int rows);

/* pre-supplied function to load a maze from a file */
char **load_maze(const char *filename, int &height, int &width);

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width);


// I) MAIN FUNCTIONS
bool find_marker(const char ch, char **maze, int height, 
   int width, int &row, int &column);

bool valid_solution(const char *path, char **maze, int height, int width);

char* find_path(char **maze, int height, int width, char start, char end);

void update_position(char direction, int &row, int &column);
bool is_valid_step(char** maze, int height, int width, int row, int col);
bool recursive_flood(char **maze, int height, int width, Position start_pos, 
   Position end_pos, char* path);
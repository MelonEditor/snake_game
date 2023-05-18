
#define WIDTH 40
#define HEIGHT 40
extern int fps;

void randomInit();
void init_game();
void draw_grid();
void unit_square(int x, int y);
void processNormalKeys(unsigned char key, int x, int y);
void snake();
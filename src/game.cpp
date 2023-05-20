#include "game.h"
#include <glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void randomInit(){ srand((unsigned int)time(NULL)); }
int random(int start, int end){ return rand() % (end - start) + start; }
int fps = 10;

struct body{
	int posx;
	int posy;
};
struct head{
	int length = 3;
	int direction = -1;
	int dead = 0;
	int posx = length;
	int posy = HEIGHT / 2;
	struct body* segments = (body*)malloc(length * sizeof(body));
}head;
struct fruit{
	int posx;
	int posy;
	void newpos(){
		this->posx = random(2, WIDTH - 2 );
		this->posy = random(2, HEIGHT - 2);
	}
}fruit;

//increases the length of the snake body
void add_length(){
	++head.length;
	body* temp = (body*)realloc(head.segments, head.length * sizeof(body));
	if(temp != NULL){ head.segments = temp; }
	head.segments[head.length - 1].posx = head.segments[head.length - 2].posx;
	head.segments[head.length - 1].posy = head.segments[head.length - 2].posy;
}
//controls the behaviour of the snake body
void body_segment_control(){
	for(int i = head.length - 1; i >= 0; --i){
		glRectd(head.segments[i].posx, head.segments[i].posy, head.segments[i].posx + 1, head.segments[i].posy + 1);
		if(!head.dead && head.direction >= 0 && head.direction <= 3){
			if(i == 0){ 
				head.segments[i].posx = head.posx;
				head.segments[i].posy = head.posy;
				continue;
			}
			head.segments[i].posx = head.segments[i - 1].posx;
			head.segments[i].posy = head.segments[i - 1].posy;
		}
	}
}
//initializes game objects
void init_game(){
	fruit.newpos();
	for(int i = 0; i < head.length; ++i){
		head.segments[i].posx = head.posx;
		head.segments[i].posy = head.posy;
		++head.posx;
		body_segment_control();
	}
}
//draws the grid
void draw_grid(){
	for(int i = 1; i < WIDTH - 1; ++i){
		for(int j = 1; j < HEIGHT - 1; ++j){
			unit_square(i, j);
		}
	}
	glLineWidth(4.0f);
	glColor4f(.8f, .8f, .8f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(1, 1);
	glVertex2f(WIDTH - 1, 1);
	glVertex2f(WIDTH - 1, HEIGHT - 1);
	glVertex2f(1, HEIGHT - 1);
	glEnd();
}
void unit_square(int x, int y){
	glLineWidth(0.5f);
	glColor4f(.3f, .3f, .3f, 0.4f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x + 1, y);
	glVertex2f(x + 1, y + 1);
	glVertex2f(x, y + 1);
	glEnd();
}

//keyboard control
void processNormalKeys(unsigned char key, int x, int y){
	if(head.dead){ return; }
	if     (key == 'd' && head.direction != 2){ head.direction = 0; }
	else if(key == 's' && head.direction != 3){ head.direction = 1; }
	else if(key == 'a' && head.direction != 0){ head.direction = 2; }
	else if(key == 'w' && head.direction != 1){ head.direction = 3; }
}
void snake(){
	//checking for collisions
	if(head.posx > WIDTH - 2)      { head.posx = WIDTH - 2;  head.segments[0].posx = WIDTH - 2;  head.dead = 1; }
	else if(head.posx < 1 )        { head.posx = 1;          head.segments[0].posx = 1;          head.dead = 1; }
	else if(head.posy > HEIGHT - 2){ head.posy = HEIGHT - 2; head.segments[0].posy = HEIGHT - 2; head.dead = 1; }
	else if(head.posy < 1)	       { head.posy = 1;          head.segments[0].posy = 1;          head.dead = 1; }
	//tail collisions
	for(int i = 1; i < head.length; ++i){
		if(head.posx == head.segments[i].posx && head.posy == head.segments[i].posy){ 
			head.dead = 1;
		}
	}

	//drawing body and moving it in respective direction
	glRectd(head.posx, head.posy, head.posx + 1, head.posy + 1);
	if(!head.dead){
		switch(head.direction){
		case 0: ++head.posx; break;
		case 1: ++head.posy; break;
		case 2: --head.posx; break;
		case 3: --head.posy; break;
		}
	}
	body_segment_control();

	//drawing the fruit
	if(head.posx == fruit.posx && head.posy == fruit.posy){
		fruit.newpos();
		for(int i = 0; i < head.length; ++i){
			if(fruit.posx == head.segments[i].posx && fruit.posy == head.segments[i].posy){
				fruit.newpos(); i = 0;
			}
		}
		add_length();
	}
	glColor4f(0.9f, 0.0f, 0.0f, 0.6f);
	glRectd(fruit.posx, fruit.posy, fruit.posx + 1, fruit.posy + 1);
}

#include "game.h"
#include "algorithm.h"
#include <glut.h>
#include <stdlib.h>
#include <time.h>

void randomInit(){ srand((unsigned int)time(NULL)); }
int random(int start, int end){ return rand() % (end - start) + start; }
int fps = 10;


struct body{
	int posx;
	int posy;
};
struct head{
	int length = 3;
	int direction = 0;
	int dead = 0;
	int posx = 5;
	int posy = 20;
	struct body* segments = (body*)malloc(length * sizeof(body));
}head;
struct fruit{
	int posx;
	int posy;
	void newpos(){
		this->posx = random(2, 38);
		this->posy = random(2, 38);
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
		if(!head.dead){
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
		switch (head.direction) {
		case 0: ++head.posx; break;
		case 1: ++head.posy; break;
		case 2: --head.posx; break;
		case 3: --head.posy; break;
		}
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
	glVertex2f(39, 1);
	glVertex2f(39, 39);
	glVertex2f(1, 39);
	glEnd();
}
void unit_square(int x, int y){
	glLineWidth(1.5f);
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
	if     (key == 'd' && head.direction != 2 && !bot){ head.direction = 0; }
	else if(key == 'w' && head.direction != 3 && !bot){ head.direction = 1; }
	else if(key == 'a' && head.direction != 0 && !bot){ head.direction = 2; }
	else if(key == 's' && head.direction != 1 && !bot){ head.direction = 3; }
	if(key == 13){ bot = bot ? 0 : 1; }
}
void snake(){
	//checking for collisions
	if(head.posx > 38)     { head.posx = 38; head.segments[0].posx = 38; head.dead = 1; }
	else if(head.posx < 1 ){ head.posx = 1;  head.segments[0].posx = 1;  head.dead = 1; }
	else if(head.posy > 38){ head.posy = 38; head.segments[0].posy = 38; head.dead = 1; }
	else if(head.posy < 1) { head.posy = 1;  head.segments[0].posy = 1;  head.dead = 1; }

	//tail collisions
	for(int i = 1; i < head.length; ++i){
		if(head.posx == head.segments[i].posx && head.posy == head.segments[i].posy){ 
			head.dead = 1;
		}
	}

	//drawing body and moving it in respective direction
	if(bot){ glColor4f(0.6f, 0.6f, 0.8f, 1.0f); }
	else{ glColor4f(0.8f, 0.8f, 0.8f, 1.0f); }
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
		add_length();
	}
	glColor4f(0.9f, 0.0f, 0.0f, 0.6f);
	glRectd(fruit.posx, fruit.posy, fruit.posx + 1, fruit.posy + 1);
}
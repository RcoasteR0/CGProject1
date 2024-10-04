#include <iostream>
#include <random>
#include <cmath>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "RGB.h"

#define Quiz6

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

using namespace std;

random_device rd;
mt19937 gen(rd());

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);

#ifdef Quiz1
float red = 1.0f, green = 1.0f, blue = 1.0f, alpha = 1.0f;
bool timer = false;
uniform_real_distribution<float> randcolor(0.0f, 1.0f);

GLvoid Timer(int value);
#endif // Quiz1

#ifdef Quiz2
struct RGBa
{
	GLfloat Red = 0, Green = 0, Blue = 0;
};

struct Rect
{
	GLfloat left = 0, bottom = 0, right = 0, top = 0;
	RGBa rgb;
};

Rect lt = { -1.0f, 0, 0, 1.0f , {RED} };
Rect rt = { 0, 0, 1.0f, 1.0f, {GREEN} };
Rect lb = { -1.0f, -1.0f, 0, 0, {BLUE} };
Rect rb = { 0, -1.0, 1.0f, 0, {YELLOW} };

RGBa bg = { WHITE };

uniform_real_distribution<GLfloat> randcolor(0.0f, 1.0f);
void Change_RandomColor(RGBa* rgb)
{
	(*rgb).Red = randcolor(gen);
	(*rgb).Green = randcolor(gen);
	(*rgb).Blue = randcolor(gen);
}
#endif // Quiz2

#ifdef Quiz3
struct RGB
{
	GLfloat Red = 0, Green = 0, Blue = 0;
};

class Rect
{
public:
	GLfloat left, top, right, bottom;
	RGB rgb;

	Rect()
	{
		left = 0; top = 0; right = 0; bottom = 0;
	}

	Rect(GLfloat l, GLfloat b, GLfloat r, GLfloat t, RGB color)
	{
		left = l; top = t; right = r; bottom = b; rgb = color;
	}

	Rect(GLfloat l, GLfloat b, GLfloat size, RGB color)
	{
		left = l; bottom = b; right = l + size; top = b + size; rgb = color;
	}

	~Rect()
	{

	}

	void Draw()
	{
		glColor3f(rgb.Red, rgb.Green, rgb.Blue);
		glRectf(left, bottom, right, top);
	}
};

uniform_real_distribution<GLfloat> randcoord(-1.0f, 0.8f);
uniform_real_distribution<GLfloat> randcolor(0.0f, 1.0f);
uniform_real_distribution<GLfloat> randsize(0.1f, 0.5f);

RGB RandomColor()
{
	return { randcolor(gen), randcolor(gen) , randcolor(gen) };
}

bool CheckColide(Rect rect1, Rect rect2)
{
	return  (rect1.left <= rect2.right) && (rect1.top >= rect2.bottom) && (rect1.right >= rect2.left) && (rect1.bottom <= rect2.top);
}

Rect MergeRect(Rect rect1, Rect rect2)
{
	GLfloat left, bottom, right, top;

	if (rect1.left < rect2.left)
		left = rect1.left;
	else
		left = rect2.left;

	if (rect1.right > rect2.right)
		right = rect1.right;
	else
		right = rect2.right;

	if (rect1.top > rect2.top)
		top = rect1.top;
	else
		top = rect2.top;

	if (rect1.bottom < rect2.bottom)
		bottom = rect1.bottom;
	else
		bottom = rect2.bottom;

	return Rect(left, bottom, right, top, RandomColor());
}

Rect rects[10];
int rectcount = 0;
int prevX = 0;
int prevY = 0;
int dragrect = 0;
bool drag = false;

#endif // Quiz3

#ifdef Quiz4
struct RGB
{
	GLfloat Red = 0, Green = 0, Blue = 0;
};

class Rect
{
public:
	GLfloat left, top, right, bottom;
	RGB rgb;

	Rect()
	{
		left = 0; top = 0; right = 0; bottom = 0;
	}

	Rect(GLfloat l, GLfloat b, GLfloat r, GLfloat t, RGB color)
	{
		left = l; bottom = b; right = r; top = t; rgb = color;
	}

	Rect(GLfloat l, GLfloat b, GLfloat size, RGB color)
	{
		left = l; bottom = b; right = l + size; top = b + size; rgb = color;
	}

	~Rect()
	{

	}

	GLfloat Size_X() { return abs(right - left); }
	GLfloat Size_Y() { return abs(top - bottom); }
	GLfloat Middle_X() { return left + Size_X() / 2; }
	GLfloat Middle_Y() { return bottom + Size_Y() / 2; }

	void Draw()
	{
		glColor3f(rgb.Red, rgb.Green, rgb.Blue);
		glRectf(left, bottom, right, top);
	}

	void Move_X(GLfloat move)
	{
		left += move;
		right += move;
	}

	void Move_Y(GLfloat move)
	{
		top += move;
		bottom += move;
	}

	void ChangeSize_X(GLfloat size)
	{
		left = Middle_X() - size / 2;
		right = Middle_X() + size / 2;
	}

	void ChangeSize_Y(GLfloat size)
	{
		bottom = Middle_Y() - size / 2;
		top = Middle_Y() + size / 2;
	}
};

enum Animation
{
	STOP, DIAGONAL, ZIGZAG, CHANGESIZE, RANDOMCOLOR
};

uniform_real_distribution<GLfloat> randcolor(0.0f, 1.0f);
uniform_real_distribution<GLfloat> randsize(0.1f, 0.5f);
uniform_int_distribution<int> randdir(-1, 1);

RGB RandomColor()
{
	return { randcolor(gen), randcolor(gen) , randcolor(gen) };
}

bool CheckColideWall_X(Rect rect)
{
	return  (rect.left <= -1.0f) || (rect.right >= 1.0f);
}

bool CheckColideWall_Y(Rect rect)
{
	return  (rect.bottom <= -1.0f) || (rect.top >= 1.0f);
}

GLvoid Timer(int value);

const GLfloat rectsize = 0.2;
Rect rects[5];
Rect initrects[5];
Animation anim = STOP;
GLfloat move_X[5];
GLfloat move_Y[5];
int delay = 0;
int rectcount = 0;
#endif // Quiz4

#ifdef Quiz5
struct RGB
{
	GLfloat Red = 0, Green = 0, Blue = 0;
};

class Rect
{
public:
	GLfloat left, top, right, bottom;
	RGB rgb;

	Rect()
	{
		left = 0; top = 0; right = 0; bottom = 0;
	}

	Rect(GLfloat l, GLfloat b, GLfloat r, GLfloat t, RGB color)
	{
		left = l; bottom = b; right = r; top = t; rgb = color;
	}

	Rect(GLfloat l, GLfloat b, GLfloat size, RGB color)
	{
		left = l; bottom = b; right = l + size; top = b + size; rgb = color;
	}

	~Rect()
	{

	}

	GLfloat Size_X() { return abs(right - left); }
	GLfloat Size_Y() { return abs(top - bottom); }
	GLfloat Middle_X() { return left + Size_X() / 2; }
	GLfloat Middle_Y() { return bottom + Size_Y() / 2; }

	void Draw()
	{
		glColor3f(rgb.Red, rgb.Green, rgb.Blue);
		glRectf(left, bottom, right, top);
	}

	void Move_X(GLfloat move)
	{
		left += move;
		right += move;
	}

	void Move_Y(GLfloat move)
	{
		top += move;
		bottom += move;
	}

	void ChangeSize_X(GLfloat size)
	{
		left = Middle_X() - size / 2;
		right = Middle_X() + size / 2;
	}

	void ChangeSize_Y(GLfloat size)
	{
		bottom = Middle_Y() - size / 2;
		top = Middle_Y() + size / 2;
	}
};

uniform_real_distribution<GLfloat> randcolor(0.0f, 1.0f);
uniform_real_distribution<GLfloat> randcoord(-1.0f, 0.9f);
uniform_int_distribution<int> randcount(20, 40);

RGB RandomColor()
{
	return { randcolor(gen), randcolor(gen) , randcolor(gen) };
}

bool CheckColide(Rect rect1, Rect rect2)
{
	return  (rect1.left <= rect2.right) && (rect1.top >= rect2.bottom) && (rect1.right >= rect2.left) && (rect1.bottom <= rect2.top);
}

void InitializeArray(Rect rects[40], int* rectcount)
{
	*rectcount = randcount(gen);

	cout << "rect :" << *rectcount << endl;
	for (int i = 0; i < *rectcount; ++i)
		rects[i] = Rect(randcoord(gen), randcoord(gen), 0.05f, RandomColor());
}

GLvoid Timer(int value);

Rect rects[40];
Rect eraser;
int rectcount;
int prevX = 0;
int prevY = 0;
bool hold = false;
#endif // Quiz5

#ifdef Quiz6
struct RGB
{
	GLfloat Red = 0, Green = 0, Blue = 0;
};

class Rect
{
public:
	GLfloat left, top, right, bottom;
	RGB rgb;

	Rect()
	{
		left = 0; top = 0; right = 0; bottom = 0;
	}

	Rect(GLfloat l, GLfloat b, GLfloat r, GLfloat t, RGB color)
	{
		left = l; bottom = b; right = r; top = t; rgb = color;
	}

	Rect(GLfloat l, GLfloat b, GLfloat size, RGB color)
	{
		left = l; bottom = b; right = l + size; top = b + size; rgb = color;
	}

	~Rect()
	{

	}

	GLfloat Size_X() { return abs(right - left); }
	GLfloat Size_Y() { return abs(top - bottom); }
	GLfloat Middle_X() { return left + Size_X() / 2; }
	GLfloat Middle_Y() { return bottom + Size_Y() / 2; }

	void Draw()
	{
		glColor3f(rgb.Red, rgb.Green, rgb.Blue);
		glRectf(left, bottom, right, top);
	}

	void Move_X(GLfloat move)
	{
		left += move;
		right += move;
	}

	void Move_Y(GLfloat move)
	{
		top += move;
		bottom += move;
	}

	void ChangeSize_X(GLfloat size)
	{
		left = Middle_X() - size / 2;
		right = Middle_X() + size / 2;
	}

	void ChangeSize_Y(GLfloat size)
	{
		bottom = Middle_Y() - size / 2;
		top = Middle_Y() + size / 2;
	}
};

enum Split
{
	STRAIGHT, DIAGONAL, ONEDIRECTION, EIGHTDIRECTION
};

uniform_real_distribution<GLfloat> randcolor(0.0f, 1.0f);
uniform_real_distribution<GLfloat> randsize(0.1f, 0.5f);
uniform_real_distribution<GLfloat> randcoord(-1.0f, 0.9f);

RGB RandomColor()
{
	return { randcolor(gen), randcolor(gen) , randcolor(gen) };
}

GLvoid Timer(int value);

Rect rects[10];
Rect splitedrects[80];
Split splittype[10];
int rectcount;
#endif // Quiz6

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv); //--- glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //--- 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); //--- 윈도우의 위치 지정
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); //--- 윈도우의 크기 지정
	glutCreateWindow("Example1"); //--- 윈도우 생성(윈도우 이름)
	
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) //--- glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

#ifdef Quiz5
	InitializeArray(rects, &rectcount);
#endif // Quiz5

	glutDisplayFunc(drawScene); //--- 출력 콜백함수의 지정
	glutReshapeFunc(Reshape); //--- 다시 그리기 콜백함수 지정
	glutKeyboardFunc(Keyboard); //--- 키보드 입력 콜백함수 지정
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

#ifdef Quiz1
	glutTimerFunc(1000, Timer, 1);
#endif // Quiz1
#ifdef Quiz4
	glutTimerFunc(1000 / 60, Timer, 1);
#endif // Quiz4
#ifdef Quiz5
	glutTimerFunc(1000 / 60, Timer, 1);
#endif // Quiz4

	glutMainLoop(); //--- 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;

#ifdef Quiz1
	r = red; g = green; b = blue; a = alpha;
#endif // Quiz1
#ifdef Quiz2
	r = bg.Red; g = bg.Green; b = bg.Blue;
#endif // Quiz2
#ifdef Quiz4
	r = 0.3f; g = 0.3f; b = 0.3f;
#endif // Quiz4

	glClearColor(r, g, b, a); //--- 바탕색을 변경
	glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기

#ifdef Quiz2
	glColor3f(lt.rgb.Red, lt.rgb.Green, lt.rgb.Blue);
	glRectf(lt.left, lt.bottom, lt.right, lt.top);

	glColor3f(rt.rgb.Red, rt.rgb.Green, rt.rgb.Blue);
	glRectf(rt.left, rt.bottom, rt.right, rt.top);

	glColor3f(lb.rgb.Red, lb.rgb.Green, lb.rgb.Blue);
	glRectf(lb.left, lb.bottom, lb.right, lb.top);

	glColor3f(rb.rgb.Red, rb.rgb.Green, rb.rgb.Blue);
	glRectf(rb.left, rb.bottom, rb.right, rb.top);
#endif // Quiz2
#ifdef Quiz3
	for (int i = 0; i < rectcount; ++i)
		rects[i].Draw();
#endif // Quiz3
#ifdef Quiz4
	for (int i = 0; i < rectcount; ++i)
		rects[i].Draw();
#endif // Quiz4
#ifdef Quiz5
	for (int i = 0; i < rectcount; ++i)
	{
		rects[i].Draw();
	}
	if (hold)
	{
		eraser.Draw();
	}
#endif // Quiz5

	glutSwapBuffers(); //--- 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
#ifdef Quiz1
	switch (key) {
	case 'c': //--- 배경색을 청록색으로 설정
		red = 0.0f; green = 1.0f; blue = 1.0f;
		break;
	case 'm': //--- 배경색을 자홍색으로 설정
		red = 1.0f; green = 0.0f; blue = 1.0f;
		break;
	case 'y': //--- 배경색을 노랑색으로 설정
		red = 1.0f; green = 1.0f; blue = 0.0f;
		break;
	case 'a': // 랜덤색
		red = randcolor(gen); green = randcolor(gen); blue = randcolor(gen);
		break;
	case 'w': // 백색
		red = 1.0f; green = 1.0f; blue = 1.0f;
		break;
	case 'k': // black
		red = 0.0f; green = 0.0f; blue = 0.0f;
		break;
	case 't': // set timer and random color
		timer = true;
		break;
	case 's': // end timer
		timer = false;
		break;
	}
#endif // Quiz1
#ifdef Quiz3
	if (key == 'a' && rectcount < 10)
	{
		rects[rectcount] = Rect(randcoord(gen), randcoord(gen), randsize(gen), RandomColor());
		++rectcount;
	}
#endif // Quiz3
#ifdef Quiz4
	switch (key)
	{
	case '1':
		if (anim == DIAGONAL)
			anim = STOP;
		else
		{
			anim = DIAGONAL;

			for (int i = 0; i < rectcount; ++i)
			{
				move_X[i] = 0.05f * randdir(gen);
				move_Y[i] = 0.05f * randdir(gen);
				if (!(move_X[i] && move_Y[i]))
					--i;
			}
		}
		break;
	case '2':
		if (anim == ZIGZAG)
			anim = STOP;
		else
		{
			anim = ZIGZAG;

			for (int i = 0; i < rectcount; ++i)
			{
				move_X[i] = 0.0125f * randdir(gen);
				move_Y[i] = 0.05f * randdir(gen);
				if (!(move_X[i] && move_Y[i]))
					--i;
			}
		}
		break;
	case '3':
		if (anim == CHANGESIZE)
			anim = STOP;
		else
		{
			anim = CHANGESIZE;
			delay = 0;
		}
		break;
	case '4':
		if (anim == RANDOMCOLOR)
			anim = STOP;
		else
		{
			anim = RANDOMCOLOR;
			delay = 0;
		}
		break;
	case 's':
		anim = STOP;
		break;
	case 'm':
		for (int i = 0; i < rectcount; ++i)
			rects[i] = initrects[i];
		break;
	case 'r':
		rectcount = 0;
		break;
	default:
		break;
	}
#endif // Quiz4
#ifdef Quiz5
	if (key == 'r')
	{
		InitializeArray(rects, &rectcount);
	}
#endif // Quiz5

	//프로그램 종료
	if (key == 'q')
		glutLeaveMainLoop();

	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

GLvoid Mouse(int button, int state, int x, int y)
{
	GLfloat mouseX = x - WINDOW_WIDTH / 2;
	GLfloat mouseY = WINDOW_HEIGHT / 2 - y;

#ifdef Quiz2
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (mouseX >= lt.left * WINDOW_WIDTH / 2 && mouseX <= lt.right * WINDOW_WIDTH / 2 
			&& mouseY <= lt.top * WINDOW_HEIGHT / 2 && mouseY >= lt.bottom * WINDOW_HEIGHT / 2)
			Change_RandomColor(&(lt.rgb));
		else if (mouseX >= rt.left * WINDOW_WIDTH / 2 && mouseX <= rt.right * WINDOW_WIDTH / 2
			&& mouseY <= rt.top * WINDOW_HEIGHT / 2 && mouseY >= rt.bottom * WINDOW_HEIGHT / 2)
			Change_RandomColor(&(rt.rgb));
		else if (mouseX >= lb.left * WINDOW_WIDTH / 2 && mouseX <= lb.right * WINDOW_WIDTH / 2
			&& mouseY <= lb.top * WINDOW_HEIGHT / 2 && mouseY >= lb.bottom * WINDOW_HEIGHT / 2)
			Change_RandomColor(&(lb.rgb));
		else if (mouseX >= rb.left * WINDOW_WIDTH / 2 && mouseX <= rb.right * WINDOW_WIDTH / 2
			&& mouseY <= rb.top * WINDOW_HEIGHT / 2 && mouseY >= rb.bottom * WINDOW_HEIGHT / 2)
			Change_RandomColor(&(rb.rgb));
		else
			Change_RandomColor(&bg);
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (mouseX >= -WINDOW_WIDTH / 2 && mouseX <= 0 && mouseY <= WINDOW_HEIGHT / 2 && mouseY >= 0)
		{
			if (mouseX >= lt.left * WINDOW_WIDTH / 2 && mouseX <= lt.right * WINDOW_WIDTH / 2
				&& mouseY <= lt.top * WINDOW_HEIGHT / 2 && mouseY >= lt.bottom * WINDOW_HEIGHT / 2)
			{
				lt.left += 0.05f; lt.bottom += 0.05f;
				lt.right -= 0.05f; lt.top -= 0.05f;
			}
			else
			{
				lt.left -= 0.05f; lt.bottom -= 0.05f;
				lt.right += 0.05f; lt.top += 0.05f;
			}
		}
		else if (mouseX >= 0 && mouseX <= WINDOW_WIDTH / 2 && mouseY <= WINDOW_HEIGHT / 2 && mouseY >= 0)
		{
			if (mouseX >= rt.left * WINDOW_WIDTH / 2 && mouseX <= rt.right * WINDOW_WIDTH / 2
				&& mouseY <= rt.top * WINDOW_HEIGHT / 2 && mouseY >= rt.bottom * WINDOW_HEIGHT / 2)
			{
				rt.left += 0.05f; rt.bottom += 0.05f;
				rt.right -= 0.05f; rt.top -= 0.05f;
			}
			else
			{
				rt.left -= 0.05f; rt.bottom -= 0.05f;
				rt.right += 0.05f; rt.top += 0.05f;
			}
		}
		else if (mouseX >= -WINDOW_WIDTH / 2 && mouseX <= 0 && mouseY <= 0 && mouseY >= -WINDOW_HEIGHT / 2)
		{
			if (mouseX >= lb.left * WINDOW_WIDTH / 2 && mouseX <= lb.right * WINDOW_WIDTH / 2
				&& mouseY <= lb.top * WINDOW_HEIGHT / 2 && mouseY >= lb.bottom * WINDOW_HEIGHT / 2)
			{
				lb.left += 0.05f; lb.bottom += 0.05f;
				lb.right -= 0.05f; lb.top -= 0.05f;
			}
			else
			{
				lb.left -= 0.05f; lb.bottom -= 0.05f;
				lb.right += 0.05f; lb.top += 0.05f;
			}
		}
		else if (mouseX >= 0 && mouseX <= WINDOW_WIDTH / 2 && mouseY <= 0 && mouseY >= -WINDOW_HEIGHT / 2)
		{
			if (mouseX >= rb.left * WINDOW_WIDTH / 2 && mouseX <= rb.right * WINDOW_WIDTH / 2
				&& mouseY <= rb.top * WINDOW_HEIGHT / 2 && mouseY >= rb.bottom * WINDOW_HEIGHT / 2)
			{
				rb.left += 0.05f; rb.bottom += 0.05f;
				rb.right -= 0.05f; rb.top -= 0.05f;
			}
			else
			{
				rb.left -= 0.05f; rb.bottom -= 0.05f;
				rb.right += 0.05f; rb.top += 0.05f;
			}
		}

	}
#endif // Quiz2
#ifdef Quiz3
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		for (int i = rectcount - 1; i >= 0; --i)
		{
			if (mouseX >= rects[i].left * WINDOW_WIDTH / 2 && mouseX <= rects[i].right * WINDOW_WIDTH / 2
				&& mouseY <= rects[i].top * WINDOW_HEIGHT / 2 && mouseY >= rects[i].bottom * WINDOW_HEIGHT / 2)
			{
				drag = true;
				dragrect = i;
				prevX = mouseX;
				prevY = mouseY;
				break;
			}
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (drag)
		{
			drag = false;
			for (int i = rectcount - 1; i >= 0; --i)
			{
				if (i == dragrect)
					continue;

				if (CheckColide(rects[i], rects[dragrect]))
				{
					rects[dragrect] = MergeRect(rects[i], rects[dragrect]);
					
					for (int j = i; j < rectcount; ++j)
					{
						rects[j] = rects[j + 1];
					}
					--rectcount;
					break;
				}
			}
		}
	}
#endif // Quiz3
#ifdef Quiz4
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && rectcount < 5)
	{
		rects[rectcount] = Rect(mouseX / (WINDOW_WIDTH / 2) - rectsize / 2, mouseY / (WINDOW_HEIGHT / 2) - rectsize / 2, rectsize, RandomColor());
		initrects[rectcount] = rects[rectcount];
		++rectcount;
	}
#endif // Quiz4
#ifdef Quiz5
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		hold = true;
		eraser = Rect(mouseX / (WINDOW_WIDTH / 2) - 0.1f / 2, mouseY / (WINDOW_HEIGHT / 2) - 0.1f / 2, 0.1f, { BLACK });
		prevX = mouseX;
		prevY = mouseY;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		hold = false;
	}

#endif // Quiz5

}

GLvoid Motion(int x, int y)
{
	GLfloat mouseX = x - WINDOW_WIDTH / 2;
	GLfloat mouseY = WINDOW_HEIGHT / 2 - y;
#ifdef Quiz3
	if (drag)
	{
		rects[dragrect].left += (mouseX - prevX) / (WINDOW_WIDTH / 2);
		rects[dragrect].right += (mouseX - prevX) / (WINDOW_WIDTH / 2);
		rects[dragrect].top += (mouseY - prevY) / (WINDOW_HEIGHT / 2);
		rects[dragrect].bottom += (mouseY - prevY) / (WINDOW_HEIGHT / 2);
		prevX = mouseX;
		prevY = mouseY;
	}
#endif // Quiz3
#ifdef Quiz5
	if (hold)
	{
		eraser.left += (mouseX - prevX) / (WINDOW_WIDTH / 2);
		eraser.right += (mouseX - prevX) / (WINDOW_WIDTH / 2);
		eraser.top += (mouseY - prevY) / (WINDOW_HEIGHT / 2);
		eraser.bottom += (mouseY - prevY) / (WINDOW_HEIGHT / 2);
		prevX = mouseX;
		prevY = mouseY;
	}
#endif // Quiz5

	glutPostRedisplay();
}

#ifdef Quiz1
GLvoid Timer(int value)
{
	if (timer)
	{
		red = randcolor(gen); green = randcolor(gen); blue = randcolor(gen);
	}

	glutPostRedisplay();
	glutTimerFunc(1000, Timer, 1);
}
#endif // Quiz1
#ifdef Quiz4
GLvoid Timer(int value)
{
	switch (anim)
	{
	case DIAGONAL:
		for (int i = 0; i < rectcount; ++i)
		{
			rects[i].Move_X(move_X[i]);
			rects[i].Move_Y(move_Y[i]);

			if (CheckColideWall_X(rects[i]))
				move_X[i] *= -1;
			if (CheckColideWall_Y(rects[i]))
				move_Y[i] *= -1;
		}
		break;
	case ZIGZAG:
		for (int i = 0; i < rectcount; ++i)
		{
			rects[i].Move_X(move_X[i]);
			rects[i].Move_Y(move_Y[i]);

			if (CheckColideWall_X(rects[i]))
				move_X[i] *= -1;
			if (CheckColideWall_Y(rects[i]))
				move_Y[i] *= -1;
		}
		break;
	case CHANGESIZE:
		if (delay == 0)
		{
			for (int i = 0; i < rectcount; ++i)
			{
				rects[i].ChangeSize_X(randsize(gen));
				rects[i].ChangeSize_Y(randsize(gen));
			}
		}
		else if (delay == 29)
			delay = -1;

		++delay;
		break;
	case RANDOMCOLOR:
		if (delay == 0)
		{
			for (int i = 0; i < rectcount; ++i)
				rects[i].rgb = RandomColor();
		}
		else if (delay == 29)
			delay = -1;

		++delay;
		break;
	default:
		break;
	}

	glutPostRedisplay();
	glutTimerFunc(1000 / 60, Timer, 1);
}
#endif // Quiz4
#ifdef Quiz5
GLvoid Timer(int value)
{
	if (hold)
	{
		for (int i = rectcount - 1; i >= 0; --i)
		{
			if (CheckColide(rects[i], eraser))
			{
				eraser.ChangeSize_X(eraser.Size_X() + 0.1f);
				eraser.ChangeSize_Y(eraser.Size_Y() + 0.1f);
				eraser.rgb = rects[i].rgb;

				for (int j = i; j < rectcount; ++j)
				{
					rects[j] = rects[j + 1];
				}
				--rectcount;
				break;
			}
		}
	}

	glutPostRedisplay();
	glutTimerFunc(1000 / 60, Timer, 1);
}
#endif // Quiz5


#include <iostream>
#include <random>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "RGB.h"

#define Quiz2

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

using namespace std;

random_device rd;
mt19937 gen(rd());

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

#ifdef Quiz1
float red = 1.0f, green = 1.0f, blue = 1.0f, alpha = 1.0f;
bool timer = false;
uniform_real_distribution<float> randcolor(0.0f, 1.0f);

GLvoid Timer(int value);
#endif // Quiz1

#ifdef Quiz2
struct RGBa
{
	GLfloat Red = 0, Green = 0, Blue = 0, Alpha = 1;
};

struct Rect
{
	GLfloat left = 0, top = 0, right = 0, bottom = 0;
	RGBa rgb;
};

Rect lt = { -WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0, 0 , {RED} };
Rect rt = { 0, WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2, 0 , {GREEN} };
Rect lb = { -WINDOW_WIDTH / 2, 0, 0, -WINDOW_HEIGHT / 2 , {BLUE} };
Rect rb = { 0, 0, WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 2, {YELLOW} };
#endif // Quiz2

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

	glutDisplayFunc(drawScene); //--- 출력 콜백함수의 지정
	glutReshapeFunc(Reshape); //--- 다시 그리기 콜백함수 지정
	glutKeyboardFunc(Keyboard); //--- 키보드 입력 콜백함수 지정

#ifdef Quiz1
	glutTimerFunc(1000, Timer, 1);
#endif // Quiz1

	glutMainLoop(); //--- 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	float r, g, b, a;

#ifdef Quiz1
	r = red; g = green; b = blue; a = alpha;
#endif // Quiz1
#ifdef Quiz2
	r = 1.0f; g = 1.0f; b = 1.0f; a = 1.0f;


#endif // Quiz2

	glClearColor(r, g, b, a); //--- 바탕색을 변경
	glClear(GL_COLOR_BUFFER_BIT); //--- 설정된 색으로 전체를 칠하기
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
	case 'q':
		glutLeaveMainLoop();
		break;
	}
#endif // Quiz1

	glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
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



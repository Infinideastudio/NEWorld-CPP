#include "Setting.h"

int windowwidth;
int windowheight;
int handleLimit = 2;
bool retina = false;			//retina
bool useLoading = true;         //���ؽ���
bool SmoothLighting = true;		//ƽ������
bool versync = false;			//��ֱͬ��
float FOVyNormal = 60.0;		//��Ұ�Ƕ�
float FOVyRunning = 8.0f;
float FOVyExt;
float mousemove = 0.25;			//���������
int viewdistance = 5;			//��Ұ����
int selectPrecision = 100;		//ѡ�񷽿�ľ���
int selectDistance = 10;		//ѡ�񷽿�ľ���
float walkspeed = 0.15f;		//���ǰ���ٶ�
float runspeed = 0.3f;			//����ܲ��ٶ�
int MaxAirJumps = 2;			//����N������
bool UseCIArray = false;		//ʹ��CIA
int linelength = 10;
int linedist = 30;
float skycolorR = 0.7f;
float skycolorG = 1.0f;
float skycolorB = 1.0f;
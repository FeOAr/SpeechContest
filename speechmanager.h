#pragma once
#include<vector>
#include<map>
#include"player.h"

/*�ݽ�������*/
class SpeechManager
{
public:
	SpeechManager();  /*���캯��*/
	void showMenu();  /*��ʱ��ʾ*/
	void exitSystem();
	void initPlayers();
	void creatPlayers();
	void startSpeech();
	void randomSpeechSequence();  /*���˳��*/
	void speechContest();  /*��ʼ����*/
	void displayScore();  /*��ʾ����*/
	void saveRecord();  /*��¼д���ļ�*/
	void loadRecord();  /*���ļ���ȡ��¼*/
	void displayRecord();
	void clearRecord();  /*������м�¼*/
	void show();
	~SpeechManager();	/*��������*/
public:
	vector<int>v1;  /*����ѡ���������洢ѡ�ֱ��*/
	vector<int>v2;  /*��һ�ֽ���*/
	vector<int>vVictory;  /*����ʤ��*/

	map<int, Player>mPlayer;  /*��ű���Լ���Ӧѡ��*/
	int turn;  /*��������*/

	bool fileIsEmpty;  /*���ļ���־*/
	map<int, vector<string>> Record;  /*�����¼���죬ÿ����������*/
};
#pragma once
#include<vector>
#include<map>
#include"player.h"

/*演讲管理类*/
class SpeechManager
{
public:
	SpeechManager();  /*构造函数*/
	void showMenu();  /*临时显示*/
	void exitSystem();
	void initPlayers();
	void creatPlayers();
	void startSpeech();
	void randomSpeechSequence();  /*随机顺序*/
	void speechContest();  /*开始比赛*/
	void displayScore();  /*显示分数*/
	void saveRecord();  /*记录写入文件*/
	void loadRecord();  /*从文件读取记录*/
	void displayRecord();
	void clearRecord();  /*清除所有记录*/
	void show();
	~SpeechManager();	/*析构函数*/
public:
	vector<int>v1;  /*所有选手容器，存储选手编号*/
	vector<int>v2;  /*第一轮晋级*/
	vector<int>vVictory;  /*最终胜利*/

	map<int, Player>mPlayer;  /*存放编号以及对应选手*/
	int turn;  /*比赛轮数*/

	bool fileIsEmpty;  /*空文件标志*/
	map<int, vector<string>> Record;  /*往届记录，届，每届所有数据*/
};
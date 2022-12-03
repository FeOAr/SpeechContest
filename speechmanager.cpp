#include "speechManager.h"
#include<iostream>
#include<algorithm>
#include <ctime>
#include<deque>
#include <numeric>
#include <fstream>
using namespace std;

SpeechManager::SpeechManager()
{
	this->initPlayers();
	this->creatPlayers();
	this->loadRecord();
}

SpeechManager::~SpeechManager()
{
}

void SpeechManager::showMenu() {
	cout << "********************************************" << endl;
	cout << "*************  欢迎参加演讲比赛 ************" << endl;
	cout << "*************  1.开始演讲比赛  *************" << endl;
	cout << "*************  2.查看往届记录  *************" << endl;
	cout << "*************  3.清空比赛记录  *************" << endl;
	cout << "*************  0.退出比赛程序  *************" << endl;
	cout << "********************************************" << endl;
	cout << endl;
}

void SpeechManager::initPlayers() {
	this->v1.clear();
	this->v2.clear();
	this->vVictory.clear();
	this->mPlayer.clear();
	this->turn = 1;
	this->fileIsEmpty = true;
	this->Record.clear();
}

void SpeechManager::creatPlayers() {
	/*此处可以有其他方式*/
	string nameSeed = "ABCDEFGHIJKL";
	for (int i = 0; i < nameSeed.length(); i++) {
		string name = "player-";
		name += nameSeed[i];

		Player tempPlayer;
		tempPlayer.name = name;
		for (int n = 0; n < 2; n++)
		{
			tempPlayer.score[n] = 0;
		}

		this->v1.push_back(20221201+i);
		this->mPlayer.insert(make_pair(20221201 + i, tempPlayer));
	}
}

void SpeechManager::startSpeech() {
	/*first turn*/
	randomSpeechSequence();  /*抽签*/
	speechContest();  /*比赛*/
	displayScore();  /*显示结果*/

	/*second turn*/
	this->turn++;
	randomSpeechSequence();  /*抽签*/
	speechContest();  /*比赛*/
	displayScore();  /*显示结果*/

	saveRecord();  /*保存结果*/

	cout << "本届比赛结束！" << endl;
}

void SpeechManager::randomSpeechSequence() {
	srand((unsigned int)time(NULL));

	cout << "第 << " << this->turn << " >> 轮比赛选手正在抽签" << endl;
	cout << "---------------------" << endl;
	cout << "抽签后演讲顺序如下：" << endl;
	if (turn == 1) {
		random_shuffle(v1.begin(), v1.end());
		for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++)
		{
			cout << *it << ' ';
		}cout << endl;
	}
	else
	{
		random_shuffle(v2.begin(), v2.end());
		for (vector<int>::iterator it = v2.begin(); it != v2.end(); it++)
		{
			cout << *it << ' ';
		}cout << endl;
	}
	cout << "---------------------" << endl;
}

void SpeechManager::speechContest() {
	cout << "------------- 第" << turn << "轮正式比赛开始：------------- " << endl;
	multimap<double, int, greater<int>> groupScore; //临时容器，分数为key， 选手编号为value，降序插入
	int calculate = 0;  /*计数器，分组用*/

	vector<int> tempVector;  /*具体操作的工具容器*/
	if (turn == 1) {
		tempVector = v1;
	}
	else
	{
		tempVector = v2;
	}

	/*核心比赛功能*/
	for (vector<int>::iterator it = tempVector.begin(); it != tempVector.end(); it++) {
		calculate++;

		/*打分*/
		deque<float>scoreDeque;  /*双端队列*/
		for (int i = 0; i < 10; i++)  /*十个评委的打分，装进队列*/
		{
			float point = (rand() % 401 + 600) / 10.f;  /*.f强转10为float，否做赋给float类型的左值时编译器会警告*/
			scoreDeque.push_back(point);
		}
		sort(scoreDeque.begin(), scoreDeque.end(), greater<float>());  /*降序*/
		scoreDeque.pop_front();  /*去掉一个最高分*/
		scoreDeque.pop_back();  /*去掉一个最低分*/
		double sum = accumulate(scoreDeque.begin(), scoreDeque.end(), 0.0f);
		double avg = sum / (double)scoreDeque.size();  /*求平均分*/
		mPlayer[*it].score[turn - 1] = avg;

		groupScore.insert(make_pair(avg, *it));  /*平均分与编号，降序插入*/

		if (calculate % 6 == 0) {
			cout << "第" << calculate / 6 << "小组比赛名次：" << endl;
			for (multimap<double, int, greater<int>>::iterator it = groupScore.begin(); it != groupScore.end(); it++)
			{
				/*此处it->second 为选手编号*/
				cout << "playerId: " << it->second
					<< " name: " << this->mPlayer[it->second].name 
					<< " score： " << this->mPlayer[it->second].score[turn-1] << endl;
			}

			int count = 0;

			/*该组前三名，晋级*/
			for (multimap<double, int, greater<int>>::iterator it = groupScore.begin(); it != groupScore.end() && count < 3; it++, count++) {
				if (turn == 1)
				{
					v2.push_back(it->second);  /*第一轮晋级就下一轮，不用清理*/
				}
				else {
					vVictory.push_back(it->second);  /*第二轮晋级就出结果*/
				}
			}
			groupScore.clear();  /*处理完六个选手后，清理*/
			cout << endl;
		}
	}
	cout << "------------- 第" << turn << "轮比赛完毕  ------------- " << endl;
	system("pause");
}

void SpeechManager::displayScore() {
	cout << "---------第" << this->turn << "轮晋级选手信息如下：-----------" << endl;
	vector<int> toolVector;
	if (turn == 1) {
		toolVector = v2;
	}
	else
	{
		toolVector = vVictory;
	}

	for (vector<int>::iterator it = toolVector.begin(); it != toolVector.end(); it++) {
		cout << "playerId: " << *it
			<< " name: " << mPlayer[*it].name
			<< " score： " << mPlayer[*it].score[turn - 1] << endl;
	}
}

void SpeechManager::saveRecord() {
	ofstream ofs;
	ofs.open("SpeechRecord.csv", ios::out|ios::app);  /*写方式，追加写*/

	for (vector<int>::iterator it = vVictory.begin(); it != vVictory.end(); it++)
	{
		ofs << *it << "," << mPlayer[*it].name << "," <<mPlayer[*it].score[1] << ",";
	}
	ofs << endl;
	ofs.close();

	cout << "记录已保存" << endl;
	this->fileIsEmpty = false;
}

void SpeechManager::loadRecord() {
	ifstream ifs("SpeechRecord.csv", ios::in);

	if (!ifs.is_open()) {
		this->fileIsEmpty = true;
		cout << "文件不存在！" << endl;
		ifs.close();
		return;
	}

	char iseof;
	ifs >> iseof;  /*读一个字符后，指针后移才会到结束符*/
	if (ifs.eof()) {
		this->fileIsEmpty = true;
		cout << "文件为空！" << endl;
		ifs.close();
		return;
	}

	this->fileIsEmpty = false;

	ifs.putback(iseof);

	string data;
	int index = 0;
	while (ifs >> data)
	{
		int position = -1;
		int begin = 0;
		vector<string> strRecord;
		while (true)
		{
			position = data.find(',', begin);
			if (position == -1) {  /*查找异常*/
				break;
			}
			string temp = data.substr(begin, position - begin);
			begin = position + 1;
			strRecord.push_back(temp);
		}
		Record.insert(make_pair(index, strRecord));
		index++;
	}
	ifs.close();
}

void SpeechManager::displayRecord() {
	if (this->fileIsEmpty) {
		cout << "文件不存在或为空" << endl;
	}
	else
	{
		string awardType[] = { "冠军","亚军","季军" };
		for (int i = 0; i < this->Record.size(); i++)
		{
			cout << "第" << i + 1 << "届 " << endl;
			for (int n = 0; n <= 6; n += 3)
			{
				cout << awardType[n / 3] << "编号：" << this->Record[i][n]
					<< " 姓名: " << this->Record[i][n + 1]
					<< " 得分：" << this->Record[i][n + 2] << " " << endl;
			}
		}
	}
	system("pause");
	system("cls");
}

void SpeechManager::clearRecord() {
	char choice;
	cout << "确认清空？(y/n)" << endl;
	cin >> choice;
	if ((choice == 'y') || (choice == 'Y')) {
		ofstream ofs("SpeechRecord.csv", ios::trunc);
		ofs.close();

		//初始化属性
		this->initPlayers();
		this->creatPlayers();
		this->loadRecord();
		cout << "清空成功！" << endl;
	}
}

void SpeechManager::exitSystem() {
	cout << "感谢使用，再见！" << endl;
	exit(0);
}

void SpeechManager::show() {
	for (map<int, Player>::iterator it = this->mPlayer.begin(); it != mPlayer.end(); it++)
	{
		cout << "playerId: " << it->first
			<< "  name: " << it->second.name
			<< "  score[0]: " << it->second.score[0]
			<< "  score[1]: " << it->second.score[1] << endl;
	}
}
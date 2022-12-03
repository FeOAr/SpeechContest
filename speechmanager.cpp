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
	cout << "*************  ��ӭ�μ��ݽ����� ************" << endl;
	cout << "*************  1.��ʼ�ݽ�����  *************" << endl;
	cout << "*************  2.�鿴�����¼  *************" << endl;
	cout << "*************  3.��ձ�����¼  *************" << endl;
	cout << "*************  0.�˳���������  *************" << endl;
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
	/*�˴�������������ʽ*/
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
	randomSpeechSequence();  /*��ǩ*/
	speechContest();  /*����*/
	displayScore();  /*��ʾ���*/

	/*second turn*/
	this->turn++;
	randomSpeechSequence();  /*��ǩ*/
	speechContest();  /*����*/
	displayScore();  /*��ʾ���*/

	saveRecord();  /*������*/

	cout << "�������������" << endl;
}

void SpeechManager::randomSpeechSequence() {
	srand((unsigned int)time(NULL));

	cout << "�� << " << this->turn << " >> �ֱ���ѡ�����ڳ�ǩ" << endl;
	cout << "---------------------" << endl;
	cout << "��ǩ���ݽ�˳�����£�" << endl;
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
	cout << "------------- ��" << turn << "����ʽ������ʼ��------------- " << endl;
	multimap<double, int, greater<int>> groupScore; //��ʱ����������Ϊkey�� ѡ�ֱ��Ϊvalue���������
	int calculate = 0;  /*��������������*/

	vector<int> tempVector;  /*��������Ĺ�������*/
	if (turn == 1) {
		tempVector = v1;
	}
	else
	{
		tempVector = v2;
	}

	/*���ı�������*/
	for (vector<int>::iterator it = tempVector.begin(); it != tempVector.end(); it++) {
		calculate++;

		/*���*/
		deque<float>scoreDeque;  /*˫�˶���*/
		for (int i = 0; i < 10; i++)  /*ʮ����ί�Ĵ�֣�װ������*/
		{
			float point = (rand() % 401 + 600) / 10.f;  /*.fǿת10Ϊfloat����������float���͵���ֵʱ�������ᾯ��*/
			scoreDeque.push_back(point);
		}
		sort(scoreDeque.begin(), scoreDeque.end(), greater<float>());  /*����*/
		scoreDeque.pop_front();  /*ȥ��һ����߷�*/
		scoreDeque.pop_back();  /*ȥ��һ����ͷ�*/
		double sum = accumulate(scoreDeque.begin(), scoreDeque.end(), 0.0f);
		double avg = sum / (double)scoreDeque.size();  /*��ƽ����*/
		mPlayer[*it].score[turn - 1] = avg;

		groupScore.insert(make_pair(avg, *it));  /*ƽ�������ţ��������*/

		if (calculate % 6 == 0) {
			cout << "��" << calculate / 6 << "С��������Σ�" << endl;
			for (multimap<double, int, greater<int>>::iterator it = groupScore.begin(); it != groupScore.end(); it++)
			{
				/*�˴�it->second Ϊѡ�ֱ��*/
				cout << "playerId: " << it->second
					<< " name: " << this->mPlayer[it->second].name 
					<< " score�� " << this->mPlayer[it->second].score[turn-1] << endl;
			}

			int count = 0;

			/*����ǰ����������*/
			for (multimap<double, int, greater<int>>::iterator it = groupScore.begin(); it != groupScore.end() && count < 3; it++, count++) {
				if (turn == 1)
				{
					v2.push_back(it->second);  /*��һ�ֽ�������һ�֣���������*/
				}
				else {
					vVictory.push_back(it->second);  /*�ڶ��ֽ����ͳ����*/
				}
			}
			groupScore.clear();  /*����������ѡ�ֺ�����*/
			cout << endl;
		}
	}
	cout << "------------- ��" << turn << "�ֱ������  ------------- " << endl;
	system("pause");
}

void SpeechManager::displayScore() {
	cout << "---------��" << this->turn << "�ֽ���ѡ����Ϣ���£�-----------" << endl;
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
			<< " score�� " << mPlayer[*it].score[turn - 1] << endl;
	}
}

void SpeechManager::saveRecord() {
	ofstream ofs;
	ofs.open("SpeechRecord.csv", ios::out|ios::app);  /*д��ʽ��׷��д*/

	for (vector<int>::iterator it = vVictory.begin(); it != vVictory.end(); it++)
	{
		ofs << *it << "," << mPlayer[*it].name << "," <<mPlayer[*it].score[1] << ",";
	}
	ofs << endl;
	ofs.close();

	cout << "��¼�ѱ���" << endl;
	this->fileIsEmpty = false;
}

void SpeechManager::loadRecord() {
	ifstream ifs("SpeechRecord.csv", ios::in);

	if (!ifs.is_open()) {
		this->fileIsEmpty = true;
		cout << "�ļ������ڣ�" << endl;
		ifs.close();
		return;
	}

	char iseof;
	ifs >> iseof;  /*��һ���ַ���ָ����ƲŻᵽ������*/
	if (ifs.eof()) {
		this->fileIsEmpty = true;
		cout << "�ļ�Ϊ�գ�" << endl;
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
			if (position == -1) {  /*�����쳣*/
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
		cout << "�ļ������ڻ�Ϊ��" << endl;
	}
	else
	{
		string awardType[] = { "�ھ�","�Ǿ�","����" };
		for (int i = 0; i < this->Record.size(); i++)
		{
			cout << "��" << i + 1 << "�� " << endl;
			for (int n = 0; n <= 6; n += 3)
			{
				cout << awardType[n / 3] << "��ţ�" << this->Record[i][n]
					<< " ����: " << this->Record[i][n + 1]
					<< " �÷֣�" << this->Record[i][n + 2] << " " << endl;
			}
		}
	}
	system("pause");
	system("cls");
}

void SpeechManager::clearRecord() {
	char choice;
	cout << "ȷ����գ�(y/n)" << endl;
	cin >> choice;
	if ((choice == 'y') || (choice == 'Y')) {
		ofstream ofs("SpeechRecord.csv", ios::trunc);
		ofs.close();

		//��ʼ������
		this->initPlayers();
		this->creatPlayers();
		this->loadRecord();
		cout << "��ճɹ���" << endl;
	}
}

void SpeechManager::exitSystem() {
	cout << "��лʹ�ã��ټ���" << endl;
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
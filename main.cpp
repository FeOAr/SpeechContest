#include<iostream>
#include"speechmanager.h"
using namespace std;

int main() {
	SpeechManager SMObject;
	int choice = 0;
	while (true)
	{
		SMObject.showMenu();
		cout << "请输入选择：" << endl;
		cin >> choice;
		switch (choice)
		{
		case 1:  /*开始比赛*/
			SMObject.startSpeech();
			break;
		case 2:  /*查看记录*/
			SMObject.displayRecord();
			break;
		case 3:  /*清空记录*/
			SMObject.clearRecord();
			break;
		case 0:  /*退出*/
			SMObject.exitSystem();
			break;
		default:
			system("cls");
			break;
		}
	}
	return 0;
}
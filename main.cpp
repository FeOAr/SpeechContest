#include<iostream>
#include"speechmanager.h"
using namespace std;

int main() {
	SpeechManager SMObject;
	int choice = 0;
	while (true)
	{
		SMObject.showMenu();
		cout << "������ѡ��" << endl;
		cin >> choice;
		switch (choice)
		{
		case 1:  /*��ʼ����*/
			SMObject.startSpeech();
			break;
		case 2:  /*�鿴��¼*/
			SMObject.displayRecord();
			break;
		case 3:  /*��ռ�¼*/
			SMObject.clearRecord();
			break;
		case 0:  /*�˳�*/
			SMObject.exitSystem();
			break;
		default:
			system("cls");
			break;
		}
	}
	return 0;
}
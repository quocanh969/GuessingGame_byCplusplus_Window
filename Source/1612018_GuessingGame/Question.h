#pragma once
#include "stdafx.h"

class Question
{
	public:
	int isPicked = 0;
	WCHAR ans1[20], ans2[20];
	WCHAR Hint[255];
	WCHAR trueAns[20];
	WCHAR imgName[255];

};

void catWCHAR(WCHAR a[])
{
	// Tìm độ dài
	int dem = 0;
	while (a[dem] != 0)
	{
		dem++;
	}
	WCHAR b[255];
	wcscpy(b, a);
	wcscpy(a, L"source\\");
	for (int i = 0; i <= dem; i++)
	{
		a[i + 7] = b[i];
	}
}

void initQuestionData(Question quizToAsk[])
{
	freopen("src.txt", "rt", stdin);
	vector<Question> arr;
	
	
	while(!wcin.eof())
	{
		//cin >> temp.imgName;
		/*getline(wcin, temp.imgName);
		temp.imgName = L"source\\"+temp.imgName;
		
		Question temp;
		getline(wcin, temp.ans1);
		getline(wcin, temp.ans2);
		getline(wcin, temp.Hint);
		getline(wcin, temp.trueAns);*/
		Question temp;

		wcin.getline(temp.imgName, 255);
		
		catWCHAR(temp.imgName);

		wcin.getline(temp.ans1, 20);
		wcin.getline(temp.ans2, 20);
		wcin.getline(temp.Hint, 255);
		wcin.getline(temp.trueAns, 255);


		arr.push_back(temp);
	}

	int stt = 0, i = 0;
	
	while(i < 10)
	{
		srand(time(NULL));
		stt = rand() % arr.size();

		if (arr[stt].isPicked == 0)
		{
			quizToAsk[i] = arr[stt];
			i++;
			arr[stt].isPicked = 1;
		}
	}

	fclose(stdin);
}
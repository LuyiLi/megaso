#include "pch.h"
#include "pocket.h"
#include <stdio.h>

pocket::pocket()
{
}
void pocket::pocketGenerate()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			pocketData[j][i] = 0;
		}
	}
}

int pocket::checkIfExist()
{
	FILE *fp;
	fopen_s(&fp, "pocket.txt", "r");
	if (fp == NULL)
	{
		return 0;
	}
	else
	{
		fclose(fp);
		return 1;
	}
}

void pocket::pocketUpdate()
{
	for (int i = 0; i < 10; i++)
	{
	
		if (pocketData[0][i] == 0 || pocketData[1][i] == 0)
		{
			pocketData[0][i] = 0;
			pocketData[1][i] = 0;
		}
	}
}

void pocket::pocketWrite(int targrtPocket[2][10])
{
	FILE *fp;
	fopen_s(&fp, "pocket.txt", "w");
	int m = 0, n = 0;
	int num = 0;
	for (m; m < 10; m++)
	{
		for (n; n < 2; n++)
		{
			num = targrtPocket[n][m];
			fprintf(fp, "%d,", num);
		}
		fprintf(fp, "R,");
		n = 0;
	}
	m = 0;
	fprintf(fp, "E");
	fclose(fp);
}

void pocket::pocketRead()
{
	FILE *fp;
	fopen_s(&fp, "pocket.txt", "r");
	char presentChar;
	int targetNum = 0;
	int num = 0;
	if (fp != NULL)
	{
		int m = 0, n = 0;
		int negativeFlag = 0;
		while ((presentChar = fgetc(fp)) != EOF)
		{
			if (presentChar == 'E')
			{
				fclose(fp);
				return;
			}
			else
			{
				if (presentChar != ',')
				{

					if (presentChar == '-')
					{
						negativeFlag = 1;
					}
					if ((int)presentChar >= 48 && (int)presentChar <= 57)
					{
						targetNum = targetNum * 10 + (int)presentChar - 48;
					}
					if (presentChar == 'R')
					{
						fgetc(fp);
						n = 0;
						m++;
					}
				}
				else if (presentChar == ',')
				{
					if (negativeFlag)
					{
						targetNum = targetNum * (-1);
					}
					negativeFlag = 0;
					pocketData[n][m] = targetNum;
					n++;
					targetNum = 0;
				}
			}
		}
		fclose(fp);
	}
}

pocket::~pocket()
{
}

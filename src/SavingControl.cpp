#include "pch.h"
#include <stdio.h>
#include "SavingControl.h"


SavingControl::SavingControl()
{
}

void SavingControl::fileWrite(int* data)
{
	FILE *fp;
	fopen_s(&fp,"saving.txt", "w");
	int num = 0;
	int i = data[num];
	while (data[num])
	{
		i = data[num];
		fprintf(fp, "%d,", i);
		num++;
	}
	fprintf(fp, "E");
	fclose(fp);
}

void SavingControl::fileRead(int* target)
{
	FILE *fp;
	fopen_s(&fp, "saving.txt", "r");
	char presentChar;
	int targetNum = 0;
	int num = 0;
	if (fp != NULL) 
	{
		int negativeFlag = 0;
		while ((presentChar = fgetc(fp))!=EOF)
		{
			printf("%c", presentChar);
			if (presentChar == 'E')
			{
				fclose(fp);
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
					
				}
				else if (presentChar == ',')
				{
					if (negativeFlag)
					{
						targetNum = targetNum * (-1);
					}
					negativeFlag = 0;
					target[num++] = targetNum;
					targetNum = 0;
				}

			}
		}
		fclose(fp);
		printf("\n%d %d\n", target[0], target[1]);
	}
}

SavingControl::~SavingControl()
{
}

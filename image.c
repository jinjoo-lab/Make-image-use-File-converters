#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
typedef struct Rgb
{
	int red;
	int green;
	int blue;
}rgb; // RGB ����ü
int** inarray; // .dat������ �Էµ����͸� ������ 2���� �迭(2���� ������ ����)
rgb** outarray; // ��� rgb���� ������ 2���� �迭(2���� ������ ����)  
void readRowCol(char* string,int* row,int* col); // �Է� ���ڿ����� ��� ���� �����ϴ� �Լ�
void readData(char* string,int s_row,int s_col); // .dat ���Ͽ��� ���� �����͸� �Է� �޾� inarray�迭�� �����ϴ� �Լ�
void findMaxMin(int* max,int* min,int s_row,int s_col); // 2���� �迭 inarray���� �ִ񰪰� �ּڰ��� ã�� �Լ�
void loadGreyscale(int* max,int* min,int s_row,int s_col); // 2���� �迭 outarray�� ����� rgb���� �Է��ϴ� �Լ�
void outputimage(char* string,int s_row,int s_col); // .ppm ���Ͽ� �ʿ��� ������ loadGreyscale�Լ��� ������� �����ϴ� �Լ�
int findNint(char* string);// ���ڿ��� ���� �������� �ƴ��� �Ǵ��ϴ� �Լ�
int main(void)
{
	int size_row; // �迭�� ���� �� ����
	int size_col; // �迭�� ���� �� ����
	int max=0,min=0; // �Էµ������� �ִ񰪰� �ּڰ��� ������ ����
	char Name[100]; // �Է¹޴� ���������� ���ڿ��� ������ �迭
	printf("���� �̸��� �Է��ϼ���(��: map-input-��-��.dat):");
	gets(Name); // �Է¹��� ���ڿ��� �迭 Name�� ����
	readRowCol(Name,&size_row,&size_col); // ��� ���� �б�
	readData(Name,size_row,size_col); // 2���� �迭 ����, ������ �б�
	findMaxMin(&max,&min,size_row,size_col); // �Է� ���� �����Ϳ��� �ִ񰪰� �ּڰ� ����
	loadGreyscale(&max,&min,size_row,size_col); // �Է� ������ �����͵��� rgb ����� outarray�迭�� ����
	outputimage(Name,size_row,size_col); // �ʿ��� �����͵�� ������ ��ȯ�� rgb������ .ppm ���Ͽ� ����
	return 0;
}
void readRowCol(char* string,int* row,int* col)
{
	sscanf(string,"map-input-%d-%d.dat",col,row); // �Է¹��� ���ڿ����� row,col���� ���� �����Ѵ�
	if(*row<0||*col<0)
	{
		printf("Error : Problem reading in rows and colums\n");
		exit(-1);
	} // �Է¹��� row�� col�� (����� �ƴ�)�߸��� ������ ��� error �޼��� ��� �� ���α׷� ���� ����
	printf("���� ũ��: %d ���� ũ��: %d\n",*row,*col);
}
void readData(char* string,int s_row,int s_col)
{
	char* file_name=(char*)malloc(strlen(string)); // char�� ������ file_name ������ �Է¹��� ���ڿ��� ũ�⸸ŭ ���� �޸� �Ҵ�
	FILE* stream;// ���� ��Ʈ�� ����
	int i=0,j=0,over_val=0; // i�� j�� 2���� �迭�� �����͸� �����ϱ� ���� ���Ǵ� ����, over_val�� ���Ͽ� ����� �����Ͱ� �Է¹���(row*col)��ĺ��� ������� �Ǵ��� ���� ����
	char input[20]; // ������ �����͸� ó�� ���ڿ� ���·� �Է¹ޱ� ���� ����ϴ� �迭
	strcpy(file_name,string); // �Է¹��� ���ڿ� string�� ���� file_name�� ����
	
	stream=fopen(file_name,"r"); // �Է¹��� file_name�� ������ r���� ����
	if(stream==NULL)// ������ ���� fopen�� ������ NULL���� ��µǴ� ���
	{
		printf("Error : Unable to open file %s\n",file_name);
		exit(-1);
	} // ������ ���� ���ٴ� Error �޼��� ����� ���α׷� ��������

	inarray=(int**)malloc(s_row*sizeof(int*)); // ������ ���� �����͸� ������ 2���� �迭�� ���� �����޸��Ҵ�(row��ŭ)
	for(i=0;i<s_row;i++) // ���Ͽ� ���� �����͸� ������ 2���� �迭�� ���� �����޸��Ҵ�(col��ŭ)
	{
		inarray[i]=(int*)malloc(sizeof(int)*s_col);
	} // 2���� �迭 ����

	for(i=0;i<s_row;i++)
	{
		for(j=0;j<s_col;j++) // 2���� �迭�� ũ�� ��ŭ �ݺ��� ����
		{
			if(fscanf(stream,"%s",input)==EOF) // ������ �����͸� �迭 input�� ���ڿ� ���·� �Է¹޴´�.
			// �� �������� �迭�� ũ�⺸�� ���� �����Ͱ� ���Ͽ� ����Ǿ��־� �ݺ����� ������ ���� ������ ���� ������ EOF�� ��ȯ�� ���
			{
				printf("Error : End of file reached prior to getting all the required data\n");
				exit(-1);
			}// Error �޼��� ����� ���α׷� ���� ����
			else if(!findNint(input)) // �Է¹��� ���ڿ��� �����Ͱ� (����)���������� ���� �ƴҰ��
			{
				printf("Error : Read a non-integer value\n");
				exit(-1);
			} // Error �޼��� ����� ���α׷� ���� ����
			else
				inarray[i][j]=atoi(input); // �ùٸ� ���� ������ �����Ͱ� �Էµ� ��� ���ڿ��� ������ ��ȯ�� �Է� �迭(inarray)�� ����
		}
	}
	if(fscanf(stream,"%d",&over_val)!=EOF) // �迭�� ũ�⸸ŭ �ݺ��� ������ ���� over_val�� �Ѱ��� �����͸� �� �Է¹޴´�
	// �� ���� ������ ���� ������ EOF�� ��ȯ���� �ʴ� ��� ���Ͽ� �迭�� ũ�� ���� ���� �����Ͱ� ����Ǿ� �ֱ� ������
	{
		printf("Error : Too many data points\n");
		exit(-1);
	} // error �޼��� ����� ���α׷� ���� ����
	fclose(stream); // ���Ͻ�Ʈ���� �ݴ´�
}
void findMaxMin(int* max,int* min,int s_row,int s_col)
{
	int i=0,j=0; // �ݺ����� ������ ����
	*max=inarray[0][0]; // �Է¹��� �������� �ִ� �ʱ�ȭ
	*min=inarray[0][0]; // �Է¹��� �������� �ּڰ� �ʱ�ȭ
	for(i=0;i<s_row;i++)
	{
		for(j=0;j<s_col;j++) // �迭�� ũ�� ��ŭ �ݺ��� ����
		{
			if(inarray[i][j]>*max) // ���� �迭�� ����� ���� max���� Ŭ ���
				*max=inarray[i][j]; // �� ���� max�� ����
			if(inarray[i][j]<*min) // ���� �迭�� ����� ���� min���� ���� ���
				*min=inarray[i][j]; // �� ���� min�� ����
		}
	}
	printf("max:%d min:%d\n",*max,*min); // max�� min���� ����Ͽ� ����ڷ� �Ͽ��� ���� ����
}
void loadGreyscale(int* max,int* min,int s_row,int s_col)
{
	int i,j; // �ݺ����� ������ ����
	int mother; // shade of grey�� ����Ҷ� ����� �м��� �и�
	double shade; // r,g,b�� ����� ��(�Ǽ������� ����)
	mother=*max-*min; // �и� �ִ񰪿��� �ּڰ��� �� �� ����

	outarray=(rgb**)malloc(sizeof(rgb)*s_row); // r,g,b �����͸� ������ 2���� �迭�� ���� �����޸��Ҵ�(row��ŭ)
	for(i=0;i<s_row;i++) //r,g,b �����͸� ������ 2���� �迭�� ���� �����޸��Ҵ�(col��ŭ)
	{
		outarray[i]=(rgb*)malloc(sizeof(rgb)*s_col);
	} // rgb����ü �ڷ����� 2���� �迭 ����
	for(i=0;i<s_row;i++)
	{
		for(j=0;j<s_col;j++) // �迭�� ũ�� ��ŭ �ݺ��� ����
		{
			shade=((double)(inarray[i][j] - *min)/mother)*255; // shade of grey �Ŀ� ���� �����Ͽ� shade ���
			outarray[i][j].blue=shade; // shade�� ���� �迭(outarray)�� r,g,b�� ���� ����
			outarray[i][j].green=shade;// shade�� ���� �迭(outarray)�� r,g,b�� ���� ����
			outarray[i][j].red=shade;// shade�� ���� �迭(outarray)�� r,g,b�� ���� ����
		}
	}
}
void outputimage(char* string,int s_row,int s_col)
{
	FILE* stream2; // ���Ͻ�Ʈ�� ����
	char* out_name; // ������ ������ ����(���ڿ�)�� ������ ������ ����
	int i,j; // �ݺ��� ������ ���� ����
	out_name=string; // �Է¹��� ���ڿ��� out_name�� ����
	strcat(out_name,".ppm");// out_name�� .ppm�̶�� ������ ���¸� ��Ÿ���� ���ڿ��� ����
	
	stream2=fopen(out_name,"w"); // .ppm������ ����
	fprintf(stream2,"P3"); // �̹��� ����� ���� .ppm ���Ͽ� �ʿ��� ��� �Է� 
	fprintf(stream2,"\n");
	fprintf(stream2,"%d %d",s_col,s_row);// �̹��� ����� ���� .ppm ���Ͽ� �ʿ��� ��� �Է�(�迭�� ���� ��)
	fprintf(stream2,"\n");
	fprintf(stream2,"255");// �̹��� ����� ���� .ppm ���Ͽ� �ʿ��� ��� �Է�
	fprintf(stream2,"\n");

	for(i=0;i<s_row;i++)
	{
		for(j=0;j<s_col;j++) // �迭�� ũ�⸸ŭ �ݺ��� ����
		{
			fprintf(stream2,"%d %d %d ",outarray[i][j].blue,outarray[i][j].green,outarray[i][j].red);
		} // ����� ���� rgb�� ���� ���Ͽ� ���
	}
	fclose(stream2); // ������ �ݴ´�
}
int findNint(char* string)
{
	int i; // �ݺ����� ������ ���� 
	for(i=0;i<strlen(string);i++) // �Է¹��� ���ڿ��� ũ�⸸ŭ �ݺ��� ����
	{
		if(string[0]=='-') // ������ �����Ͱ� ó���� ���� 0��° �ε����� ���ڰ� '-'�ϰ�� 
			i++; // �״��� �ε������� �˻��ϵ��� i�� �ϳ��� ����

		if(!isdigit(string[i])) // ���ڿ��� ���� �ϳ��ϳ��� ���������� �ƴ��� �Ǵ�
			return 0; //  �������� �ƴ϶�� 0����
	}
	return 1; // �������� ������ ������ ��� 1����
}
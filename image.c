#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
typedef struct Rgb
{
	int red;
	int green;
	int blue;
}rgb; // RGB 구조체
int** inarray; // .dat파일의 입력데이터를 저장할 2차원 배열(2차원 포인터 형태)
rgb** outarray; // 출력 rgb값을 저장할 2차원 배열(2차원 포인터 형태)  
void readRowCol(char* string,int* row,int* col); // 입력 문자열에서 행과 열을 추출하는 함수
void readData(char* string,int s_row,int s_col); // .dat 파일에서 정수 데이터를 입력 받아 inarray배열에 저장하는 함수
void findMaxMin(int* max,int* min,int s_row,int s_col); // 2차원 배열 inarray에서 최댓값과 최솟값을 찾는 함수
void loadGreyscale(int* max,int* min,int s_row,int s_col); // 2차원 배열 outarray에 계산한 rgb값을 입력하는 함수
void outputimage(char* string,int s_row,int s_col); // .ppm 파일에 필요한 정보와 loadGreyscale함수의 결과값을 저장하는 함수
int findNint(char* string);// 문자열의 값이 정수인지 아닌지 판단하는 함수
int main(void)
{
	int size_row; // 배열의 행의 값 변수
	int size_col; // 배열의 열의 값 변수
	int max=0,min=0; // 입력데이터의 최댓값과 최솟값을 저장할 변수
	char Name[100]; // 입력받는 파일제목의 문자열을 저장할 배열
	printf("파일 이름을 입력하세요(예: map-input-열-행.dat):");
	gets(Name); // 입력받은 문자열을 배열 Name에 저장
	readRowCol(Name,&size_row,&size_col); // 행과 열값 읽기
	readData(Name,size_row,size_col); // 2차원 배열 생성, 데이터 읽기
	findMaxMin(&max,&min,size_row,size_col); // 입력 받은 데이터에서 최댓값과 최솟값 추출
	loadGreyscale(&max,&min,size_row,size_col); // 입력 파일의 데이터들을 rgb 계산후 outarray배열에 저장
	outputimage(Name,size_row,size_col); // 필요한 데이터들과 위에서 변환한 rgb값들을 .ppm 파일에 저장
	return 0;
}
void readRowCol(char* string,int* row,int* col)
{
	sscanf(string,"map-input-%d-%d.dat",col,row); // 입력받은 문자열에서 row,col값을 따로 저장한다
	if(*row<0||*col<0)
	{
		printf("Error : Problem reading in rows and colums\n");
		exit(-1);
	} // 입력받은 row와 col이 (양수가 아닌)잘못된 형태일 경우 error 메세지 출력 후 프로그램 강제 종료
	printf("행의 크기: %d 열의 크기: %d\n",*row,*col);
}
void readData(char* string,int s_row,int s_col)
{
	char* file_name=(char*)malloc(strlen(string)); // char형 포인터 file_name 선언후 입력받은 문자열의 크기만큼 동적 메모리 할당
	FILE* stream;// 파일 스트림 생성
	int i=0,j=0,over_val=0; // i와 j는 2차원 배열에 데이터를 저장하기 위해 사용되는 변수, over_val은 파일에 저장된 데이터가 입력받은(row*col)양식보다 많은경우 판단을 위한 변수
	char input[20]; // 파일의 데이터를 처음 문자열 형태로 입력받기 위해 사용하는 배열
	strcpy(file_name,string); // 입력받은 문자열 string의 값을 file_name에 복사
	
	stream=fopen(file_name,"r"); // 입력받은 file_name의 파일을 r모드로 연다
	if(stream==NULL)// 파일을 여는 fopen이 실패해 NULL값이 출력되는 경우
	{
		printf("Error : Unable to open file %s\n",file_name);
		exit(-1);
	} // 파일을 열수 없다는 Error 메세지 출력후 프로그램 강제종료

	inarray=(int**)malloc(s_row*sizeof(int*)); // 파일의 정수 데이터를 저장할 2차원 배열을 위한 동적메모리할당(row만큼)
	for(i=0;i<s_row;i++) // 파일에 정수 데이터를 저장할 2차원 배열을 위한 동적메모리할당(col만큼)
	{
		inarray[i]=(int*)malloc(sizeof(int)*s_col);
	} // 2차원 배열 생성

	for(i=0;i<s_row;i++)
	{
		for(j=0;j<s_col;j++) // 2차원 배열의 크기 만큼 반복문 실행
		{
			if(fscanf(stream,"%s",input)==EOF) // 파일의 데이터를 배열 input에 문자열 형태로 입력받는다.
			// 그 과정에서 배열의 크기보다 적은 데이터가 파일에 저장되어있어 반복문이 끝나기 전에 파일의 끝에 도달해 EOF를 반환한 경우
			{
				printf("Error : End of file reached prior to getting all the required data\n");
				exit(-1);
			}// Error 메세지 출력후 프로그램 강제 종료
			else if(!findNint(input)) // 입력받은 문자열의 데이터가 (정수)숫자형태의 값이 아닐경우
			{
				printf("Error : Read a non-integer value\n");
				exit(-1);
			} // Error 메세지 출력후 프로그램 강제 종료
			else
				inarray[i][j]=atoi(input); // 올바른 정수 형태의 데이터가 입력된 경우 문자열을 정수로 변환후 입력 배열(inarray)에 저장
		}
	}
	if(fscanf(stream,"%d",&over_val)!=EOF) // 배열의 크기만큼 반복문 실행후 변수 over_val에 한개의 데이터를 더 입력받는다
	// 그 값이 파일의 끝에 도달한 EOF가 반환되지 않는 경우 파일에 배열의 크기 보다 많은 데이터가 저장되어 있기 때문에
	{
		printf("Error : Too many data points\n");
		exit(-1);
	} // error 메세지 출력후 프로그램 강제 종료
	fclose(stream); // 파일스트림을 닫는다
}
void findMaxMin(int* max,int* min,int s_row,int s_col)
{
	int i=0,j=0; // 반복문을 실행할 변수
	*max=inarray[0][0]; // 입력받은 데이터의 최댓값 초기화
	*min=inarray[0][0]; // 입력받은 테이터의 최솟값 초기화
	for(i=0;i<s_row;i++)
	{
		for(j=0;j<s_col;j++) // 배열의 크기 만큼 반복문 실행
		{
			if(inarray[i][j]>*max) // 만약 배열에 저장된 값이 max보다 클 경우
				*max=inarray[i][j]; // 그 값을 max에 저장
			if(inarray[i][j]<*min) // 만약 배열에 저장된 값이 min보다 작을 경우
				*min=inarray[i][j]; // 그 값을 min에 저장
		}
	}
	printf("max:%d min:%d\n",*max,*min); // max와 min값을 출력하여 사용자로 하여금 정보 전달
}
void loadGreyscale(int* max,int* min,int s_row,int s_col)
{
	int i,j; // 반복문을 실행할 변수
	int mother; // shade of grey를 계산할때 사용할 분수의 분모
	double shade; // r,g,b에 저장될 값(실수형으로 선언)
	mother=*max-*min; // 분모에 최댓값에서 최솟값을 뺀 값 저장

	outarray=(rgb**)malloc(sizeof(rgb)*s_row); // r,g,b 데이터를 저장할 2차원 배열을 위한 동적메모리할당(row만큼)
	for(i=0;i<s_row;i++) //r,g,b 데이터를 저장할 2차원 배열을 위한 동적메모리할당(col만큼)
	{
		outarray[i]=(rgb*)malloc(sizeof(rgb)*s_col);
	} // rgb구조체 자료형의 2차원 배열 생성
	for(i=0;i<s_row;i++)
	{
		for(j=0;j<s_col;j++) // 배열의 크기 만큼 반복문 실행
		{
			shade=((double)(inarray[i][j] - *min)/mother)*255; // shade of grey 식에 값을 대입하여 shade 계산
			outarray[i][j].blue=shade; // shade의 값을 배열(outarray)의 r,g,b에 각각 저장
			outarray[i][j].green=shade;// shade의 값을 배열(outarray)의 r,g,b에 각각 저장
			outarray[i][j].red=shade;// shade의 값을 배열(outarray)의 r,g,b에 각각 저장
		}
	}
}
void outputimage(char* string,int s_row,int s_col)
{
	FILE* stream2; // 파일스트림 생성
	char* out_name; // 생성할 파일의 제목(문자열)을 저장할 포인터 변수
	int i,j; // 반복문 실행을 위한 변수
	out_name=string; // 입력받은 문자열을 out_name에 저장
	strcat(out_name,".ppm");// out_name에 .ppm이라는 파일의 형태를 나타내는 문자열을 결합
	
	stream2=fopen(out_name,"w"); // .ppm파일을 연다
	fprintf(stream2,"P3"); // 이미지 출력을 위해 .ppm 파일에 필요한 양식 입력 
	fprintf(stream2,"\n");
	fprintf(stream2,"%d %d",s_col,s_row);// 이미지 출력을 위해 .ppm 파일에 필요한 양식 입력(배열의 열과 행)
	fprintf(stream2,"\n");
	fprintf(stream2,"255");// 이미지 출력을 위해 .ppm 파일에 필요한 양식 입력
	fprintf(stream2,"\n");

	for(i=0;i<s_row;i++)
	{
		for(j=0;j<s_col;j++) // 배열의 크기만큼 반복문 실행
		{
			fprintf(stream2,"%d %d %d ",outarray[i][j].blue,outarray[i][j].green,outarray[i][j].red);
		} // 계산이 끝난 rgb의 값을 파일에 출력
	}
	fclose(stream2); // 파일을 닫는다
}
int findNint(char* string)
{
	int i; // 반복문을 실행할 변수 
	for(i=0;i<strlen(string);i++) // 입력받은 문자열의 크기만큼 반복문 실행
	{
		if(string[0]=='-') // 음수의 데이터값 처리를 위해 0번째 인덱스의 문자가 '-'일경우 
			i++; // 그다음 인덱스부터 검사하도록 i값 하나씩 증가

		if(!isdigit(string[i])) // 문자열의 문자 하나하나가 십진수인지 아닌지 판단
			return 0; //  십진수가 아니라면 0리턴
	}
	return 1; // 정상적인 숫자의 형태일 경우 1리턴
}
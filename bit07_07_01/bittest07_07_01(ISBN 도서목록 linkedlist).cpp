#pragma warning(disable:4996)
#include <stdio.h>
#include <iostream>
#include <malloc.h>
#include <windows.h>
#include <ctype.h>

typedef struct _sData{
	char * pData;
	struct _sData * pNext;
}sData;
typedef struct _ISBN
{
	char * Title;
	char * Company;
	char * Price;
	int WriterNum;
	int TableNum;
	struct _sData * pWriter;
	struct _sData * pTable;
	struct _ISBN * pNext;
	struct _ISBN * pPre;
}ISBN;
typedef struct _sSearch
{
	_ISBN *pSData;
	_ISBN * pPre;
	struct _sSearch * pNext;
}sSearch;
void NewBookData(char* * pData, char* PrintMessage){
	char temp[1024];
	printf("%s",PrintMessage);
	gets(temp);
	fflush(stdin);
	*pData = (char *)malloc(strlen(temp) + 1);
	strcpy(*pData, temp);
}
int NewBookDataWiter(char* *pData, char* PrintMessage){
	char temp[1024];
	for (int iCurrentBookNum = 0; iCurrentBookNum < 5; iCurrentBookNum++){
		printf(" %d. %s",iCurrentBookNum+1, PrintMessage);
		gets(temp);
		fflush(stdin);
		if (temp[0]=='\0'){
			return iCurrentBookNum;
		}
		*pData = (char *)malloc(strlen(temp) + 1);
		strcpy(*pData, temp);
		memset(temp,NULL,1024);
	}
	return 5;
}
int NewBookDataTable(char* *pData, char* PrintMessage){
	char temp[1024];
	for (int iCurrentBookNum = 0; iCurrentBookNum < 20; iCurrentBookNum++){
		printf(" %d. %s", iCurrentBookNum + 1, PrintMessage);
		gets(temp);
		fflush(stdin);
		if (temp[0] == '\0'){
			return iCurrentBookNum;
		}
		*pData = (char *)malloc(strlen(temp) + 1);
		strcpy(*pData, temp);
		memset(temp, NULL, 1024);
	}
	return 20;
}
/*
void KillData(ISBN * BookArray){
	BookArray->Title = NULL;
	for (int i = 0; i < BookArray->WriterNum; i++){
		BookArray->Writer[i]=NULL;
	}
	BookArray->Company = NULL;
	BookArray->Price = NULL;
	for (int i = 0; i < BookArray->TableNum; i++){
		BookArray->Table[i] = NULL;
	}
}*/

void InputBookData(ISBN* *pNode){ 
	NewBookData(&(*pNode)->Title, "제목 : ");
	(*pNode)->WriterNum = NewBookDataWiter(&(*pNode)->pWriter->pData, "저자 : ");
	NewBookData(&(*pNode)->Company, "출판사 :");
	NewBookData(&(*pNode)->Price, "가격 :");
	(*pNode)->TableNum = NewBookDataTable(&(*pNode)->pTable->pData, "목차 :");
}
void Insert(ISBN* pNode, ISBN* *pTop){
	pNode->pNext = NULL;
	if (*pTop == NULL)
	{
		*pTop = pNode;
	}
	else{
		ISBN * pStart = *pTop;
		while (pStart->pNext != NULL)//연결
		{
			pStart = pStart->pNext;
		}
		pStart->pNext = pNode;
	}
}
void MultiInsert(sSearch* pNode, sSearch* *pTop){
	pNode->pNext = *pTop;
	*pTop = pNode;
}
void AddBook(ISBN * *pTop ){
	ISBN * pNode = (ISBN *)malloc(sizeof(ISBN));
	pNode->pWriter = (struct _sData *)malloc(sizeof(_sData));
	pNode->pTable = (struct _sData *)malloc(sizeof(_sData));
	InputBookData(&pNode);
	Insert(pNode,pTop);
}
void Print(ISBN * pStart){
	printf("도서 명 : %s\n", pStart->Title);
	printf("저자 :\n");
	for (int i= 0; i < pStart->WriterNum; i++){
		printf("%d.%s ", i+1, pStart->pWriter->pData);
	}
	printf("\n출판사 : %s\n", pStart->Company);
	printf("가격 : %s\n", pStart->Price);
	printf("목차 :\n");
	for (int i = 0; i < pStart->TableNum; i++){
		printf(" %d. %s\n", i+1, pStart->pTable->pData);
	}
	printf("\n");
}
void PrintBookList(ISBN* pTop){
	ISBN * pStart = pTop;
	while(pStart != NULL){
		Print(pStart);
		pStart = pStart->pNext;
	}
}
int SSearch(ISBN* pStart, char* SearchBookdata){
	int count = 0;
	while (pStart!=NULL){
		if (strcmp(SearchBookdata,pStart->Title) == 0){
			return 0;
		}
		pStart = pStart->pNext;
		count++;
	}
	if (count == 0){
		return 1;
	}
	return 2;

}
int MSearchW(ISBN* pStart,sSearch* *pCTop, char* SearchBookdata){
	//Bookdata 저자 수 or 목차 수
	int count = 0;
	while (pStart != NULL){
		while (pStart->pWriter !=NULL){
			if (strstr(pStart->pWriter->pData, SearchBookdata) != 0){
				sSearch* pCopy = (sSearch*)malloc(sizeof(sSearch));
				pCopy->pSData = pStart;
				pCopy->pNext = NULL;
				pCopy->pPre = pStart->pPre;
				MultiInsert(pCopy, pCTop);
				count++;
			}
			pStart->pWriter = pStart->pWriter->pNext;
		}
		pStart = pStart->pNext;
	}
	if (count == 0){
		return 0;
	}
	else{
		return 2;
	}
}
int MSearchT(ISBN* pStart, sSearch* *pCTop, char* SearchBookdata){
	//Bookdata 저자 수 or 목차 수
	int count = 0;
	while (pStart != NULL){
		while (pStart->pWriter != NULL){
			if (strcmp(pStart->pWriter->pData, SearchBookdata) == 0){
				sSearch* pCopy = (sSearch*)malloc(sizeof(sSearch));
				pCopy->pSData = pStart;
				pCopy->pNext = NULL;
				pCopy->pPre = pStart->pPre;
				MultiInsert(pCopy, pCTop);
				count++;
			}
			pStart->pWriter = pStart->pWriter->pNext;
		}
		pStart = pStart->pNext;
	}
	if (count == 0){
		return 0;
	}
	else{
		return 2;
	}
}
void SearchBook(ISBN * pTop,  char* *SearchMsg){
	char Searchtype = '1';	//찾기할 방법 단일찾기 다중찾기
	char SearchBookdata[1024] ;		//찾을 책 정보
	int ReturnSearch = 0;			//결과에 따른 메시지 표현
	ISBN * pStart = pTop;			//현재보고있는 노드를 나타내는 포인터
	sSearch * pCTop = NULL;

	printf("검색 방법 선택(1.제목검색 , 2.저자검색, 3.목차검색, 0.검색취소: )");
	Searchtype = getchar();
	fflush(stdin);
	while (Searchtype != '0'){
		switch (Searchtype)
		{
		case '1':	
			printf("1. 제목검색\n 검색하고 싶은 도서 명 : ");
			gets(SearchBookdata);
			fflush(stdin);
			ReturnSearch = SSearch(pStart, SearchBookdata);
			if ( ReturnSearch == 0){
				Print(pStart);
			}
			Searchtype = '0';
			break;
		case '2':
			printf("2. 저자검색\n 검색하고 싶은 저자 :");
			gets(SearchBookdata);
			fflush(stdin);
			ReturnSearch = MSearchW(pStart,&pCTop, SearchBookdata);
			if (ReturnSearch == 0){
				while (pCTop !=NULL){
					Print(pCTop->pSData);
					pCTop = pCTop->pNext;
				}
			}
			Searchtype = '0';
			break;
		case '3':
			printf("3. 목차검색\n 검색하고 싶은 목차 :");
			gets(SearchBookdata);
			fflush(stdin);
			ReturnSearch = MSearchT(pStart,&pCTop ,SearchBookdata);
			if (ReturnSearch == 0){
				while (pCTop != NULL){ 
					Print(pCTop->pSData);
					pCTop = pCTop->pNext;
				}
			}
			Searchtype = '0';
			break;
		default:
			break;
		}
	}
	printf("%s", SearchMsg[ReturnSearch]);
	free(pCTop);
}/*
void ChangeBook(ISBN * BookArray, int BookNum, char* *SearchMsg,  char* *ChangeMsg){
	char SearchBookdata[128] = { ' ' };
	int SearchBookIndex = 0;
	printf("수정하고 싶은 도서 명 :");
	gets(SearchBookdata);
	fflush(stdin);
	int ReturnSearch = SSearch(BookArray, BookNum, SearchBookdata, &SearchBookIndex);
	printf("%s", SearchMsg[ReturnSearch]);
	if (ReturnSearch == 0){
		printf("수정할 정보 :");
		KillData(&BookArray[SearchBookIndex]);
		InputBookData(&BookArray[SearchBookIndex]);
	}
	printf("%s", ChangeMsg[ReturnSearch]);
}

void DeleteBook(ISBN * BookArray, int *BookNum, char* *DeleteMsg){
	char SearchBookdata[128] = { ' ' };
	int SearchBookIndex = 0;
	printf("수정하고 싶은 도서 명 :");
	gets(SearchBookdata);
	fflush(stdin);
	int ReturnSearch = SSearch(BookArray, *BookNum, SearchBookdata, &SearchBookIndex);
	if (ReturnSearch == 0){
		for (int i = SearchBookIndex; i < *BookNum; i++){
			KillData(&BookArray[i]);
			BookArray[i].Title = BookArray[i + 1].Title;
			for (int j = 0; j < BookArray[i].WriterNum; j++){
				BookArray[i].Writer[j] = BookArray[i + 1].Writer[j];
			}
			BookArray[i].Company = BookArray[i + 1].Company;
			BookArray[i].Price = BookArray[i + 1].Price;
			for (int j = 0; j < BookArray[i].TableNum; j++){
				BookArray[i].Table[j] = BookArray[i + 1].Table[j];
			}
		}
		*BookNum -= 1;
	}
	printf("%s", DeleteMsg[ReturnSearch]);
}
void FilePrint(ISBN* BookArray, int TotalBook, int Booknum){
	FILE *fp;
	printf("주소록을 저장합니다.\n");
	fp = fopen("ISBN.txt", "w");
	fprintf(fp, "%d\n", TotalBook);
	for (int index = 0; index < Booknum; index++)
	{
		fprintf(fp, "%s\n", BookArray[index].Title);
		for (int i = 0; i < BookArray[index].WriterNum; i++){
			fprintf(fp,"%s\n", BookArray[index].Writer[i]);
		}
		fprintf(fp, "%s\n", BookArray[index].Company);
		fprintf(fp, "%s\n", BookArray[index].Price);
		for (int j = 0; j < BookArray[index].TableNum;j++)
		{
			fprintf(fp, "%s\n", BookArray[index].Table[j]);
		}
	}
	fclose(fp);
}
char* FileDataScan(FILE * fp, char **pData){
	char temp[1024] = { " " };
	char* tester;
	tester = fgets(temp, 1024, fp);
	*pData = (char *)malloc(strlen(temp));
	strcpy(*pData, temp);
	(*pData)[strlen(temp) - 1] = NULL;
	return tester;
}
int FileDataIndex(FILE * fp, int *pData){
	fscanf(fp, "%d\n", *pData);
	return *pData;
}
void FileScan(ISBN* BookArray, int *Total, int *Booknum){
	FILE *fp;
	fp = fopen("ISBN.txt", "r");
	if (fp == NULL)
		printf("주소록이 없습니다.");
	else{
		printf("주소록을 읽어옵니다.\n");
		fscanf(fp, "%d\n", Total);
		int index = 0;
		free(BookArray);
		BookArray = (ISBN *)malloc(sizeof(user)* (*usermax));
		while (!feof(fp))
		{
			if (FileDataScan(fp, &arr[index].name) == NULL)break;
			FileDataScan(fp, &arr[index].address);
			FileDataScan(fp, &arr[index].phonenum);
			index++;

		}
		*count = index;
	}
	fclose(fp);
}*/
/*
void FileScan(ISBN* BookArray, int *TotalBook, int *BookNum){
	FILE *fp;
	fp = fopen("ISBN.txt", "r");
	if (fp == NULL)
		printf("도서목록이 없습니다.");
	else{
		printf("도서목록을 읽어옵니다.\n");
		fscanf(fp, "%d\n", TotalBook);
		int index = 0;
		BookArray[0].Title = NULL;
		free(BookArray);
		BookArray = (ISBN *)malloc(sizeof(BookArray)* (*TotalBook));
		while (!feof(fp))
		{
			if (FileDataScan(fp, &BookArray[index].Title) == NULL)break;
			fscanf(fp, "%d\n", &BookArray[index].WriterNum);
			for (int i = 0; i < BookArray[index].WriterNum; i++){
				FileDataScan(fp, &BookArray[index].Writer[i]);
			}
			FileDataScan(fp, &BookArray[index].Company);
			FileDataScan(fp, &BookArray[index].Price);
			fscanf(fp, "%d\n", &BookArray[index].TableNum);
			for (int i = 0; i < BookArray[index].TableNum; i++){
				FileDataScan(fp, &BookArray[index].Table[i]);
			}
			index++;

		}
		*BookNum = index;
	}
	fclose(fp);
}*/
void main(){
	char * SearchMsg[] = { "찾기성공", "삭제 등록된 데이터가 없습니다.", "찾으려는 데이터가 존재하지 않습니다." };
	char * ChangeMsg[] = { "수정성공", "등록된 데이터가 없습니다.", "찾으려는 데이터가 존재하지 않습니다." };
	char * DeleteMsg[] = { "삭제성공", "삭제 등록된 데이터가 없습니다.", "찾으려는 데이터가 존재하지 않습니다." };
	ISBN * pTop=NULL;
	char mode = '1';
	while (mode != '0'){
		printf("\nmode 선택 ( 1 = 입력 , 2 = 출력 , 3 = 단일검색, 4 = 수정(정확한 이름), 5 = 단일삭제(정확한 이름), 6 = 파일저장, 7 = 파일로드 0 = 종료) \n");
		mode = getchar();
		fflush(stdin);

		switch (mode)
		{
		case '1':
			AddBook(&pTop);
			break;

		case '2':
			PrintBookList(pTop);
			break;
		case '3':
			SearchBook(pTop, SearchMsg);
			break;
/*		case '4':
			ChangeBook(BookArray, BookNum, SearchMsg, ChangeMsg);
			break;
		case '5':
			DeleteBook(BookArray, &BookNum, DeleteMsg);
			break;
		case '6':
			FilePrint(BookArray, TotalBook, BookNum);
			break;
		case '7':
			FileScan(BookArray, &TotalBook, &BookNum);
			break;*/
		default:
			mode = '0';
		/*	for (int i= 0; i < TotalBook;i++)
			KillData(&BookArray[i]);*/
			break;
		}
	}
}
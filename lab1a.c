#include <stdio.h>
#include<string.h>
#include <ctype.h>

#define MAX 100
#define SUBJECTS 5

struct Student
{
 char id[20];
 char name[30];
 int minor[SUBJECTS], major[SUBJECTS];
 int totSub[SUBJECTS];
 int tot;
 float pct;
 char grd[3];
 float cg;
};

int validID(char id[]){
int i;
for(i=0;id[i];i++)
 if(!isalnum(id[i])) return 0;
return 1;
}

int dupID(struct Student st[],int c,char id[])
{
 int i;
 for(i=0;i<c;i++)
  if(strcmp(st[i].id,id)==0) return 1;
 return 0;
}

int validNm(char nm[]){
 int i;
 for(i=0;nm[i];i++)
  if(!isalpha(nm[i])) return 0;
 return 1;
}

int vMin(int m){ if(m>=0 && m<=40) return 1; else return 0; }
int vMaj(int m)
{
 if(m>=0 && m<=60) return 1;
 return 0;
}

void calcRes(struct Student *s)
{
 int i,flg=1;
 s->tot=0;

 for(i=0;i<SUBJECTS;i++){
  s->totSub[i]=s->minor[i]+s->major[i];
  s->tot+=s->totSub[i];
  if(s->totSub[i]<50) flg=0;
 }

 s->pct = s->tot/(float)SUBJECTS;

 if(!flg) strcpy(s->grd,"F");
 else if(s->pct>=90) strcpy(s->grd,"O");
 else if(s->pct>=85) strcpy(s->grd,"A+");
 else if(s->pct>=75) strcpy(s->grd,"A");
 else if(s->pct>=65) strcpy(s->grd,"B+");
 else if(s->pct>=60) strcpy(s->grd,"B");
 else if(s->pct>=55) strcpy(s->grd,"C");
 else if(s->pct>=50) strcpy(s->grd,"D");
 else strcpy(s->grd,"F");

 s->cg=s->pct/10;
}

void showRpt(struct Student s[],int n){
 int i;
 printf("\nID\tName\tTot\tPct\tGrd\tCG\n\n");
 for(i=0;i<n;i++)
 printf("%s\t%s\t%d\t%.2f\t%s\t%.2f\n",
 s[i].id,s[i].name,s[i].tot,
 s[i].pct,s[i].grd,s[i].cg);
}

void stats(struct Student s[],int n)
{
 int i;
 float sm=0,mx=s[0].pct,mn=s[0].pct;
 int gCnt[8]={0};

 for(i=0;i<n;i++){
  sm+=s[i].pct;
  if(s[i].pct>mx) mx=s[i].pct;
  if(s[i].pct<mn) mn=s[i].pct;

  if(strcmp(s[i].grd,"O")==0) gCnt[0]++;
  else if(strcmp(s[i].grd,"A+")==0) gCnt[1]++;
  else if(strcmp(s[i].grd,"A")==0) gCnt[2]++;
  else if(strcmp(s[i].grd,"B+")==0) gCnt[3]++;
  else if(strcmp(s[i].grd,"B")==0) gCnt[4]++;
  else if(strcmp(s[i].grd,"C")==0) gCnt[5]++;
  else if(strcmp(s[i].grd,"D")==0) gCnt[6]++;
  else gCnt[7]++;
 }

 printf("\nClass Avg %% : %.2f",sm/n);
 printf("\nMax %% : %.2f",mx);
 printf("\nMin %% : %.2f\n",mn);

 printf("\nGrade Dist:\n");
 printf("O:%d A+:%d A:%d B+:%d B:%d C:%d D:%d F:%d\n",
 gCnt[0],gCnt[1],gCnt[2],gCnt[3],
 gCnt[4],gCnt[5],gCnt[6],gCnt[7]);
}

int main()
{
 struct Student st[MAX];
 int n,i,j;
 FILE *fp=fopen("student.txt","r");

 if(fp==NULL){
  printf("File err\n");
  return 1;
 }

 fscanf(fp,"%d",&n);

 for(i=0;i<n;i++)
 {
  fscanf(fp,"%s %s",st[i].id,st[i].name);

  if(!validID(st[i].id)||dupID(st,i,st[i].id)){
   printf("Bad ID\n");
   return 1;
  }

  if(!validNm(st[i].name)){
   printf("Bad Name\n");
   return 1;
  }

  for(j=0;j<SUBJECTS;j++){
   fscanf(fp,"%d %d",&st[i].minor[j],&st[i].major[j]);
   if(!vMin(st[i].minor[j])||!vMaj(st[i].major[j])){
    printf("Marks err\n");
    return 1;
   }
  }
  calcRes(&st[i]);
 }

 fclose(fp);

 showRpt(st,n);
 stats(st,n);

 return 0;
}

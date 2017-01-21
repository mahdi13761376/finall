#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
struct words
{
    char word[20];
    struct words *next;
};
struct player
{
    char name[20];
    double score;
    int levelPlayed;
    int isSave;
    int numLevel;
};
int countLevels()
{
    DIR *dp;
    struct dirent *ep;
    int cnt_levels =  0;
    dp = opendir ("levels");
    if (dp != NULL)
    {
        while (ep = readdir (dp))
        {
            char *file_name = ep->d_name,tmp[5];
            if(file_name[strlen(file_name)-5]<='9'&&file_name[strlen(file_name)-5]>='0') cnt_levels++;
        }
        (void) closedir (dp);
    }
    else
        return -1;
    return cnt_levels;
}
struct words * makeNode(char *word)
{
    struct words* tmp=(struct words *)calloc(1,sizeof(struct words ));
    strcpy(tmp->word,word);
    tmp->next=NULL;
    return tmp;
}
void addNode(struct words * input,char * word)
{
    struct words *tmp = input;
    while(tmp->next != NULL)
        tmp =tmp ->next;
    tmp->next=makeNode(word);
}
void deleteNode(struct words **input,int index)
{
    if(*input ==NULL)
        return;
    if(index ==1)
    {
        struct words * tmp;
        tmp =*input;
        *input = (*input)->next;
        free(tmp);
    }
    else
    {
        struct words *tmp,*tmp2;
        tmp=*input;
        int i=0;
        for(i=0; i<index-2; i++)
            tmp=tmp->next;
        tmp2=tmp->next;
        tmp->next=tmp->next->next;
        free(tmp2);
    }
}
FILE *openLevelFile(int level)
{
    char address[50];
    strcpy(address,"levels\\level-");
    char end[5];
    sprintf(end,"%d",level);
    strcat(address,end);
    strcat(address,".txt");
    FILE *fileLevel=fopen(address,"r");
    return fileLevel;

}
int readingWords(FILE *levels,struct words ** level)
{
    char *tmp=(char *)malloc(20*sizeof(char));
    int check=1;
    int count=0;
    printf("\n %d",count);
    while(check)
    {
        tmp =(char*)malloc(20*sizeof(char));
        int i=0;
        for(i=0; check; i++)
        {
            tmp[i]=fgetc(levels);
            if(tmp[i]==' ')
            {
                tmp[i]='\0';
                check=0;
                printf("\n %d",count);

            }
            if(feof(levels))
                break;

        }
        check=1;
        count++;
        if(*level == NULL)
            *level=makeNode(tmp);
        else
            addNode(*level,tmp);
        if(feof(levels))
        {
            check=0;
            deleteNode(level,count);
            count--;
        }
    }
    fclose(levels);
    return count;
}
void printWord(char* word,int line)
{
    system("cls");
    for(; line>1; line--)
        printf("\n");
    printf("%s\n",word);
}
void SetColor(int ForgC)
{
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}
int numOfPlayer(FILE* players)
{
    fseek(players,0,SEEK_END);
    int size = ftell(players)/(sizeof(struct player));
    fseek(players,0,SEEK_SET);
    return size;

}
struct player addPlayer(char name[20])
{
    struct player *out=(struct player*)calloc(1,sizeof(struct player));
    strcpy(out->name,name);
    out->score=0;
    out->levelPlayed=0;
    out->isSave=0;
    out->numLevel=0;
    return *out;
}
struct player * readingPlayers(FILE *in)
{
    struct player *out;
    if(numOfPlayer(in)==0)
    {
        out=NULL;
        return out;

    }
    out=calloc(numOfPlayer(in),sizeof(struct player));
    fread(out,sizeof(struct player),numOfPlayer(in),in);

    return out;
}
int indexOfplayer(struct player* in,int size,char name[20])
{
    int i=0;
    for(i=0; i<size; i++)
    {
        if(!strcmp(name,in[i].name))
        {
            return i;
        }
    }
    return-1;
}
void  swap(struct player *a,struct player *b)
{
    struct player *tmp=a;
    a=b;
    b=tmp;
}
void sort(struct player *in,int num)
{
    int i=0,j=0;
    for(; i<num; i++)
        for(; j<(num-i); j++)
            if(in[j+1].score>in[j].score)
                swap(&in[j],&in[j+1]);

}
void print10(struct player*input,int num)
{
    int tedad=10,i=0;
    system("cls");
    printf("name        score       level played\n");
    if(num <10)
        tedad=num;
    for(; i<tedad; i++)
        printf("%d.%s       %f      %d\n",i+1,input[i].name,input[i].score,input[i].levelPlayed);
}
int main (void)
{
    FILE *load=fopen("player\\players.bin","rb");
    printf("what is your name:");
    int number=numOfPlayer(load);
    struct player *players=readingPlayers(load);
    SetColor(258);
    char name[20];
    scanf("%s",name);
    SetColor(999);
    int index=indexOfplayer(players,number,name);
    char ans;
    int isLoad=0,isWrong=0;
    time_t rand_timer=time(NULL);
    srand(rand_timer);
    FILE *level;
    struct words *node=NULL,*node_tmp;
    int tedad=0,i=0,numOfWords=0,levelPlayed=0,wrongAnswer=0,correctAnswer=0,levelNum=0,index2=0;
    double levelScore=0;
    if(index!=-1&&players[index].isSave)
        isWrong=1;
    while(isWrong==1)
    {
        printf("1.play a new game\n2.resume an old game\nplaese enter 1or2\n");
        SetColor(258);
        scanf(" %c",&ans);
        SetColor(999);
        if(ans=='2')
        {
            isWrong=0;
            levelPlayed=players[index].levelPlayed;
            levelNum=players[index].numLevel;
            levelScore=players[index].score;
            if(levelPlayed>=10)
            {
                printf("you complete all levels\nplease enter 1 to start a new game\n");
                isWrong=1;
                levelPlayed=0;
                levelNum=0;
                levelScore=0;

            }
        }
        else if(ans=='1')
        {
            isWrong=0;
            while(!levelNum)
            {
                char in[5];
                printf("please choose a level we have %d levels\n",countLevels());
                SetColor(258);
                scanf("%s",in);
                levelNum=atoi(in);
                SetColor(999);
                if(levelNum>countLevels()||levelNum<=0)
                {
                    levelNum=0;
                    printf("\nwrong input\n");
                }

            }
        }
        else
            printf("\nwrong input\n");
    }
    if(index==-1)
    {
        number++;
        if(players==NULL)
            players=calloc(number,sizeof(struct player));
        else
            realloc(players,number*sizeof(struct player));
        players[number-1]=addPlayer(name);
    }
    while(index==-1)
    {
        printf("you dont have any save game\nare you ready for a new game?[y/n]\n");
        SetColor(258);
        scanf(" %c",&ans);
        SetColor(999);
        if(ans=='n')
            return 1;
        else if(ans=='y')
            index=number-1;
        else
            printf("wrong input!!!!\n");

    }
    while(!levelNum)
    {
        char in[5];
        printf("please choose a level we have %d levels\n",countLevels());
        SetColor(258);
        scanf("%s",in);
        levelNum=atoi(in);
        SetColor(999);
        if(levelNum>countLevels()||levelNum<=0)
        {
            levelNum=0;
            printf("wrong input\n");
        }
    }
    while(1)
    {
        level=openLevelFile(levelNum);
        if(node==NULL)
        {
            tedad=readingWords(level,&node);
        }
        i=0;
        index2=(rand()%tedad)+1;
        correctAnswer=0;
        wrongAnswer=0;
        int line=1,check=1,index_tmp=index2;
        node_tmp=node;
        clock_t time1 =clock();
        double score_tmp=0;
        for(; index2>1; index2--)
            node_tmp=node_tmp->next;
        clock_t timer=clock(),timer1=0;
        while(i<strlen(node_tmp->word))
        {
            int in;
            if(((clock()-time1)/CLOCKS_PER_SEC)%2==0&&check)
            {
                printWord(node_tmp->word,line);
                check=0;
                line++;
                in=0;
            }
            if(kbhit())
            {
                if(in)
                    printf("\b");
                in=getch();
                SetColor(258);
                putch(in);
                SetColor(999);
                if(in==node_tmp->word[i])
                {
                    node_tmp->word[i]-=32;
                    i++;
                    printWord(node_tmp->word,line);
                    SetColor(258);
                    putch(in);
                    SetColor(999);
                    correctAnswer++;
                }
                else if(in=='P')
                {
                    timer1+=clock()-timer;
                    printf("\nThe game is paused please enter R to continue\n");
                    while(in!='R')
                    {
                        SetColor(258);
                        in=getch();
                        putch(in);
                        SetColor(999);
                        printf("\b");
                        timer=clock();
                    }

                }
                else if(in=='Q')
                {
                    printf("do you want to save the game[y/n]\n");
                    while(1)
                    {
                        SetColor(258);
                        in=getch();
                        putchar(in);
                        SetColor(999);
                        if(in=='y')
                        {
                            players[index].isSave=1;
                            players[index].levelPlayed=levelPlayed;
                            players[index].numLevel=levelNum;
                            players[index].score=levelScore;
                            fclose(load);
                            load=fopen("player\\players.bin","wb");
                            fwrite(players,sizeof(struct player),number,load);
                            fclose(load);
                            break;
                        }
                        else if(in=='n')
                            break;
                        else
                            printf("\nwrong input!!!!\n");
                    }
                    sort(players,number);
                    print10(players,number);
                    return 0;
                }
                else
                    wrongAnswer++;
            }
            if(((clock()-time1)/CLOCKS_PER_SEC)%2==1)
                check=1;
        }
        numOfWords++;
        timer1+=clock()-timer;
        timer1/=CLOCKS_PER_SEC;
        tedad--;
        score_tmp+=((correctAnswer*3)-wrongAnswer)/timer1;
        deleteNode(&node,index_tmp);
        if(node == NULL)
        {
            score_tmp/=numOfWords;
            levelScore+=score_tmp;
            levelPlayed++;
            int input;
            levelNum++;
            if(levelPlayed<10)
            {
                while(1)
                {
                    printf("\nyou finished the level\nyour level score:%f\nscore:%f\ndo you want to continue???[y/n]\n",score_tmp,levelScore);
                    SetColor(258);
                    input=getch();
                    putchar(input);
                    SetColor(999);
                    if(input=='y')
                    {
                        break;
                    }
                    else if(input=='n')
                    {

                        while(1)
                        {
                            printf("do you want to save the game?[y/n]\n");
                            SetColor(258);
                            input=getch();
                            putchar(input);
                            SetColor(999);
                            if(input=='y')
                            {
                                i=0;
                                players[index].isSave=1;
                                players[index].levelPlayed=levelPlayed;
                                players[index].numLevel=levelNum;
                                players[index].score=levelScore;
                                fclose(load);
                                load=fopen("player\\players.bin","wb");
                                fwrite(players,sizeof(struct player),number,load);
                                fclose(load);
                                sort(players,number);
                                printf("%s",players[index].name);
                                print10(players,number);
                                return 0;
                            }
                            else if(input=='n')
                            {
                                players[index].isSave=1;
                                players[index].levelPlayed=levelPlayed;
                                players[index].numLevel=levelNum;
                                players[index].score=levelScore;
                                sort(players,number);
                                print10(players,number);
                                return 0;
                            }
                            else
                                printf("wrong input!!!!\n");


                        }
                    }
                    else
                        printf("\nwrong input!!!!\n");
                }
            }
            else
            {
                printf("you finished all levels\nlevelscore:%f/nscore:%f\n",score_tmp,levelScore);
                while(1)
                {
                    printf("do you want to save your score?[y/n]\n");
                    SetColor(258);
                    input=getch();
                    putchar(input);
                    SetColor(999);
                    if(input=='y')
                    {
                        i=0;
                        players[index].isSave=1;
                        players[index].levelPlayed=levelPlayed;
                        players[index].numLevel=levelNum;
                        players[index].score=levelScore;
                        fclose(load);
                        load=fopen("player\\players.bin","wb");
                        fwrite(players,sizeof(struct player),number,load);
                        fclose(load);
                        sort(players,number);
                        print10(players,number);
                        return 0;
                    }
                    else if(input=='n')
                    {
                        players[index].isSave=1;
                        players[index].levelPlayed=levelPlayed;
                        players[index].numLevel=levelNum;
                        players[index].score=levelScore;
                        sort(players,number);
                        printf("%s",players[index].name);
                        print10(players,number);
                        return 0;
                    }
                    else
                        printf("wrong input!!!!\n");
                }

            }
            if(levelNum>10)
                levelNum-=10;
            numOfWords=0;
            score_tmp=0;
        }
    }
    return 0;
}

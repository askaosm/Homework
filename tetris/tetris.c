#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));
    createRankList();
	while(!exit){
		clear();
		switch(menu()){
        case MENU_RANK: rank(); break;
		case MENU_PLAY: play(); break;
	    case MENU_RECM: recommendedPlay(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}
	endwin();
	system("clear");
	return 0;
}

void InitTetris(){  /*initilize*/
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2]=rand()%7; 

	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	//DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
   
//////////////////////////////////////////////////////////////
	RecNode* root;
	root=InitRecommend(nextBlock[0],field);
	recommend(root);

	DrawRecommend(recommendY,recommendX,nextBlock[0],recommendR); 
    free(root);
	/////////////////////////////////////////////////////////////

	

	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	/*next next block*/
	move(8,WIDTH+10);
	DrawBox(9,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15,WIDTH+10);
	printw("SCORE");
	DrawBox(16,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(17,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	/*nextblock[1]*/
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
	/*nextblock[2]*/
	for( i = 0; i < 4; i++ ){
		move(10+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[2]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}

	
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
	
	
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int new_blockY;
	int new_blockX;
	for(int i=0; i<BLOCK_HEIGHT;i++){
		for(int j=0; j<BLOCK_WIDTH;j++){
			if(block[currentBlock][blockRotate][i][j]){
				new_blockY=blockY+i;
				new_blockX=blockX+j;

				/*check if the position is movable*/
				if( new_blockX<0 || new_blockX>=WIDTH || new_blockY<0 || new_blockY>=HEIGHT || f[new_blockY][new_blockX])
				return 0;
			}
		}
	}

	return 1;
}


void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int prev_bRotate=blockRotate;
    int prevX=blockX;
	int prevY=blockY;
	
	
	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	switch(command){
		case KEY_UP : prev_bRotate= (blockRotate+3) %4; break; /*회전 상태를 하나전 상태로*/

		case KEY_DOWN: prevY--;  break; /*y좌표 하나 줄임*/

		case KEY_LEFT : prevX++;  break; /*x좌표 하나 늘임*/
        
		case KEY_RIGHT : prevX--; break; /*x좌표 하나 줄임*/

		default : break;
	}
    int shadowY=prevY;
	while(CheckToMove(f, currentBlock, prev_bRotate, shadowY+1, prevX)) 
	shadowY++;
 
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
    for (int i=0; i < BLOCK_HEIGHT; i++){
		for (int j =0; j < BLOCK_WIDTH; j++){
			if (block[currentBlock][prev_bRotate][i][j]  && prevY+i >= 0){
				move(i+prevY+1, j+prevX+1); 
				printw(".");
			}if(block[currentBlock][prev_bRotate][i][j] && shadowY+i>=0){
				move(i+shadowY+1, j+prevX+1); 
				printw(".");
			}
		}
	}
		//3. 새로운 블록 정보를 그린다.
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
	DrawRecommend(recommendY,recommendX,currentBlock,recommendR); 
    //커서를 화면 밖으로 이동
	move(HEIGHT, WIDTH+10);
}

void BlockDown(int sig){
	// user code
	if(CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)){ /* draw the new block position on the screen*/
		blockY++;
		DrawChange(field,KEY_DOWN,nextBlock[0], blockRotate, blockY, blockX);
	}else{
				
		if(blockY==-1) gameOver=1;
		else{
			score+=AddBlockToField(field, nextBlock[0],blockRotate,blockY,blockX); /*add the current block to the field*/

			for(int i=0;i<BLOCK_NUM-1;i++) /*Shift all elements of an array by one arr[0]*/
			   nextBlock[i]= nextBlock[i+1];

			nextBlock[BLOCK_NUM-1]=rand()%7; /*nextblock[2]*/
			//////////////////////////////////////////
			RecNode *root;
			root=InitRecommend(nextBlock[0],field);
			recommend(root);
			free(root);
            //////////////////////////////////////////
			DrawNextBlock(nextBlock);
		
	        /*reset*/
			blockRotate=0;
			blockY=-1;
			blockX =WIDTH /2 -2;
            /*update and print score*/
			score+=DeleteLine(field);
			PrintScore(score);
            DrawField();
		
		}

	}
		timed_out=0;
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int touched=0;
	//Block이 추가된 영역의 필드값을 바꾼다.
	for(int i=0; i< BLOCK_HEIGHT;i++){
		for(int j=0; j<BLOCK_WIDTH;j++){
			if(block[currentBlock][blockRotate][i][j]){
				f[blockY+i][blockX+j]=1; 
				 if (HEIGHT == i + blockY+1) 
				touched++; //추가되는 위치의 바로 아래에 필드가 채워져 있으면

			}			
		}
	}
	return touched*10;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code
    int deletedLines=0;
	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
    int fill;
	for(int i=0; i<HEIGHT;i++){
		fill=1;
		for(int j=0; j<WIDTH; j++){
			if(f[i][j]==0){
				fill=0;
				break;
			}
		}
	
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
        if(fill==1){
		    for(int newY=i ; newY>0 ; newY--){
			    for( int newX=0; newX<WIDTH; newX++){
				   if(newY==0) f[newY][newX]=0;
				   else
			          f[newY][newX]=f[newY-1][newX];
			    }
	        }    
     		deletedLines++;
    	}

    }   

    return (deletedLines*deletedLines*100);
}
///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
    int shadowY=y;
	/*Find the Y-coordinate of the lowest position of the block.*/
	while(CheckToMove(field,blockID,blockRotate,shadowY+1,x)){ /*until Y-coordinate can not move */
		shadowY++; 
	}
    
	DrawBlock(shadowY,x,blockID,blockRotate,'/');
}
void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){
	DrawShadow(y,x,blockID,blockRotate);
	DrawBlock(y,x,blockID,blockRotate,' ');
}
void createRankList(){
	// user code
	FILE *fp;
	int i, j;

	//1. 파일 열기
	fp = fopen("rank.txt", "r");
	if(fp==NULL){
	    fp=fopen("rank.txt","w");
	}else{
	//including information and save linked list
	struct Node *current=NULL;
	fscanf(fp,"%d",&score_number); /*the number of data*/
	
    for(i=1;i<=score_number;i++){  /*rank 정보 linked list 읽기*/
		Node* newNode=(Node*)malloc(sizeof(Node));
		fscanf(fp,"%s %d",newNode->name,&newNode->score); 
	    if(head!=NULL){ /*linked list*/
			current->next=newNode;
			current=newNode;
		}else{  /*첫번째 데이터 읽는 후 */
			current=newNode;
			head=newNode;
		}
		free(newNode);
    }
	}

fclose(fp);

}

void rank(){
	// user code
	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 초기화
	struct Node* current;
	int X=1, Y=score_number,ch;
    int count=0;
	clear();
	noecho();
	//2. printw()로 3개의 메뉴출력
	printw("1. list ranks from X to Y\n");
	printw("2. list reanks by a specific name\n");
	printw("3. delete a specific rank\n");
	
	//3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
    ch=wgetch(stdscr);
    
	//4. 각 메뉴에 따라 입력받을 값을 변수에 저장
	//4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
	if (ch == '1') {
		echo(); /*입력한 문자가 화면에 출력됨*/
		printw("X: ");
		scanw("%d",&X); /*아무것도 입력 안되면 X=1*/
		printw("Y: ");
		scanw("%d",&Y);
		noecho();/*입력된 문자는 이후 출력 안됨*/
        
	    printw("    name    |      score      \n");
		printw("------------------------------\n");

	
		if(X>Y || score_number==0){ /*범위 안에 아니면*/
			printw("search failure: no rank in the list");
		}else{
            count=1;
			current=head;
			while(current!=NULL){
				if(X<=count && count<=Y){  /*범위 안에 있으면 */
					printw("%12s|%12d\n", current->name,current->score);
				}
				if(Y>score_number) break; 
				
				count++;
				current=current->next;
			}
		}
		
	}

	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
	else if ( ch == '2') {
		char str[NAMELEN+1];
		int check =0; //이름이 입력과 같은 사람의 수
		echo(); /*입력한 문자가 화면에 출력됨*/
		printw("input the name: ");
		scanw("%s",&str); 
		noecho();

	    printw("    name    |      score      \n");
		printw("------------------------------\n");
	
        
		current=head;
		while(current!=NULL){
			if(! strcmp(current->name,str) ) {
			  printw("%12s|%12d\n", current->name,current->score);
			  check++;
			}
			current= current->next;
		}
		if(check==0){
			printw("search failure: no name in the list");
			exit(1);
		}


	}

	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if ( ch == '3') {
		int num;
		int count;
		int flag=0;
		echo(); /*입력한 문자가 화면에 출력됨*/
		printw("input the rank: ");
		scanw("%d",&num); 
		noecho();

        count=1;
		current=head;
		struct Node* prev=NULL;
		while(current!=NULL){
			if(num==1){
				head=current->next;
				printw("result: tha rank deleted "); 	flag=1;
				score_number--; break;
			}
			if(num==count){
				prev->next=current->next;
                printw("result: tha rank deleted ");  	flag=1;
                score_number--; break;
			}
            prev=current;
			current=current->next;
			count++;
		}
		if(!flag) printw("search failure: the rank not int the list");
		writeRankFile();

	}
	refresh();
	getch();

}

void writeRankFile(){
	// user code
    // 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
	//1. "rank.txt" 연다
	FILE *fp = fopen("rank.txt", "w");
	if(fp==NULL){
		fprintf(stderr,"ERROR! File is not open.\n");
		exit(1);
	}
   //2. 랭킹 정보들의 수를 "rank.txt"에 기록
	fprintf(fp,"%d\n",score_number);/*write  the number of score*/
	struct Node* current=head;
	//정보를 기록 
	while(current!=NULL){
		fprintf(fp,"%s %d\n",current->name,current->score);
		current=current->next;
	}
    fclose(fp);
}

void newRank(int score){
	// user code
    // 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
	char str[NAMELEN+1];
	//1. 사용자 이름을 입력받음
	clear();
	printw("your name: "); echo();
	scanw("%s", str); 
   
	//2. 새로운 노드를 생성해 이름과 점수를 저장, score_number가
	score_number++; /*counting the score number*/
	struct Node* newNode=(Node*)malloc(sizeof(Node));
	strcpy(newNode->name,str);
	newNode->score=score;
	newNode->next=NULL;

	if(head==NULL){
		//어느 점수보다 큰 경우 아니면 데이터가 하나도 없는 경우 가장 앞에 삽입
        newNode->next= head;  //추가한 노드 뒤에 붙이고
		head=newNode; ///그것을 head로
	}else if(head->score < score ) {
		newNode->next= head;  //추가한 노드 뒤에 붙이고
		head=newNode; ///그것을 head로

	}else{

		struct Node *temp=head;
		struct Node *current=head;
		current=current->next;
		//current=current->next;
		while(current!=NULL && current->score > score){
			/*삽입하는 점수가 rnaking에 있는 점수보다 작을때 까지 */
			temp=current;
			current =current->next;
		}
		//삽입
		newNode->next=current;
		temp->next=newNode;

	}
	free(newNode);
	writeRankFile();
	
}

////////////////////////////////////////////////////////////////////////////
void freeNode(RecNode *node) {
    if(node != NULL) {
        for(int i = 0; i < CHILDREN_MAX; i++) {
            freeNode(node->child[i]);
        }
        free(node->child);
        free(node);
    }
}
void DrawRecommend(int y, int x, int blockID,int blockRotate){
	DrawBlock(y,x,blockID,blockRotate,'R');
}
RecNode* InitRecommend( int curBlockID, char field[HEIGHT][WIDTH]) {
    RecNode *root=(RecNode*)malloc(sizeof(RecNode));
	if(root!=NULL){
	root->curBlockID = curBlockID;
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            root->recField[i][j] = field[i][j];
    root->level = 0;
    root->child=malloc(sizeof(RecNode*)*CHILDREN_MAX);
//global v
    recommendR=0;  
    recommendX=0;
    recommendY=0;
	
	return root;
	}
	
	else{
		fprintf(stderr,"ERROR! :   Initrec fumction");
	}
	
}

int recommend(RecNode *root){
	int max=0;
    int Score=0;
	int x=0,y=0,rotation=0;
	if(root==NULL){
		fprintf(stderr,"ERROR! recommend function : root"); 
		return -1;
	}
/////////////////////
    //root->child=malloc(sizeof(RecNode*)*CHILDREN_MAX);

	for(int rotation=0; rotation<NUM_OF_ROTATE; rotation++){
		for(int x=0; x<WIDTH; x++){	//0~WIDTH
			  /*block 이동가능?*/
			  if (!CheckToMove(root->recField, root->curBlockID, rotation, 0, x)) 
			  continue;
			/*copy root field to child field*/
			root->child[rotation]=malloc(sizeof(RecNode));  //
			RecNode *child=root->child[rotation];
			//memset(child, 0, sizeof(RecNode));
			  for(int i=0;i<HEIGHT;i++){
				for(int j=0;j<WIDTH;j++){
					child->recField[i][j]=root->recField[i][j];
				}
			  }
            /*부모노드 레벨+1*/
			child->level=root->level+1;
			int lvblock= nextBlock[root->level];
			/*가장 아래 까지 내려갔을때 y*/
			y=0;
			while(y<HEIGHT){
				if(CheckToMove(child->recField,lvblock,rotation,y+1,x))
                y++;
			}

            /*calculate score???*/
			Score=AddBlockToField(child->recField,lvblock,rotation,y,x);
		    Score+=DeleteLine(child->recField);	
			if(child->level <= VISIBLE_BLOCKS)
			   Score+=recommend(child);  	/*recursion*/

            /*update score*/
			if(max<Score){
				max=Score;
				/*Save best position*/
				if(root->level==0){
					if(recommendY<y){
					recommendR=rotation;
					recommendX=x;
					recommendY=y;
					}
				}
			}

			  if(root->child[rotation] != NULL) {
                 freeNode(root->child[rotation]);
                 root->child[rotation] = NULL; 
            } 
		}
	}
	
	return max;
}


void recommendedPlay(){
	
}
//////////////////////////////////////////////////////////////


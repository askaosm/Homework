#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
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
}

void rank(){
	// user code
}

void writeRankFile(){
	// user code
}

void newRank(int score){
	// user code
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}


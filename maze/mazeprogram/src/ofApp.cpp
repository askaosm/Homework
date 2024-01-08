/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.

	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

	=========================================================================
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	=========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"


#include <iostream>
#include<vector>
#include <chrono>
#include<stack>
using namespace std;
//--------------------------------------------------------------

stack<Block> blockStack;
stack<Block> pathStack;

int directionY[4] = { 1, -1, 0, 0 };
int directionX[4] = { 0, 0, 1, -1 };

void ofApp::setup() {

	ofSetWindowTitle("input Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isGamePlay = false;

	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an input file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked

	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS", false, false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if (title == "Open") {
		readFile();
	}
	if (title == "Exit") {
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if (title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;

	}

	if (title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly

	}

	if (title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if (title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);

	int cell_size = 20;
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			switch (input[i][j])
			{
			case '-':
				ofDrawLine((j - 1) * cell_size, i * cell_size, (j + 1) * cell_size, i * cell_size);
				break;
			case '|':
				ofDrawLine(j * cell_size, (i - 1) * cell_size, j * cell_size, (i + 1) * cell_size);
				break;
			default:
				break;
			}
		}
	}

	if (isGamePlay) {
		if (!isOpen) {
			cout << "You must open file first" << endl;
		}
		else {
			ofSetColor(0, 255, 0);
			sprintf(str, "<<Play Mode>>");
			myFont.drawString(str, 150, ofGetHeight() - 20);
			Block goal = { WIDTH - 2, HEIGHT - 2 };  
			myFont.drawString("G", goal.x * cell_size, goal.y * cell_size);  

			for (int i = 0; i < HEIGHT; i++) {
				for (int j = 0; j < WIDTH; j++) {
					ofSetColor(0, 255, 255);
					ofSetLineWidth(5);

					if (UpPath[i][j]) {
						ofDrawLine(j * cell_size, (i)*cell_size, j * cell_size, (i + 2) * cell_size);
					}

					if (DownPath[i][j]) {
						ofDrawLine(j * cell_size, (i - 2) * cell_size, j * cell_size, (i)*cell_size);

					}

					if (LeftPath[i][j]) {
						ofDrawLine((j)*cell_size, i * cell_size, (j + 2) * cell_size, i * cell_size);

					}

					if (RightPath[i][j]) {

						ofDrawLine((j - 2) * cell_size, i * cell_size, (j)*cell_size, i * cell_size);

					}
				}
			}
			ofSetColor(255, 0, 0);

			ofDrawCircle(playerX * cell_size, playerY * cell_size, 10);  /*present state*/

		}


	}

	if (isdfs)
	{
		ofSetColor(200);
		ofSetLineWidth(5);

		if (isOpen) {

			dfsdraw();
		}
		else
			cout << "You must open file first" << endl;
	}
	if (bShowInfo) {
		// Show keyboard duplicates of menu functions
		ofSetColor(100);
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight() - 20);
	}
	free_flag = 1;

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if (bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else {
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU));
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth() - ofGetWidth()) / 2, (ofGetScreenHeight() - ofGetHeight()) / 2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if (bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if (bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if (GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// Escape key exit has been disabled but it can be checked here
	if (key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if (bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if (key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if (key == 'f') {
		bFullscreen = !bFullscreen;
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}
	
	Block goal = { WIDTH - 2, HEIGHT - 2 };
	if (key == 'p') {//start playing mode 
		/*initialize*/
		isGamePlay = true;
		play = true;
		UpPath = (bool**)malloc(sizeof(bool*) * HEIGHT);
		DownPath = (bool**)malloc(sizeof(bool*) * HEIGHT);
		LeftPath = (bool**)malloc(sizeof(bool*) * HEIGHT);
		RightPath = (bool**)malloc(sizeof(bool*) * HEIGHT);
		for (int i = 0; i < HEIGHT; i++) {
			UpPath[i] = (bool*)malloc(sizeof(bool) * WIDTH);
			DownPath[i] = (bool*)malloc(sizeof(bool) * WIDTH);
			LeftPath[i] = (bool*)malloc(sizeof(bool) * WIDTH);
			RightPath[i] = (bool*)malloc(sizeof(bool) * WIDTH);
		}
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				UpPath[i][j] = DownPath[i][j] = LeftPath[i][j] = RightPath[i][j] = false;
			}
		}
	}

	if (isGamePlay) {
		start_time = std::chrono::system_clock::now();
		
		switch (key) {
		case OF_KEY_UP:
			if (playerY > 0 && input[playerY - 1][playerX] != '-') {
				playerY -= 2;
				UpPath[playerY][playerX] = true;
			}
			break;

		case OF_KEY_DOWN:
			if ((playerY) < HEIGHT - 2 && input[playerY + 1][playerX] != '-') {
				playerY += 2;
				DownPath[playerY][playerX] = true;
			}
			break;

		case OF_KEY_LEFT:
			if (playerX > 0 && input[playerY][playerX - 1] != '|') {
				playerX -= 2;
				LeftPath[playerY][playerX] = true;
			}
			break;

		case OF_KEY_RIGHT:
			if (playerX < WIDTH - 2 && input[playerY][playerX + 1] != '|') {
				playerX += 2;
				RightPath[playerY][playerX] = true;
			}
			break;
		default:
			break;
		}
		if (playerX == goal.x && playerY == goal.y) {

			auto end_time = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsed_seconds = end_time - start_time;
			//myFont.drawString("Congratulations!", goal.x * 100 ,goal.y * 100);

			ofLogNotice("Game") << "Congratulations! You reached the goal.";
			ofLogNotice("Game") << "Elapsed time: " << elapsed_seconds.count() << "s";
		}
		if (UpPath[playerY][playerX] || DownPath[playerY][playerX] || LeftPath[playerY][playerX] || RightPath[playerY][playerX]) {
			// Check if game over  //|-|
			if (!(playerY > 0 && input[playerY - 1][playerX] != '-') &&
				!(playerX > 0 && input[playerY][playerX - 1] != '|') &&
				!(playerX < WIDTH - 2 && input[playerY][playerX + 1] != '|')) {
				ofLogNotice("Game") << "Game Over! ";
				isGamePlay = false;

			}///|_|
			else if (!(playerY < HEIGHT - 2 && input[playerY + 1][playerX] != '-') &&
				!(playerX > 0 && input[playerY][playerX - 1] != '|') &&
				!(playerX < WIDTH - 2 && input[playerY][playerX + 1] != '|')) {
				ofLogNotice("Game") << "Game Over!";
				isGamePlay = false;

			}////|=
			else if (!(playerY > 0 && input[playerY - 1][playerX] != '-') &&
				!(playerY < HEIGHT - 2 && input[playerY + 1][playerX] != '-') &&
				!(playerX < WIDTH - 2 && input[playerY][playerX + 1] != '|')) {
				ofLogNotice("Game") << "Game Over!";
				isGamePlay = false;

			}//=|
			else if (!(playerY > 0 && input[playerY - 1][playerX] != '-') &&
				!(playerY < HEIGHT - 2 && input[playerY + 1][playerX] != '-') &&
				!(playerX > 0 && input[playerY][playerX - 1] != '|')) {
				ofLogNotice("Game") << "Game Over! ";
				isGamePlay = false;

			}
		}

	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	isdfs = false;
	if (free_flag == 1) {
		freeMemory();
	}

	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		printf("file name is %s\n", fileName);
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;
			HEIGHT = 0;
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
				string line = *it;
				HEIGHT++;
				WIDTH = line.length();
			}

			input = (char**)malloc(sizeof(char*) * (HEIGHT));
			for (int i = 0; i < HEIGHT; i++) {
				input[i] = (char*)malloc(sizeof(char) * WIDTH);
			}

			int cnt = 0;
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
				string line = *it;
				for (int i = 0; i < WIDTH; i++) {
					input[cnt][i] = line.c_str()[i];
				}
				cnt++;
			}
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::freeMemory() {
	for (int i = 0; i < HEIGHT; i++) {
		free(input[i]);
		free(visited[i]);
	}
	free(input);
	free(visited);

	if (isdfs) {
		while (!blockStack.empty()) {
			blockStack.pop();
		}
		while (!pathStack.empty()) {
			pathStack.pop();
		}
		isdfs = false;
	}

	if (play) {
		for (int i = 0; i < HEIGHT; i++) {
			free(UpPath[i]);
			free(DownPath[i]);
			free(LeftPath[i]);
			free(RightPath[i]);
		}
		free(UpPath);
		free(DownPath);
		free(LeftPath);
		free(RightPath);
		play = false;

	}

}


bool ofApp::DFS()
{
	isdfs = true;
	visited = (int**)malloc(sizeof(int*) * (HEIGHT));
	for (int i = 0; i < HEIGHT; i++) {
		visited[i] = (int*)malloc(sizeof(int) * WIDTH);
	}


	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			visited[i][j] = false;

		}
	}

	Block startBlock = { 1, 1 };
	Block goal = { WIDTH - 2, HEIGHT - 2 };
	blockStack.push(startBlock);//dfs
	pathStack.push(startBlock);//all visitedpath


	visited[1][1] = true;
	while (!blockStack.empty()) {
		Block top = blockStack.top();
		//pathStack.push(top);

		if (blockStack.top().x == goal.x && blockStack.top().y == goal.y) {

			return true;  /*serch complete*/

		}

		bool isMovePossible = false;
		for (int index = 0; index < 4; index++) {
			Block nextBlock = { blockStack.top().x + directionX[index], blockStack.top().y + directionY[index] };
			isMovePossible = false;
			if ((!visited[nextBlock.y][nextBlock.x]) && (input[nextBlock.y][nextBlock.x] == ' ')) {

				visited[nextBlock.y][nextBlock.x] = true;
				pathStack.push(nextBlock);
				blockStack.push(nextBlock);
				isMovePossible = true;
				break;
			}

		}

		if (!isMovePossible) {
			blockStack.pop();

		}


	}
	//return 0;
}

void ofApp::dfsdraw()
{
	//TO DO 

	int cell_size = 20;
	ofSetColor(255, 0, 0);

	// Draw the all path
	stack<Block> pathStackCopy = pathStack;
	Block currentB, nextB;

	if (!pathStackCopy.empty()) {
		currentB = pathStackCopy.top();
	}
	while (!pathStackCopy.empty()) {
		nextB = pathStackCopy.top();
		pathStackCopy.pop();
		//	printf("%d , %d\n", currentB.x, currentB.y);
		ofDrawCircle(currentB.x * cell_size, currentB.y * cell_size, 10);

		currentB = nextB;
	}

	//printf("/n");

	//Best 
	ofSetColor(0, 0, 255);
	stack<Block> tempStack = blockStack;
	Block currentBlock, nextBlock;


	while (!tempStack.empty()) {
		nextBlock = tempStack.top();
		tempStack.pop();
		//printf("%d , %d\n", nextBlock.x, nextBlock.y);
		ofDrawCircle(nextBlock.x * cell_size, nextBlock.y * cell_size, cell_size / 2);
	}
}


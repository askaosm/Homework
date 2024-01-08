#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Cell {
    int set; // ID of the set in which the cell exists
    bool down, right; // if there is the wall ,the value is 1 if not 0
    Cell() : set(0), down(true), right(true) {} // constructor
};

class Maze {
    int width, height;
    Cell** cells;

public:
    Maze(int width, int height) : width(width), height(height) {
        cells = initializeCells();
    }

    ~Maze() {
        freeCells();
    }

    void createMaze() {
        srand(time(NULL));
        createRandomCells();
        mergeLastRow();
        printMaze();
        cout << "Success: maze.maz is created." << endl;
    }

private:
    Cell** initializeCells() {
        int count = 1;
        Cell** cells = new Cell * [height];
        for (int i = 0; i < height; i++) {
            cells[i] = new Cell[width];
            for (int j = 0; j < width; j++) {
                cells[i][j].set = count++;
            }
        }
        return cells;
    }

    void createRandomCells();
    void mergeLastRow();
    void printMaze();
    void MergeSets(int updateID, int updatedID);
    void freeCells();
};

/*2. Randomly choose to remove or keep the wall*/
void Maze::createRandomCells() {
    int randnum, currentID;

    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width - 1; j++) {
            if (cells[i][j].set != cells[i][j + 1].set) {
                randnum = rand() * (i + j + 2);
                if (randnum % 2== 0) {
                    if (cells[i][j].set < cells[i][j + 1].set) {
                        MergeSets(cells[i][j].set, cells[i][j + 1].set);
                        cells[i][j + 1].right = false;

                    }
                    else if (cells[i][j].set > cells[i][j + 1].set) {
                        MergeSets(cells[i][j + 1].set, cells[i][j].set);
                        cells[i][j + 1].right = false;

                    }

                }
            }
        }

        bool removedDownWall = false;
        for (int j = 0; j < width; j++) {
            randnum = rand() * (i + j + 2);
            if (randnum % 2 == 0) {
                removedDownWall = true;
                cells[i + 1][j].down = false;
                cells[i + 1][j].set = cells[i][j].set;
            }

            if (j + 1 < width && cells[i][j + 1].set != cells[i][j].set) {
                if (removedDownWall = false) { /*need to remove at least up wall*/
                    cells[i + 1][j].down = false;
                    cells[i + 1][j].set = cells[i][j].set;
                    removedDownWall = true;
                }

            }
        }
    }
}

/*3. handle last row*/
void Maze::mergeLastRow() {
    for (int j = 0; j < width - 1; j++) {
        if (cells[height - 1][j].set != cells[height - 1][j + 1].set) {
            if (cells[height - 1][j + 1].set < cells[height - 1][j].set) {
                MergeSets(cells[height - 1][j + 1].set, cells[height - 1][j].set);
                cells[height - 1][j + 1].right = false;

            }
            else if (cells[height - 1][j + 1].set > cells[height - 1][j].set) {
                MergeSets(cells[height - 1][j].set, cells[height - 1][j + 1].set);
                cells[height - 1][j + 1].right = false;

            }

        }
    }
}

void Maze::printMaze() {
    ofstream output("maze.maz");

    if (!output.is_open()) {
        cerr << "Error: Can not open maze.maz file\n"; exit(1);
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            output << (cells[i][j].down ? "+-" : "+ "); /*corener and blank or wall*/
        }
        output << "+\n";

        for (int j = 0; j < width; j++) {

            output << (cells[i][j].right ? "| " : "  ");/*wall and blank or blank*2*/
            if (j == width - 1)
                output << "|\n";
        }


        if (i == height - 1) {
            for (int j = 0; j < width; j++) {
                output << "+-";
                if (j == width - 1) output << "+\n";
            }
        }
    }
    output.close();
}

void Maze::MergeSets(int updateID, int updatedID) {
    /* Merge IDs of rooms belonging to the same set*/
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (cells[i][j].set == updatedID)
                cells[i][j].set = updateID;
        }
    }
}
/*free memory*/
void Maze::freeCells() {
    for (int i = 0; i < height; i++) {
        delete[] cells[i];
    }
    delete[] cells;
}
int main() {
    int width, height;
    cout << "width: ";
    cin >> width;
    cout << "height: ";
    cin >> height;

    Maze maze(width, height);
    maze.createMaze();
    return 0;
}


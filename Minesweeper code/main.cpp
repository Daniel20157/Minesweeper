#include<SFML/Graphics.hpp>
#include<time.h>
#include<iostream>
#include<stdlib.h>
using namespace sf;


int main()
{
    //randomly number generator
    srand(time(0));

    //ask user for difficulty level
    int difficulty;
    std::cout << "Please choose a difficulty level:\n" << "Easy type 1\n" << "Medium type 2\n" << "Hard type 3\n";
    std::cin >> difficulty;
// based on difficulty level we will set the number of rows, columns, and mines
    int rows, columns, mines, unflaggedmines;
    switch(difficulty) {
        case 1:
            rows = 9; 
            columns = 9;
            mines = 10;
            break;
        case 2:
            rows =  16;
            columns = 16;
            mines = 40;
            break;
        case 3:
            rows = 16;
            columns = 30;
            mines = 99;
            break;
        default:
            std::cout << "Invalid choice. Exiting.\n";
            return 0;
    }

    int w = 32; // width of each tile and rendering the size of the grid based on the number of rows and columns
    RenderWindow app(VideoMode(w * (rows + 2), w * (columns + 2)), "Minesweeper");

// declearing the grid

    //grid holds all the mines and numbers but is covered by sgrid
    int grid[rows+1][columns+1];

    //sgrid is what is shown to player
    int sgrid[rows+1][columns+1]; 

    Texture t; // loading in the tiles as the graphics for the game
    t.loadFromFile("tiles.jpg");
    Sprite s(t);

 // Adjust the mine initialization based on the difficulty
    int totalMines = 0;
    
    for (int i = 1; i <= rows; i++) // Initialize the grid
        for (int j = 1; j <= columns; j++) {
            sgrid[i][j] = 10;

            if (totalMines < mines)  {  
                if (rand() % 5 == 0) {
                   grid[i][j] = 9; // 9 represents a mine
                   totalMines++;
                } else {
                    grid[i][j] = 0;
                }
            } else {
                grid[i][j] = 0; // No more mines beyond the required number
            }
        }

//grid [1][1] = 9;
//unflaggedmines = 1;
//totalMines = 1;
 // calculating the number of mines around each cell

    for (int i = 1; i <= rows; i++)
        for (int j = 1; j <= columns; j++) {

            // goes through each square around grid[i][j] and ++ to show how many mines are nearby
            //delete this maybe?
            int n = 0;
            if (grid[i][j] == 9) continue;
            if (grid[i + 1][j] == 9) n++;
            if (grid[i][j + 1] == 9) n++;
            if (grid[i - 1][j] == 9) n++;
            if (grid[i][j - 1] == 9) n++;

            if (grid[i + 1][j + 1] == 9) n++;
            if (grid[i - 1][j - 1] == 9) n++;
            if (grid[i - 1][j + 1] == 9) n++;
            if (grid[i + 1][j - 1] == 9) n++;

            grid[i][j] = n;
        }
bool firstClick = true; // To ensure the first click is safe
bool gameOver = false;  // To track the game over state
bool minesRevealed = false;  // To track if mines are revealed
// while loop to get the current position of mouse
    while (app.isOpen()){
        Vector2i pos = Mouse::getPosition(app);
        int x = pos.x / w;
        int y = pos.y / w;
// while loop to process when a button to close window is closed or to reveal a cell that has been clicked
        Event e;
        while (app.pollEvent(e)){

            if (e.type == Event::Closed)
                app.close();

            if (e.type == Event::MouseButtonPressed) {
                
                //updates sgrid[x][y] to have what grid[x][y] has, "revealing" it
                if (e.mouseButton.button == Mouse::Left) {
                    
                    sgrid[x][y] = grid[x][y];
                    //checks if user hit a mine
                    if (grid[x][y] == 9) {

                        //first click protection
                        if (firstClick) {

                            std::cout << "firstclick protection! \n";
                            //have to regenerate grid
                            totalMines = 0;
                            for (int i = 1; i <= rows; i++){
                                for (int j = 1; j <= columns; j++) {
                                    sgrid[i][j] = 10;
                                    if ( totalMines < mines) {
                                        if (rand() % 5 == 0) {
                                            grid[i][j] = 9;
                                            totalMines++;
                                        } 
                                        else {
                                            grid[i][j] = 0;
                                        }
                                    } 
                                    else {
                                        grid[i][j] = 0;
            
                                    }
                                }
                            }
                            for (int i = 1; i <= rows; i++){
                                for (int j = 1; j <= columns; j++){
                                    int n = 0;
                                    if (grid[i][j] == 9) continue;
                                    if (grid[i + 1][j] == 9) n++;
                                    if (grid[i][j + 1] == 9) n++;
                                    if (grid[i - 1][j] == 9) n++;
                                    if (grid[i][j - 1] == 9) n++;

                                    if (grid[i + 1][j + 1] == 9) n++;
                                    if (grid[i - 1][j - 1] == 9) n++;
                                    if (grid[i - 1][j + 1] == 9) n++;
                                    if (grid[i + 1][j - 1] == 9) n++;

                                    grid[i][j] = n;
                                }
                                firstClick = false;
                            }
                        }
                        else {
                            //reveals all squares
                            for (int z = 0; z <= rows ; z++) {
                                for (int v = 0; v <= columns; v++) {
                                    sgrid[z][v] = grid[z][v];
                                    //sleep(sf::milliseconds(100));
                                }
                            }
                        }
                    }
                    else {
                        firstClick = false;
                    }
                }
            
                //flags square
                else if (e.mouseButton.button == Mouse::Right) {
                    sgrid[x][y] = 11;
                    ///*
                    //checks if player flagged a mine
                    if (grid[x][y] == 9) {
                        unflaggedmines--;
                        //std::cout << "flagged mine\n";
                    }

                    //checks if player flaggs all mines
                    if (unflaggedmines <= 0) {
                        // player has won!
                       // std::cout << "player has won \n";

                        //sleep (sf::milliseconds(200));
                        //sets all sgrid to 10 (unoppened)
                        for (int z = 0; z <= rows ; z++) {
                            for (int v = 0; v <= columns; v++) {
                                sgrid[z][v] = 10;
                            }
                        }

                        //win coding
                        for (int A = 1; A <= 8 ; A++) {
                            for (int z = 0; z <= rows ; z++) {
                                for (int v = 0; v <= columns; v++) {
                                    sgrid[z][v] = A;
                                }
                            }
                            
                            // clear window
                            app.clear(Color::White);

                            // Drawing the grid
                            for (int i = 1; i <= rows; i++)
                                for (int j = 1; j <= columns; j++) {
                                    if (sgrid[i][j] == 9) sgrid[i][j] = grid[i][j];

                                    s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
                                    s.setPosition(i * w, j * w);
                                    app.draw(s);
                                }
                            // Display the app
                            app.display();

                            sleep (sf::milliseconds(200));
                        }
                        
                        for (int z = 0; z <= rows ; z++) {
                            for (int v = 0; v <= columns; v++) {
                                sgrid[z][v] = 0;
                            }
                        }
                        
                    }
                    //*/
                }
            }
        }
// clear window
        app.clear(Color::White);

        // Drawing the grid
        for (int i = 1; i <= rows; i++){
            for (int j = 1; j <= columns; j++) {
                if (sgrid[i][j] == 9) sgrid[i][j] = grid[i][j];

                s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
                s.setPosition(i * w, j * w);
                app.draw(s);
            }
        }
// Display the app
        app.display();
    }

    return 0;
}

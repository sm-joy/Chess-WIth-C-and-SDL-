#include "../include/game.h"


Renderer* renderer = NULL;
SDL_Texture* piecetex = NULL;
SDL_Event event;
SDL_Color bgColor = { 55, 61, 99, 255 };

//for game
int boardLabeFontSize= 14;
int menuTitleFontSize = 100;
int buttonFontSize = 60;

TTF_Font* menuTitleBoldFontMont = NULL;
TTF_Font* menuTitleFontSans = NULL;
TTF_Font* fontSans = NULL;
TTF_Font* buttonMediumFontMont = NULL;

Text* rankLabelTexture[8] = { NULL };
Text* fileLabelTexture[8] = { NULL };
static const char* rankLabelText[8] = { "8", "7", "6", "5", "4", "3", "2", "1" };
static const char* fileLabelText[8] = { "a", "b", "c", "d", "e", "f", "g", "h" };
Pos fileLabelPos[8] = {{0, 0}};
Pos rankLabelPos[8] = {{0, 0}};
CellState cell[64] = { {NOCOLOR, NONE, {0,0,0,0}}, {0,0,0,0}, false, false, 0};


//for menu

SDL_Texture* bgs[5] = {NULL};
SDL_Texture* mainMenuBgs[4] = {NULL};
SDL_Texture* optionMenuBgs[3] = {NULL};

static Mixer* mixer = NULL;
Mix_Music* frozenStar = NULL;
Mix_Music* gymnopedieNo1 = NULL;

int menuTitleX = 0;
int menuTitleY = 0;


Button* newGameButton = NULL;

void renderFrame(Renderer* renderer, CellState* cell) {
    setBackgroundColor(renderer, &bgColor);
    SDL_Color tileColor = { 255, 255, 255, 255 };
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            int index = row * 8 + col;

            // render board
            if (cell[index].isSelected) {
                tileColor = selectedColor;
            }
            else if (cell[index].istarget) {
                if ((row + col) % 2 == 0) {
                    tileColor = targetColor1;
                }
                else {
                    tileColor = targetColor2;
                }
            }
            else if ((row + col) % 2 == 0) {
                tileColor = tileColor1;
            }
            else {
                tileColor = tileColor2;
            }

            renderRect(renderer, &(cell[index].dstRect), &tileColor, 1);



            //render pieces
            if (cell[index].piece.type != NONE) {
                renderTextureEa(renderer, piecetex, &(cell[index].piece.srcRect), &(cell[index].dstRect));
            }

            if (col == 0) {
                renderText(renderer, rankLabelTexture[row], rankLabelPos[row].x, rankLabelPos[row].y);
            }

            if (row == 7) {
                renderText(renderer, fileLabelTexture[col], fileLabelPos[col].x, fileLabelPos[col].y);
            }
        }
    }
    displayWindow(renderer);
}



bool initializeGame() {
    renderer = createRenderer("ZiChess", winsize.width, winsize.height, winIconPath);
    if (renderer == NULL) {
        return false;
    }

    mixer = CoreInit();
    frozenStar = loadMusic(mixer, frozenStarMusicPath);
    gymnopedieNo1 = loadMusic(mixer, gymnopediNo1MusicPath);
  
    menuTitleBoldFontMont = loadFont(renderer, montserratBoldFontPath, menuTitleFontSize);
    buttonMediumFontMont = loadFont(renderer, montserratMediumFontPath, buttonFontSize);
    menuTitleFontSans = loadFont(renderer, opensansFontPath, menuTitleFontSize);
    fontSans = loadFont(renderer, opensansFontPath, boardLabeFontSize);

    SDL_Color* textColor = NULL;
    for (int i = 0; i < 8; ++i) {
        textColor = (i % 2) == 0 ? &tileColor2 : &tileColor1;
        rankLabelTexture[i] = createText(renderer, rankLabelText[i], textColor, fontSans);
        textColor = (i % 2) == 0 ? &tileColor1 : &tileColor2;
        fileLabelTexture[i] = createText(renderer, fileLabelText[i], textColor, fontSans);
    }
   

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            int index = i * 8 + j; // row*cols+col //do not change
            int posX = j * squareSize;
            int posY = i * squareSize;
            cell[index].dstRect.x = posX; // dont change
            cell[index].dstRect.y = posY; // dont change 
            cell[index].dstRect.w = squareSize;
            cell[index].dstRect.h = squareSize;
            cell[index].index = index;

            if (j == 0) {
                rankLabelPos[i].x = posX;
                rankLabelPos[i].y = posY;
            }

            if (i == 7) {
                fileLabelPos[j].x = posX + squareSize - fileLabelTexture[j]->width;
                fileLabelPos[j].y = posY + squareSize - fileLabelTexture[j]->height;
            }

        }
    }

    const char* startingFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    loadPositionFromFen(startingFen, cell);

    //boardtex = createboardTexture(renderer);
    piecetex = loadTexture(renderer, pieceImagePath);

    return true;
}

Scene run_game() {

    bool run = true;
    bool isPieceSelected = false;
    int selectedPieceRow = 0;
    int selectedPieceCol = 0;

    MovesArray* moves = NULL;

    while (run) {
        // event handling
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                run = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int row, col;
                    if (getCellPressed(&row, &col)) {
                        if (isPieceSelected) {
                            int targetIndex = row * 8 + col;
                            int staringIndex = selectedPieceRow * 8 + selectedPieceCol;
                            if (moves != NULL && moves->moves != NULL) {
                                for (int i = 0; i < moves->moveCount; ++i) {
                                    if (moves->moves[i]->targetCell == targetIndex) {
                                        movePiece(cell, selectedPieceRow, selectedPieceCol, row, col);
                                    }
                                }
                                destroyMoves(cell, moves->moves, moves->moveCount);
                                free(moves);
                            }
                            else {
                                movePiece(cell, selectedPieceRow, selectedPieceCol, row, col);
                            }

                            cell[staringIndex].isSelected = false;
                            isPieceSelected = false;
                        }
                        else if (markSelected(cell, row, col)) {
                            isPieceSelected = true;
                            selectedPieceCol = col;
                            selectedPieceRow = row;

                            moves = generateMoves(cell, row, col);
                        }
                    }
                }
            default:
                break;
            };
        }

        //update


        //rendering
        renderFrame(renderer, cell);
        displayWindow(renderer);
    }

    return QUIT;
}

void initMenu() {
    bgs[0] = loadTexture(renderer, knightBgPath1);
    bgs[1] = loadTexture(renderer, knightBgPath2);
    bgs[2] = loadTexture(renderer, knightBgPath3);
    bgs[3] = loadTexture(renderer, queenBgPath1);
    bgs[4] = loadTexture(renderer, kingBgPath1);

    mainMenuBgs[0] = bgs[0];
    optionMenuBgs[0] = bgs[0];     
    mainMenuBgs[1] = bgs[4];
    optionMenuBgs[1] = bgs[4];    
    mainMenuBgs[2] = bgs[1];
    optionMenuBgs[2] = bgs[2];
    mainMenuBgs[3] = bgs[3];

}

Scene runMainMenu() {
    int currentMenuBgIndex = randint(0, 3);
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color black = { 0, 0, 0, 255 };
    SDL_Color lightGray = { 224, 224, 224, 255 };
    SDL_Color gray = { 176, 176, 176, 255 };

    Text* menuTitle = createText(renderer, "ZiChess", &white, menuTitleBoldFontMont);
    menuTitleX = (winsize.width / 2) - (menuTitle->width/2);
    menuTitleY = 100;

    int button_padY = 10;
    int button_padX = 100;

    Text* newGameButtonNormalText = createText(renderer, "New Game", &lightGray, buttonMediumFontMont);
    Text* newGameButtonHoverText = createText(renderer, "New Game", &gray, buttonMediumFontMont);
    Text* newGameButtonPressText = createText(renderer, "New Game", &white, buttonMediumFontMont);
    Button* newGameButton = createButtonEx(renderer, 'N', button_padX, 300, 0, 0, newGameButtonNormalText, newGameButtonHoverText, newGameButtonPressText, NULL);

    Text* optionsButtonNormalText = createText(renderer, "Options", &lightGray, buttonMediumFontMont);
    Text* optionsButtonHoverText = createText(renderer, "Options", &gray, buttonMediumFontMont);
    Text* optionsButtonPressText = createText(renderer, "Options", &white, buttonMediumFontMont);
    Button* optionsButton = createButtonEx(renderer, 'O', button_padX, (newGameButton->rect.h + newGameButton->rect.y + button_padY), 0, 0, optionsButtonNormalText, optionsButtonHoverText, optionsButtonPressText, NULL);


    Text* quitButtonNormalText = createText(renderer, "Quit", &lightGray, buttonMediumFontMont);
    Text* quitButtonHoverText = createText(renderer, "Quit", &gray, buttonMediumFontMont);
    Text* quitButtonPressText = createText(renderer, "Quit", &white, buttonMediumFontMont);
    Button* quitButton = createButtonEx(renderer, 'Q', button_padX, (optionsButton->rect.h + optionsButton->rect.y + button_padY), 0, 0, quitButtonNormalText, quitButtonHoverText, quitButtonPressText, NULL);


    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return QUIT;

            switch (handleButtonEvent(renderer, &event)) {
            case QUIT: {
                return QUIT;
                break;
            }
            case MAIN_GAME: {
                return MAIN_GAME;
                break;
            }
            case MAIN_MENU: {
                break;
            }
            default: {
                break;
            }
            }

        }

        //update
        clearWindow(renderer);
        renderTextureEx(renderer, mainMenuBgs[currentMenuBgIndex], 0, 0, 1280, 720);
        renderText(renderer, menuTitle, menuTitleX, menuTitleY);
        renderButton(renderer, newGameButton);
        renderButton(renderer, optionsButton);
        renderButton(renderer, quitButton);
        updateMusic(mixer);
        displayWindow(renderer);
    }
}

void clean()
{
    if (menuTitleBoldFontMont != NULL) TTF_CloseFont(menuTitleBoldFontMont);
    if (menuTitleFontSans != NULL) TTF_CloseFont(menuTitleFontSans);
    if (buttonMediumFontMont != NULL) TTF_CloseFont(buttonMediumFontMont);
    if (fontSans != NULL) TTF_CloseFont(fontSans);
    
    destroyRenderer(renderer);
    destroyCore();
    cleanSettings();
}




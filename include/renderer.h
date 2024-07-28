#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>



typedef struct Text_OBJECT {
    char* text;
    TTF_Font* font;
    SDL_Color* color;
    SDL_Texture* texture;
    int height, width;
} Text;


typedef struct SDL_RENDERER {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Surface* winIcon;
    SDL_Texture** loadedTextures;
    int numTextures;
    Text** createdTexts;
    int numTexts;
} Renderer;


Renderer* createRenderer(const char* pWinTitle, int pWinWidth, int pWinHeight, const char* pWinIconPath, const char* pFontPath, int pFontSize);
void destroyRenderer(Renderer* renderer);
void clearWindow(Renderer* renderer);
void setRenderDrawColor(Renderer* renderer, SDL_Color* pColor);
void setBackgroundColor(Renderer* renderer, SDL_Color* pColor);
void displayWindow(Renderer* renderer);
void loadFont(Renderer* renderer, const char* pFontPath, int pFontSize);
void setWindowIcon(Renderer* renderer, const char* pWinIconPath);
void renderRect(Renderer* renderer, SDL_Rect* pRect, SDL_Color* pColor, int pFilled);
void renderText(Renderer* renderer, Text* text, int posX, int posY);
void renderTextCenterRect(Renderer* renderer, const char* pText, SDL_Color* pColor, SDL_Rect* pRect);
void renderTextureEx(Renderer* renderer, SDL_Texture* pTexture, int pPosX, int pPosY, int pTextureWidth, int pTextureHeight);
void renderTextureEa(Renderer* renderer, SDL_Texture* pTexture, SDL_Rect* srcRect, SDL_Rect* dstRect);
void destroyTextures(Renderer* renderer);
void renderTexture(Renderer* renderer, SDL_Texture* texture, int posX, int poxY);
void destroyTexts(Renderer* renderer);
void destroyText(Text* textobj);


#endif //RENDERER_H
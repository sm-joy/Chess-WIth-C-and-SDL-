#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct HORIZONTAL_SCROLLBAR {
    unsigned int currentValue;
    unsigned int maxValue;
    SDL_Rect barRect;
    SDL_Rect handleRect;
    bool isDragging;
    int dragOffsetX;
    SDL_Color barColor;
    SDL_Color handleColor;
} hScrollbar;

typedef struct  SDL_GRAPHICS_WRAPPER {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* winIcon;
    SDL_Texture** loadedTextures;
    size_t numTextures;
} RenderContext;


RenderContext* createRenderer(const char* pWinTitle, int pWinWidth, int pWinHeight, const char* pWinIconPath);
void destroyRenderer(RenderContext* rc);
void clearWindow(RenderContext* rc);
void setRenderDrawColor(RenderContext* rc, SDL_Color* pColor);
void setBackgroundColor(RenderContext* rc, SDL_Color* pColor);
void setBgTexture(RenderContext* rc, SDL_Texture* texture, int width, int height);
void displayWindow(RenderContext* rc);
void setWindowIcon(RenderContext* rc, const char* pWinIconPath);
void renderRect(RenderContext* rc, SDL_Rect* rect, SDL_Color* color, bool isFill);
void renderTextureEx(RenderContext* rc, SDL_Texture* pTexture, int pPosX, int pPosY, int pTextureWidth, int pTextureHeight);
void renderTextureEa(RenderContext* rc, SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* dstRect);
void destroyTextures(RenderContext* rc);
void renderTexture(RenderContext* rc, SDL_Texture* texture, int posX, int poxY);

hScrollbar* createVScrollbar(RenderContext* rc, int x, int y, int width, int height, int maxValue, SDL_Color handleColor, SDL_Color barColor);
void drawHScrollbar(RenderContext* rc, hScrollbar* scrollbar);

#endif //RENDERER_H
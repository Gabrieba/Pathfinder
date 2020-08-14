#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "../inc/main.h"


// Process events generated by the user
// Return data user
void processEvent(datauser_t* data) {
  SDL_Event event;
  int status = 1;
  while (status) {
    SDL_WaitEvent(&event);
    switch(event.type) {
      case SDL_QUIT:
        status = 0;
        data->code = -1;
        break;
      case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_LSHIFT)
          data->control = 0;
        break;
      case SDL_KEYDOWN:
        if (data->code > 0 && data->code < 6) {
          if (event.key.keysym.sym == SDLK_LSHIFT)
            data->control = 1;
          if (data->control == 1) {
            if (event.key.keysym.sym > 96 && event.key.keysym.sym < 123)
              data->string = (char)event.key.keysym.sym - 32;
            switch(event.key.keysym.sym) {
              case SDLK_SEMICOLON:
                data->string = '.';
                break;
              case SDLK_COLON:
                data->string = '/';
                break;
              case SDLK_COMMA:
                data->string = '?';
                break;
              case SDLK_AMPERSAND:
                data->string = '1';
                break;
              case 233:
                data->string = '2';
                break;
              case SDLK_QUOTEDBL:
                data->string = '3';
                break;
              case SDLK_QUOTE:
                data->string = '4';
                break;
              case SDLK_LEFTPAREN:
                data->string = '5';
                break;
              case SDLK_MINUS:
                data->string = '6';
                break;
              case 232:
                data->string = '7';
                break;
              case SDLK_UNDERSCORE:
                data->string = '8';
                break;
              case 231:
                data->string = '9';
                break;
              case 224:
                data->string = '0';
                break;
            }
          }
          else
            data->string = (char)event.key.keysym.sym;
          status = 0;
          printf("%c\n", data->string);
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        data->string = '\0';
        if (event.button.button == SDL_BUTTON_LEFT) {
          if (event.button.x > 255 && event.button.x < 455 && event.button.y > 394 && event.button.y < 416)
            data->code = 1;
          else if (event.button.x > 175 && event.button.x < 205 && event.button.y > 275 && event.button.y < 297)
            data->code = 2;
          else if (event.button.x > 175 && event.button.x < 205 && event.button.y > 325 && event.button.y < 347)
            data->code = 3;
          else if (event.button.x > 358 && event.button.x < 498 && event.button.y > 275 && event.button.y < 297)
            data->code = 4;
          else if (event.button.x > 358 && event.button.x < 498 && event.button.y > 325 && event.button.y < 347)
            data->code = 5;
          else if (event.button.x > 10 && event.button.x < 30 && event.button.y > 150 && event.button.y < 170)
            data->code = 6;
          else if (event.button.x > 10 && event.button.x < 30 && event.button.y > 180 && event.button.y < 200)
            data->code = 7;
          else if (event.button.x > 170 && event.button.x < 190 && event.button.y > 150 && event.button.y < 170)
            data->code = 8;
          else if (event.button.x > 170 && event.button.x < 190 && event.button.y > 180 && event.button.y < 200)
            data->code = 9;
          else if (event.button.x > 620 && event.button.x < 640 && event.button.y > 220 && event.button.y < 240)
            data->code = 10;
          else if (event.button.x > 620 && event.button.x < 640 && event.button.y > 260 && event.button.y < 280)
            data->code = 11;
          else if (event.button.x > 620 && event.button.x < 640 && event.button.y > 300 && event.button.y < 320)
            data->code = 12;
          else if (event.button.x > 620 && event.button.x < 640 && event.button.y > 340 && event.button.y < 360)
            data->code = 13;
          else if (event.button.x > 620 && event.button.x < 640 && event.button.y > 380 && event.button.y < 400)
            data->code = 14;
          else if (event.button.x > 350 && event.button.x < 500 && event.button.y > 525 && event.button.y < 552)
            data->code = 15;
          else
            data->code = 0;

          if (data->code > 0) {
            status = 0;
          }
        }
      break;
    }
  }
}



// Display a graphic dataform to get user informations : pathfinder algorithm, graph name, departure and arrival node indexes and heuristic function
// Return -1 if an error occured, 0 otherwise
int dataForm(char* filename, char* departurename, char* arrivalname, char* departureindex, char* arrivalindex) {
  int i;
  datauser_t data;
  SDL_Surface* screen = NULL;
  SDL_Surface* text[22] = {NULL};
  SDL_Surface* imgcase[11] = {NULL};
  SDL_Surface* separator[21] = {NULL};
  SDL_Rect position;
  TTF_Font* police1 = NULL;
  TTF_Font* police2 = NULL;
  TTF_Font* police3 = NULL;
  SDL_Color white = {255, 255, 255};
  if (SDL_Init(SDL_INIT_VIDEO) == -1) {
    fprintf(stderr, "SDL initialisation error : %s\n", SDL_GetError());
    return -1;
  }
  if (TTF_Init() == -1) {
    fprintf(stderr, "TTF initialisation error : %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }
  screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE);
  if (screen == NULL) {
    fprintf(stderr, "SDL screen initialisation error : %s\n", SDL_GetError());
    SDL_Quit();
    return -1;
  }
  police1 = TTF_OpenFont("police/clootie.otf", 50);
  police2 = TTF_OpenFont("police/clootie.otf", 35);
  police3 = TTF_OpenFont("police/clootie.otf", 20);
  SDL_WM_SetCaption("Pathfinder algorithms - Dataform", NULL);

  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 50, 50, 50));

  text[0] = TTF_RenderText_Blended(police1, "Pathfinder algorithms", white);
  position.x = 10;
  position.y = 0;
  SDL_BlitSurface(text[0], NULL, screen, &position);
  text[1] = TTF_RenderText_Blended(police3, "A Star", white);
  position.x = 50;
  position.y = 147;
  SDL_BlitSurface(text[1], NULL, screen, &position);
  text[2] = TTF_RenderText_Blended(police3, "Dijkstra", white);
  position.x = 50;
  position.y = 177;
  SDL_BlitSurface(text[2], NULL, screen, &position);
  text[3] = TTF_RenderText_Blended(police2, "Which algorithm ?", white);
  position.x = 10;
  position.y = 90;
  SDL_BlitSurface(text[3], NULL, screen, &position);
  text[4] = TTF_RenderText_Blended(police3, "START node", white);
  position.x = 10;
  position.y = 275;
  SDL_BlitSurface(text[4], NULL, screen, &position);
  text[5] = TTF_RenderText_Blended(police3, "END node", white);
  position.x = 10;
  position.y = 325;
  SDL_BlitSurface(text[5], NULL, screen, &position);
  text[6] = TTF_RenderText_Blended(police3, "Index :", white);
  position.x = 120;
  position.y = 275;
  SDL_BlitSurface(text[6], NULL, screen, &position);
  text[7] = TTF_RenderText_Blended(police3, "Or    node name :", white);
  position.x = 230;
  position.y = 275;
  SDL_BlitSurface(text[7], NULL, screen, &position);
  text[8] = TTF_RenderText_Blended(police3, "Index :", white);
  position.x = 120;
  position.y = 325;
  SDL_BlitSurface(text[8], NULL, screen, &position);
  text[9] = TTF_RenderText_Blended(police3, "Or    node name :", white);
  position.x = 230;
  position.y = 325;
  SDL_BlitSurface(text[9], NULL, screen, &position);
  text[10] = TTF_RenderText_Blended(police2, "Departure and arrival nodes", white);
  position.x = 10;
  position.y = 225;
  SDL_BlitSurface(text[10], NULL, screen, &position);
  text[11] = TTF_RenderText_Blended(police3, "Breadth First Search", white);
  position.x = 210;
  position.y = 147;
  SDL_BlitSurface(text[11], NULL, screen, &position);
  text[12] = TTF_RenderText_Blended(police3, "Depth First Search", white);
  position.x = 210;
  position.y = 177;
  SDL_BlitSurface(text[12], NULL, screen, &position);
  text[13] = TTF_RenderText_Blended(police2, "Graph data", white);
  position.x = 10;
  position.y = 380;
  SDL_BlitSurface(text[13], NULL, screen, &position);
  text[14] = TTF_RenderText_Blended(police3, "Filename :", white);
  position.x = 180;
  position.y = 395;
  SDL_BlitSurface(text[14], NULL, screen, &position);
  text[15] = TTF_RenderText_Blended(police2, "Which heuristic function ?", white);
  position.x = 500;
  position.y = 155;
  SDL_BlitSurface(text[15], NULL, screen, &position);
  text[16] = TTF_RenderText_Blended(police3, "Great circle", white);
  position.x = 660;
  position.y = 217;
  SDL_BlitSurface(text[16], NULL, screen, &position);
  text[17] = TTF_RenderText_Blended(police3, "Manhattan", white);
  position.y = 257;
  SDL_BlitSurface(text[17], NULL, screen, &position);
  text[18] = TTF_RenderText_Blended(police3, "Euclidean", white);
  position.y = 297;
  SDL_BlitSurface(text[18], NULL, screen, &position);
  text[19] = TTF_RenderText_Blended(police3, "Octile", white);
  position.y = 337;
  SDL_BlitSurface(text[19], NULL, screen, &position);
  text[20] = TTF_RenderText_Blended(police3, "Chebyshev", white);
  position.y = 377;
  SDL_BlitSurface(text[20], NULL, screen, &position);
  text[21] = TTF_RenderText_Blended(police3, "(Leave it blank to work on the grid)", white);
  position.x = 245;
  position.y = 430;
  SDL_BlitSurface(text[21], NULL, screen, &position);

  separator[0] = SDL_CreateRGBSurface(SDL_HWSURFACE, 500, 1, 32, 0, 0, 0, 0);
  SDL_FillRect(separator[0], NULL, SDL_MapRGB((separator[0])->format, 255, 255, 255));
  position.x = 10;
  position.y = 310;
  SDL_BlitSurface(separator[0], NULL, screen, &position);
  separator[1] = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, 22, 32, 0, 0, 0, 0);
  separator[2] = SDL_CreateRGBSurface(SDL_HWSURFACE, 200, 2, 32, 0, 0, 0, 0);
  separator[3] = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, 22, 32, 0, 0, 0, 0);
  separator[4] = SDL_CreateRGBSurface(SDL_HWSURFACE, 200, 2, 32, 0, 0, 0, 0);
  for (i = 1; i < 5; i++)
    SDL_FillRect(separator[i], NULL, SDL_MapRGB((separator[i])->format, 0, 0, 0));
  position.x = 255;
  position.y = 394;
  SDL_BlitSurface(separator[1], NULL, screen, &position);
  position.x = 255;
  position.y = 394;
  SDL_BlitSurface(separator[2], NULL, screen, &position);
  position.x = 455;
  position.y = 394;
  SDL_BlitSurface(separator[3], NULL, screen, &position);
  position.x = 255;
  position.y = 416;
  SDL_BlitSurface(separator[4], NULL, screen, &position);
  separator[5] = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, 22, 32, 0, 0, 0, 0);
  separator[6] = SDL_CreateRGBSurface(SDL_HWSURFACE, 30, 2, 32, 0, 0, 0, 0);
  separator[7] = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, 22, 32, 0, 0, 0, 0);
  separator[8] = SDL_CreateRGBSurface(SDL_HWSURFACE, 30, 2, 32, 0, 0, 0, 0);
  for (i = 5; i < 9; i++)
    SDL_FillRect(separator[i], NULL, SDL_MapRGB((separator[i])->format, 0, 0, 0));
  position.x = 175;
  position.y = 275;
  SDL_BlitSurface(separator[5], NULL, screen, &position);
  position.x = 175;
  position.y = 275;
  SDL_BlitSurface(separator[6], NULL, screen, &position);
  position.x = 205;
  position.y = 275;
  SDL_BlitSurface(separator[7], NULL, screen, &position);
  position.x = 175;
  position.y = 297;
  SDL_BlitSurface(separator[8], NULL, screen, &position);
  separator[9] = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, 22, 32, 0, 0, 0, 0);
  separator[10] = SDL_CreateRGBSurface(SDL_HWSURFACE, 30, 2, 32, 0, 0, 0, 0);
  separator[11] = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, 22, 32, 0, 0, 0, 0);
  separator[12] = SDL_CreateRGBSurface(SDL_HWSURFACE, 30, 2, 32, 0, 0, 0, 0);
  for (i = 9; i < 13; i++)
    SDL_FillRect(separator[i], NULL, SDL_MapRGB((separator[i])->format, 0, 0, 0));
  position.x = 175;
  position.y = 325;
  SDL_BlitSurface(separator[9], NULL, screen, &position);
  position.x = 175;
  position.y = 325;
  SDL_BlitSurface(separator[10], NULL, screen, &position);
  position.x = 205;
  position.y = 325;
  SDL_BlitSurface(separator[11], NULL, screen, &position);
  position.x = 175;
  position.y = 347;
  SDL_BlitSurface(separator[12], NULL, screen, &position);
  separator[13] = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, 22, 32, 0, 0, 0, 0);
  separator[14] = SDL_CreateRGBSurface(SDL_HWSURFACE, 140, 2, 32, 0, 0, 0, 0);
  separator[15] = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, 22, 32, 0, 0, 0, 0);
  separator[16] = SDL_CreateRGBSurface(SDL_HWSURFACE, 140, 2, 32, 0, 0, 0, 0);
  for (i = 13; i < 17; i++)
    SDL_FillRect(separator[i], NULL, SDL_MapRGB((separator[i])->format, 0, 0, 0));
  position.x = 358;
  position.y = 275;
  SDL_BlitSurface(separator[13], NULL, screen, &position);
  position.x = 358;
  position.y = 275;
  SDL_BlitSurface(separator[14], NULL, screen, &position);
  position.x = 498;
  position.y = 275;
  SDL_BlitSurface(separator[15], NULL, screen, &position);
  position.x = 358;
  position.y = 297;
  SDL_BlitSurface(separator[16], NULL, screen, &position);
  separator[17] = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, 22, 32, 0, 0, 0, 0);
  separator[18] = SDL_CreateRGBSurface(SDL_HWSURFACE, 140, 2, 32, 0, 0, 0, 0);
  separator[19] = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, 22, 32, 0, 0, 0, 0);
  separator[20] = SDL_CreateRGBSurface(SDL_HWSURFACE, 140, 2, 32, 0, 0, 0, 0);
  for (i = 17; i < 21; i++)
    SDL_FillRect(separator[i], NULL, SDL_MapRGB((separator[i])->format, 0, 0, 0));
  position.x = 358;
  position.y = 325;
  SDL_BlitSurface(separator[17], NULL, screen, &position);
  position.x = 358;
  position.y = 325;
  SDL_BlitSurface(separator[18], NULL, screen, &position);
  position.x = 498;
  position.y = 325;
  SDL_BlitSurface(separator[19], NULL, screen, &position);
  position.x = 358;
  position.y = 347;
  SDL_BlitSurface(separator[20], NULL, screen, &position);

  for (i = 0; i < 9; i++)
    imgcase[i] = SDL_LoadBMP("image/case.bmp");
  position.x = 10;
  position.y = 150;
  SDL_BlitSurface(imgcase[0], NULL, screen, &position);
  position.y = 180;
  SDL_BlitSurface(imgcase[1], NULL, screen, &position);
  position.x = 170;
  position.y = 150;
  SDL_BlitSurface(imgcase[2], NULL, screen, &position);
  position.y = 180;
  SDL_BlitSurface(imgcase[3], NULL, screen, &position);
  position.x = 620;
  position.y = 220;
  SDL_BlitSurface(imgcase[4], NULL, screen, &position);
  position.y = 260;
  SDL_BlitSurface(imgcase[5], NULL, screen, &position);
  position.y = 300;
  SDL_BlitSurface(imgcase[6], NULL, screen, &position);
  position.y = 340;
  SDL_BlitSurface(imgcase[7], NULL, screen, &position);
  position.y = 380;
  SDL_BlitSurface(imgcase[8], NULL, screen, &position);
  imgcase[9] = SDL_LoadBMP("image/apply_button.bmp");
  position.x = WIDTH/2 - ((imgcase[9])->w) / 2;
  position.y = 525;
  SDL_BlitSurface(imgcase[9], NULL, screen, &position);
  imgcase[10] = SDL_LoadBMP("image/algorithm_grid.bmp");
  position.x = WIDTH - (imgcase[10])->w - 1;
  position.y = 1;
  SDL_BlitSurface(imgcase[10], NULL, screen, &position);

  SDL_Flip(screen);
  data.code = 0;
  data.control = 0;
  data.string = '\0';
  while (data.code != -1 && data.code != 15) {
    processEvent(&data);
    // Traitement de data.code
    if (data.string != '\0') {
      switch(data.code) {
        case 1:
          sprintf(filename, "%s%c", filename, data.string);
          break;
        case 2:
          sprintf(departureindex, "%s%c", departureindex, data.string);
          break;
        case 3:
          sprintf(arrivalindex, "%s%c", arrivalindex, data.string);
          break;
        case 4:
          sprintf(departurename, "%s%c", departurename, data.string);
          break;
        case 5:
          sprintf(arrivalname, "%s%c", arrivalname, data.string);
          break;
        default:
          errorMsg("Should not be here");
      }
    data.string = '\0';
    }
  }
  for (i = 0; i < 22; i++)
    SDL_FreeSurface(text[i]);
  for (i = 0; i < 11; i++)
    SDL_FreeSurface(imgcase[i]);
  for (i = 0; i < 21; i++)
    SDL_FreeSurface(separator[i]);
  SDL_FreeSurface(screen);
  TTF_CloseFont(police1);
  TTF_CloseFont(police2);
  TTF_CloseFont(police3);
  TTF_Quit();
  SDL_Quit();
  return 0;
}
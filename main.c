#include <stdio.h>
#include <stdlib.h>
#include "/opt/homebrew/include/SDL2/SDL.h"
#include "chip8.h"


// #define CHIP8_SCREEN_WIDTH 64
// #define CHIP8_SCREEN_HEIGHT 32
// #define PIXEL_SIZE 10


void renderDisplay(SDL_Renderer* renderer, ChipState* state);

int main(int argc, char *argv[]){
    int quit = 0;
    int debug = 0;

    ChipState chip8; 
    ChipInit(&chip8);

    // SDL INIT and create pointers 
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("ERROR: sdl_init failed\n"); 
        return -1;
    }
    SDL_Window* window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,640,320,SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);


    // Open ROM
    if(argc <2){
        printf("Did not load rom\n");
        printf("Usage ./a.out <rom-name>\n"); 
        return -1; 
    }

    FILE* rom =fopen(argv[1], "rb");
    if(rom == NULL){
        printf("ERROR LOADING ROM");
        return -1;
    }

    // Read ROM into Memory then close ROM
    fseek(rom, 0, SEEK_END);
    long int romSize= ftell(rom);
    fseek(rom,0,SEEK_SET); 
    fread(chip8.memory + 0x200,1,romSize,rom); 
    fclose(rom); 
    chip8.PC = 0x200;

    while(!quit){
        // Fetch
        uint16_t  opcode = (chip8.memory[chip8.PC] << 8) | chip8.memory[chip8.PC+1];
        chip8.PC += 2; 

        // Decode & Execute
        ExecuteInstruction(&chip8, opcode);
        renderDisplay(renderer, &chip8); 

        SDL_Event event;
        while(SDL_PollEvent(&event)){
        
            if(event.type == SDL_QUIT){
                    quit = 1;
                }

            if(event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym){
                    case SDLK_1:
                        chip8.keypad[0] = 1;
                        break;
                    case SDLK_2:
                        chip8.keypad[1] = 1;
                        break;
                    case SDLK_3:
                        chip8.keypad[2] = 1;
                        break;
                    case SDLK_4:
                        chip8.keypad[3] = 1;
                        break;
                    case SDLK_q:
                        chip8.keypad[4] = 1;
                        break;
                    case SDLK_w:
                        chip8.keypad[5] = 1;
                        break;
                    case SDLK_e:
                        chip8.keypad[6] = 1;
                        break;
                    case SDLK_r:
                        chip8.keypad[7] = 1;
                        break;
                    case SDLK_a:
                        chip8.keypad[8] = 1;
                        break;
                    case SDLK_s:
                        chip8.keypad[9] = 1;
                        break;
                    case SDLK_d:
                        chip8.keypad[10] = 1;
                        break;
                    case SDLK_f:
                        chip8.keypad[11] = 1;
                        break;
                    case SDLK_z:
                        chip8.keypad[12] = 1;
                        break;
                    case SDLK_x:
                        chip8.keypad[13] = 1;
                        break;
                    case SDLK_c:
                        chip8.keypad[14] = 1;
                        break;
                    case SDLK_v:
                        chip8.keypad[15] = 1;
                        break;
                    
                }
            }
            if(event.type == SDL_KEYUP){
                switch(event.key.keysym.sym){
                    case SDLK_1:
                        chip8.keypad[0] = 0;
                        break;
                    case SDLK_2:
                        chip8.keypad[1] = 0;
                        break;
                    case SDLK_3:
                        chip8.keypad[2] = 0;
                        break;
                    case SDLK_4:
                        chip8.keypad[3] = 0;
                        break;
                    case SDLK_q:
                        chip8.keypad[4] = 0;
                        break;
                    case SDLK_w:
                        chip8.keypad[5] = 0;
                        break;
                    case SDLK_e:
                        chip8.keypad[6] = 0;
                        break;
                    case SDLK_r:
                        chip8.keypad[7] = 0;
                        break;
                    case SDLK_a:
                        chip8.keypad[8] = 0;
                        break;
                    case SDLK_s:
                        chip8.keypad[9] = 0;
                        break;
                    case SDLK_d:
                        chip8.keypad[10] = 0;
                        break;
                    case SDLK_f:
                        chip8.keypad[11] = 0;
                        break;
                    case SDLK_z:
                        chip8.keypad[12] = 0;
                        break;
                    case SDLK_x:
                        chip8.keypad[13] = 0;
                        break;
                    case SDLK_c:
                        chip8.keypad[14] = 0;
                        break;
                    case SDLK_v:
                        chip8.keypad[15] = 0;
                        break;
                    
                }
            }
        }


        
    }



    return 0;
}


void renderDisplay(SDL_Renderer* renderer, ChipState* state){
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
    SDL_RenderClear(renderer);

    // Render the Chip-8 display
    for (int y = 0; y < CHIP8_SCREEN_HEIGHT; y++) {
        for (int x = 0; x < CHIP8_SCREEN_WIDTH; x++) {
            // Calculate the index into the display buffer
            int index = y * CHIP8_SCREEN_WIDTH + x;

            // If the pixel is set (1), draw a rectangle
            if (state->screen[index]) {
                SDL_Rect pixelRect = {x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE};
                SDL_SetRenderDrawColor(renderer, 0, 255, 102, 255); // White
                SDL_RenderFillRect(renderer, &pixelRect);
            }
        }
    }

    // Update the display
    SDL_RenderPresent(renderer);

}

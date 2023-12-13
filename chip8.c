#include "chip8.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int debug = 0; 

int randNum(int low, int high){
    srand(time(0));
    int randInt = rand() % (high - low + 1) + low; 
    return randInt; 
}


void ChipInit(ChipState* state){
   // set keypad to off 
    for(int i = 0; i < 16; i++){
        state->keypad[i] = 0; 
    }
    
    uint8_t chip8Font[80] ={
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for(int i = 0; i < sizeof(chip8Font); i++){
        state->memory[i] = chip8Font[i]; 
    }

    state->SP = 0;  // stack pointer


}
void Push(ChipState* state, uint16_t address){
   if(state->SP < STACK_SIZE){
    state->stack[state->SP] = address;
    state->SP++; 
   }else{
    printf("ERROR: no space to push"); 
   }
}
uint16_t Pop(ChipState* state){
    if(state->SP == 0){
        printf("ERROR: nothing to pop");
        
    }
    return state->stack[state->SP - 1]; 
}

void ExecuteInstruction(ChipState* state, uint16_t opcode){
    
    uint16_t nnn = opcode & 0x0FFF; // Part of Address
    uint8_t kk = opcode & 0x00FF; // 8 bit immediate value
    uint8_t x = (opcode >> 8) & 0x000F; // Vx reg
    uint8_t y = (opcode >> 4) & 0x000F; // Vy reg

    uint16_t height;
    uint16_t xCoord;
    uint16_t yCoord;

    uint32_t buffer;
    
    switch(opcode & 0xF000){
        case 0x0000:
            switch (opcode & 0x00FF){
            case 0x00E0: // 00E0
               for (int i = 0; i < CHIP8_SCREEN_WIDTH * CHIP8_SCREEN_HEIGHT; i++) {
                    state->screen[i] = 0;
                }
                break;

            case 0x00EE: // 00EE
                state->PC = Pop(state);
            break;
            default:
                printf("ERROR UNKOWN OPCODE\n"); 
                break;
            }
        break; 

        case 0x1000: // 1NNN
            state->PC = nnn; 
            if(debug){
                printf("1nnn\n");
            }
            break;

        case 0x2000: // 2nnn NOT SURE IF THIS IS CORRECT 
            state->SP++;
            Push(state,state->PC);
            state->PC = nnn; 
            break;
            
        case 0x3000: // 3xkk
        
            if(state->V[x] == kk){
                state->PC += 2; 
            }
            break;
        
        case 0x4000: // 4xkk
            if(state->V[x] != kk){
                state->PC += 2; 
            }
            break;

        case 0x5000: // 5xy0
            if(state->V[x] == state->V[y]){
                state->PC += 2; 
            }
            break;


        case 0x6000: // 6XKK
            state->V[x] = kk; 
            if(debug){
                printf("6XKK\n");
            }
            break;

        case 0x7000: // 7XKK
            state->V[x] += kk; 
            if(debug){
                printf("7XKK\n");
            }
            break;
        case 0x8000:
            switch(opcode & 0x000F){
                case 0x0000: // 8xy0
                    state->V[x] = state->V[y];
                    break;
                case 0x0001: // 8xy1
                    state->V[x] = (state->V[x] |= state->V[y]);
                    break;
                case 0x0002: // 8xy2
                    state->V[x] = (state->V[x] &= state->V[y]);
                    break;
                case 0x0003: // 8xy3
                    state->V[x] = (state->V[x] ^= state->V[y]);
                    break;
                case 0x0004: // 8xy4
                    
                    buffer = (int)(state->V[x]) + (int)(state->V[y]);
                    state->V[x] = buffer & 0xFF;
                    
                    if(buffer > 255){
                        state->V[0xF] = 1; 
                    }else{
                        state->V[0xF] = 0; 
                    }
                

                    break;
                case 0x0005: // 8xy5
                    state->V[x] = state->V[x] - state->V[y];
                    if(state->V[x] > state->V[y]){
                        state->V[0xf] = 1;
                    }else{
                        state->V[0xf] = 0;
                        state->V[x] =  state->V[x]-  state->V[y]; 
                    }
                    
                    break;
                case 0x0006: // 8xy6
                     state->V[x] = state->V[y];
                     state->V[0xF] = state->V[x] &1;
                     state->V[x] = state->V[x] >> 1;
                     
                    break;

                case 0x0007: // 8xy7
                     state->V[x] = state->V[y] - state->V[x];
                    if(state->V[y] > state->V[x]){
                        state->V[0xf] = 1;
                    }else{
                        state->V[0xf] = 0;
                        state->V[x] =  state->V[y] - state->V[x]; 
                    }
                    break;

                case 0x000E: // 8xyE
                    state->V[x] = state->V[y];
                    state->V[0xF] = state->V[x] &1;
                    state->V[x] = state->V[x] << 1;
                    break;

            }
        case 0x9000: // 9xy0
            if(state->V[x] != state->V[y]){
                state->PC += 2; 
            }
            break;

        case 0xA000: // Annn
            state->I = nnn; 
            if(debug){
                printf("ANNN\n");
            }
            break; 
        case 0xB000: // Bnnn
            state->PC = nnn + state->V[0];
            break;

        case 0xC000: // Cxkk
            state->V[x] = randNum(0,255) & kk; 
            break;

        case 0xD000: // DXYN :(

            if(debug){
                printf("DXYN\n");
            }

            height = opcode & 0x000F;
            xCoord = state->V[x];
            yCoord = state->V[y];

            state->V[0xF] = 0;

            for (int yLine = 0; yLine < height; yLine++) {
            uint8_t spriteByte = state->memory[state->I + yLine];

            for (int xLine = 0; xLine < 8; xLine++) {
                if ((spriteByte & (0x80 >> xLine)) != 0) {
                    int screenX = (xCoord + xLine) % CHIP8_SCREEN_WIDTH;
                    int screenY = (yCoord + yLine) % CHIP8_SCREEN_HEIGHT;
                    int index = screenY * CHIP8_SCREEN_WIDTH + screenX;

                    if (state->screen[index] == 1) {
                        state->V[0xF] = 1; // Set collision flag
                    }

                    state->screen[index] ^= 1; // XOR the pixel
                }
            }
        }
        break;

        case 0xF000:
            
            break;





    }



}

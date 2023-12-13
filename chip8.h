#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32
#define PIXEL_SIZE 10
#define STACK_SIZE 64

typedef struct ChipState{

    uint8_t memory[4096]; // 4 KB of memory
    uint8_t screen[64 * 32]; // Screen display buffer (64x32 pixels)

    uint8_t V[16];  // 16 general-purpose registers (V0-VF)
    uint8_t keypad[16]; // keys 
    uint16_t stack[STACK_SIZE];  

    uint16_t I;     // Index register
    uint16_t PC;    // Program counter
    uint16_t SP;    // Stack pointer

    uint8_t delay;  // Delay timer
    uint8_t sound;  // Sound timer
   
}ChipState;

void ChipInit(ChipState* state); 
void ExecuteInstruction(ChipState* state, uint16_t opcode); 
void Push(ChipState* state, uint16_t address); 
uint16_t Pop(ChipState* state); 


#endif
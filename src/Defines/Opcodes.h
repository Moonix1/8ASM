#pragma once

#include <stdint.h>

typedef uint8_t Byte;

// CPU Instructions
constexpr Byte MOV_IM				= 0x00;
constexpr Byte MOV_R				= 0x01;
constexpr Byte MOV_A				= 0x02;
constexpr Byte MOV_AR				= 0x03;
constexpr Byte MOV_ADDR_IM			= 0x04;
constexpr Byte MOV_ADDR_RIM			= 0x05;
constexpr Byte MOV_ADDR_R			= 0x06;
constexpr Byte MOV_ADDR_A			= 0x07;
constexpr Byte MOV_ADDR_AR			= 0x08;
constexpr Byte MOV_ADDR_RA			= 0x09;
constexpr Byte MOV_ADDR_RR			= 0x0A;

constexpr Byte ADD_R				= 0x0B;
constexpr Byte ADD_I				= 0x0C;
constexpr Byte ADD_RI				= 0x0D;
constexpr Byte ADD_IR				= 0x0E;

constexpr Byte SUB_R				= 0x0F;
constexpr Byte SUB_I				= 0x10;
constexpr Byte SUB_RI				= 0x11;
constexpr Byte SUB_IR				= 0x12;

constexpr Byte ADC_R				= 0x13;
constexpr Byte ADC_I				= 0x14;
constexpr Byte ADC_RI				= 0x15;
constexpr Byte ADC_IR				= 0x16;
constexpr Byte SBC_R				= 0x17;
constexpr Byte SBC_I				= 0x18;
constexpr Byte SBC_RI				= 0x19;
constexpr Byte SBC_IR				= 0x1A;

constexpr Byte MUL_R				= 0x1B;
constexpr Byte MUL_I				= 0x1C;
constexpr Byte MUL_RI				= 0x1D;
constexpr Byte MUL_IR				= 0x1E;

constexpr Byte DIV_R				= 0x1F;
constexpr Byte DIV_I				= 0x20;
constexpr Byte DIV_RI				= 0x21;
constexpr Byte DIV_IR				= 0x22;

constexpr Byte DEC_R				= 0x23;
constexpr Byte INC_R				= 0x24;

constexpr Byte AND_R				= 0x25;
constexpr Byte AND_I				= 0x26;
constexpr Byte AND_RI				= 0x27;
constexpr Byte AND_IR				= 0x28;

constexpr Byte OR_R					= 0x29;
constexpr Byte OR_I					= 0x2A;
constexpr Byte OR_RI				= 0x2B;
constexpr Byte OR_IR				= 0x2C;

constexpr Byte XOR_R				= 0x2D;
constexpr Byte XOR_I				= 0x2E;
constexpr Byte XOR_RI				= 0x2F;
constexpr Byte XOR_IR				= 0x30;

constexpr Byte NOT_R				= 0x31;

constexpr Byte SHL_R				= 0x32;
constexpr Byte SHL_I				= 0x33;
constexpr Byte SHL_RI				= 0x34;
constexpr Byte SHL_IR				= 0x35;

constexpr Byte SHR_R				= 0x36;
constexpr Byte SHR_I				= 0x37;
constexpr Byte SHR_RI				= 0x38;
constexpr Byte SHR_IR				= 0x39;

constexpr Byte CMP_R				= 0x3A;
constexpr Byte CMP_RI				= 0x3B;

constexpr Byte IGT_R				= 0x3C;
constexpr Byte IGT_RI				= 0x3D;

constexpr Byte ILT_R				= 0x3E;
constexpr Byte ILT_RI				= 0x3F;

constexpr Byte IGE_R				= 0x40;
constexpr Byte IGE_RI				= 0x41;

constexpr Byte ILE_R				= 0x42;
constexpr Byte ILE_RI				= 0x43;

constexpr Byte JMP					= 0x44;
constexpr Byte JNZ					= 0x45;
constexpr Byte JZ					= 0x46;
constexpr Byte JNS					= 0x47;
constexpr Byte JS					= 0x48;
constexpr Byte JNC					= 0x49;
constexpr Byte JC					= 0x4A;

constexpr Byte CALL					= 0x4B;
constexpr Byte RET					= 0x4C;

// Stack
constexpr Byte PUSH_IM_W			= 0x4D;
constexpr Byte PUSH_IM_B			= 0x4E;
constexpr Byte PUSH_R				= 0x4F;

constexpr Byte POP_R				= 0x50;
constexpr Byte POP_A				= 0x51;
constexpr Byte POP_AR				= 0x52;

constexpr Byte HLT					= 0xFF;
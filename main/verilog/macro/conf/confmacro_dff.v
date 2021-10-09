// @description
//  ** PART OF **
//  RMR8PM3001A - Taurus 3001
//  (RISC-V 64-bit Privileged Minimal System Processor for T110 ASIC)
//
//  Global Configuration Macro for STDMACROs of DFFs
// 
//  *Note: Change this file to modify global DFF stdmacro.
//         It's recommended to global include this file.
//
//  !! If all definition switches are commented, the behaviour of STDMACROs would be UNDEFINED !!
//
// @author Kumonda221
//

//

/**
*  Make DFF STDMACROs High-Active Reset
*  *UNCOMMENT ON USE*
*/
//`define     STDMACRO_DFF_RESET_ON_HIGH

/**
*  Make DFF STDMACROs Low-Active Reset 
*  *UNCOMMENT ON USE*
*/
`define     STDMACRO_DFF_RESET_ON_LOW

//

/**
*  Make DFF STDMACROs Synchronous Reset
*  *UNCOMMENT ON USE*
*/
`define     STDMACRO_DFF_RESET_SYNC

/**
*  Make DFF STDMACROs Asynchronous Reset
*  *UNCOMMENT ON USE*
*/ 
//`define     STDMACRO_DFF_RESET_ASYNC

//
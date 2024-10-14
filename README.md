# Assembly Output Symbol Extractor

[![Build Status](https://github.com/devon-artmeier/extract-asm-symbols/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/devon-artmeier/extract-asm-symbols/actions/workflows/cmake-multi-platform.yml)

This is a tool to extract symbols from Assembly output files.

## Supports

* Psy-Q symbol files
* vasm vobj files

## Usage

    Usage: extract-symbols -i [input] -o [output] <-m [mode]> <-v [type]> <-b [base]> <-f [symbol]>
                           <-x [symbol]> <-p [prefix]> <-xp [prefix]> <-s [suffix]> <-xs [suffix]>
    
               -i [input]     - Input file
               -o [output]    - Output file
               <-m [mode]>    - Output mode
                                bin - Binary (default)
                                asm - Assembly
                                c   - C
               <-v [type]>    - Value type
                                u32 - Unsigned 32-bit (default)
                                u64 - Unsigned 64-bit
                                s32 - Signed 32-bit
                                s64 - Signed 64-bit
               <-b [base]>    - Numerical system
                                hex - Hexadecimal (default)
                                dec - Decimal
                                bin - Binary
               <-f [symbol]>  - Force include symbol
               <-x [symbol]>  - Exclude symbol
               <-p [prefix]>  - Only include symbols with prefix
               <-xp [prefix]> - Exclude symbols with prefix
               <-s [suffix]>  - Only include symbols with suffix
               <-xs [suffix]> - Exclude symbols with suffix

## Binary Output Format

    Little endian
    
    Number of symbols (4 bytes)
    For each symbol:
        Name character count (1 byte)
        Name string data
        Value (8 bytes)

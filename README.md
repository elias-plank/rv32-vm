# Work in progress VM for [RISCV32](https://riscv.org/)

This project is a small (and incomplete) virtual machine for the RV32I ISA, written in C without any dependencies.

## Features

At the moment, the virtual machine only supports:

 - [x] decoding RV32I instructions
 - [ ] executing instructions
 - [ ] debugging

## Goals

The goal of this project is to build a VM that can execute RV32I code extremely fast. I want to optimize every aspect of the VM:

 - reading instructions from disk/memory
 - decoding instructions
 - execution (maybe JIT compilation?)


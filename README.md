# Microarchitecture-Simulation-Benchmarks

This repo is a centralized place store personal projects, personal projects, labs, etc that don't need their own repo. These projects should be related to computer architecture simulation, more specifically performance and benchmarking.

## Table of Contents
- [gem5 RISCV Iterative Development](#gem5-riscv-iterative-development)
- [gem5 Memory Hierarchy](#gem5-memory-hierarchy)
- [Branch Predictors with Intel Pin Tool](#branch_predictors-with-intel-pin-tool)
- [Computer Systems Organization](#computer-systems-organization)

## gem5 RISCV Iterative Development

The goal of this project was to practice iterative development on a CPU using a budget as a limit. The goal was to achieve the highest MIPS possible within the constraints below. Changes for each iteration were decided by running four different benchmarks and analyzing how various microarchitecture changes impact each type of benchmark.

### Constraints
1. Transitors: < 180 million transistors
2. Area: < 14 mm^2
3. Only modify these micro-architecture blocks:
    - Machine Width (fetch/issue/decode per cycle)
    - Dynamic Branch Predictor size
    - Branch Target Buffer
    - Load/Store Queue Size
    - Number of Integer ALUs and Multiplier/Divider Units
    - Number of Floating-point ALUs and Multiplier/Divider Units
    - Caches (Size, Associativity)
    - Number of Memory Ports

### Clone gem5 and build
```bash
mkdir gem5
git clone https://gem5.googlesource.com/public/gem5
scons build/RISCV/gem5.opt -j 2
build/RISCV/gem5.opt configs/learning_gem5/part1/simple-riscv.py
```
Typical command structure:
<build/{ISA}/gem5.{variant}> <configuration script> <configuration script parameters>

### Run final design
```bash
build/RISCV/gem5.opt ./configs/deprecated/example/se.py --cpu-type=RiscvO3CPU -P \
'system.cpu[:].fetchWidth=10' -P 'system.cpu[:].decodeWidth=8' -P \
'system.cpu[:].issueWidth=8' -P 'system.cpu[:].branchPred.localPredictorSize=4096' -P \
'system.cpu[:].branchPred.BTBEntries=8192' -P 'system.cpu[:].branchPred.BTBTagSize=8' -P \
'system.cpu[:].LQEntries=64' -P 'system.cpu[:].SQEntries=64' -P \
'system.cpu[:].fuPool.FUList[0].count=8' -P 'system.cpu[:].fuPool.FUList[1].count=2' -P \
'system.cpu[:].fuPool.FUList[2].count=1' -P 'system.cpu[:].fuPool.FUList[3].count=1' -P \
'system.cpu[:].fuPool.FUList[8].count=3' --caches --l2cache --cacheline_size=64 --l1i_size=64kB \
--l1i_assoc=2 --l1d_size=512kB --l1d_assoc=16 --l2_size=1MB --l2_assoc=16 -c ./benchmarks/CCe
```

### Results

These can be verified in .txt files in the final directory.

MIPS = simInsts / (simSeconds * 10^6)

CCe MIPS: 80501 / (0.000035 * 10^6) = 2300.03

EM5 MIPS: 39384 / (0.000030 * 10^6) = 1312.8

RCf MIPS: 167944 / (0.000051 * 10^6) = 3292.04

MC MIPS: 260625 / (0.000098 * 10^6) = 2660.46

## gem5 Memory Hierarchy

The goal of this project was to compare various memory hierarchy designs using gem5 Python modules. More information can be found [here](https://gem5.googlesource.com/public/gem5)

Similar command structure to the previous gem5 project
<build/{ISA}/gem5.{variant}> <configuration script> <benchmark binary>

### Results

| Design            | Instructions | Time (s) | MIPS   |
|-------------------|--------------|----------|--------|
| No cache          | 88,767,643   | 7.76     | 11.44  |
| One level         | 88,767,643   | 0.44     | 201.74 |
| Two level         | 88,767,643   | 0.37     | 239.91 |
| Three level       | 88,767,643   | 0.37     | 239.91 |
| No cache low power| 88,767,643   | 8.14     | 10.91  |

The level two and level three cache designs resulted in the best performance. This is because the addition of a larger L2 cache prevents going back to main memory when a block is evicted from the L1 cache. The L3 cache in the three-level design was redundant due to the small matrix size (128x128 with 8B data). The 256KB L2 cache was sufficient, rendering the L3 cache unnecessary, as both two-level and three-level cache designs had identical performance.

The no-cache low power design resulted in the worst MIPS performance, as each access had to go to main memory, incurring a penalty. Additionally, the lower power configuration slowed memory access speed, further reducing performance compared to the standard no-cache design. 

However, in an energy constrained environment the low power cache would be the best choice as the difference in performance was less than one second compared to Configuration A. The majority of the execution time came from memory access latency which the power didn’t impact as much. As such it would be worth saving power for the small cost in performance.

## Branch Predictors with Intel Pin Tool

This project tests the performance of various types of branch predictors using Intel Pin Tool which can be downloaded using the link below.

[Download Here](https://software.intel.com/sites/landingpage/pintool/downloads/pin-external-3.31-98869-gfa6f126a8-gcc-linux.tar.gz)

### Run with the following commands
```bash
make bpred.test
cd obj-intel64
pin -t bpred.so -- tar -czPf /home/ee557/Desktop/p0/pin.tar.gz /home/ee557/Desktop/p0/pin-3.17-
98314-g0c048d619-gcc-linux
```

### Results
The output can be verified in branch_predictors/obj-intel64/bpred.out

## Computer Systems Organization

The computer_systems_organization directory contains multiple Verilog labs focused on RTL and logic.
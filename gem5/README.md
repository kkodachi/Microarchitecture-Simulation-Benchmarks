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
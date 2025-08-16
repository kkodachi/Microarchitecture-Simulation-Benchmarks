class BIMODAL2
{
  public:
    BIMODAL2();
    UINT64 References() const { return _references;};
    UINT64 Predicts() const { return _predicts;};
    UINT64 Mispredicts() const { return _references - _predicts;};
    VOID Activate()
    {
        INS_AddInstrumentFunction(Instruction, this);
    }

  private:
    enum 
    {
        TABLESIZE = 32,
        MAXTHREADS = 100
    };

    static VOID Instruction(INS ins, VOID *v);
    static VOID CondBranch(BIMODAL2 *obj, INT8 * branchHistory, VOID * ip,  BOOL taken);
    inline INT8 * HistAddress(INS ins);

    INT8 _branchHistory[TABLESIZE];
    UINT64 _references, _predicts;
};

inline BIMODAL2::BIMODAL2() : _references(0), _predicts(0)
{
    for (int i=0;i<TABLESIZE;i++){
        _branchHistory[i] = 0;
    }
}

inline VOID BIMODAL2::Instruction(INS ins, VOID *v)
{
    BIMODAL2 *obj = static_cast<BIMODAL2*>(v);
    if (INS_IsBranchOrCall(ins) && INS_HasFallThrough(ins))
        {
            INS_InsertPredicatedCall(ins, IPOINT_BEFORE, 
                           (AFUNPTR)CondBranch, IARG_PTR, (void *)obj, IARG_PTR, (void *)obj->HistAddress(ins), IARG_INST_PTR, IARG_BRANCH_TAKEN, IARG_END);
        }
}

inline INT8 * BIMODAL2::HistAddress(INS ins)
{
    ADDRINT ip = INS_Address(ins);
    return &(_branchHistory[ip & 0x1F]);
}

inline VOID BIMODAL2::CondBranch(BIMODAL2 *obj, INT8 * branchHistory, VOID * ip, BOOL taken)
{
    INT8 history;

    obj->_references++;
  
    history = *branchHistory & 0x3;
    obj->_predicts += (taken && history >= 2);
    obj->_predicts += (!taken && history <= 1);

    if (taken && *branchHistory < 3)
    {
        (*branchHistory)++;
    }
    else if (!taken && *branchHistory > 0)
    {
        (*branchHistory)--;
    }
}
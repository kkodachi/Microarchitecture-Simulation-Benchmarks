class Global2Bit
{
    public:
    Global2Bit() : _references(0), _predicts(0), gbhr(0) {}
    UINT64 References() const { return _references;};
    UINT64 Predicts() const { return _predicts;};
    UINT64 Mispredicts() const { return _references - _predicts;};
    VOID Activate()
    {
        INS_AddInstrumentFunction(Instruction, this);
    }

    private:
    static VOID Instruction(INS ins, VOID *v);
    static VOID CondBranch(Global2Bit *obj, BOOL taken);
    UINT64 _references, _predicts;
    INT8 gbhr;
};

inline VOID Global2Bit::Instruction(INS ins, VOID *v)
{
    Global2Bit* obj = static_cast<Global2Bit*>(v);
    if (INS_IsBranchOrCall(ins) && INS_HasFallThrough(ins)){ // if branch and has follow through instructions
        INS_InsertPredicatedCall(ins,IPOINT_BEFORE,(AFUNPTR)CondBranch,IARG_PTR,(void*)obj,
            IARG_BRANCH_TAKEN,IARG_END);
    }
}

inline VOID Global2Bit::CondBranch(Global2Bit *obj, BOOL taken)
{
    obj->_references++;
    obj->_predicts += (taken && obj->gbhr >= 2); // increment if taken and gbhr is 2 or 3
    obj->_predicts += (!taken && obj->gbhr <= 1); // decrement if not taken and gbhr is 0 or 1
    if (taken && obj->gbhr < 3) // step in taken direction
    {
        obj->gbhr++;
    }
    else if (!taken && obj->gbhr > 0)  // step in untaken direction
    {
        obj->gbhr--;
    }
}
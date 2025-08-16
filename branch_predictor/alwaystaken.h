class AlwaysTaken
{
    public:
    AlwaysTaken() : _references(0), _predicts(0) {}
    UINT64 References() const { return _references;};
    UINT64 Predicts() const { return _predicts;};
    UINT64 Mispredicts() const { return _references - _predicts;};
    VOID Activate()
    {
        INS_AddInstrumentFunction(Instruction, this);
    }

    private:
    static VOID Instruction(INS ins, VOID *v);
    static VOID CondBranch(AlwaysTaken *obj, BOOL taken);

    UINT64 _references, _predicts;
};

inline VOID AlwaysTaken::Instruction(INS ins, VOID *v)
{
    AlwaysTaken *obj = static_cast<AlwaysTaken*>(v);
    if (INS_IsBranchOrCall(ins) && INS_HasFallThrough(ins)){ // if branch and has follow through instructions
        INS_InsertPredicatedCall(ins,IPOINT_BEFORE,(AFUNPTR)CondBranch,IARG_PTR,(void*)obj,
            IARG_BRANCH_TAKEN,IARG_END);
    }
}

inline VOID AlwaysTaken::CondBranch(AlwaysTaken *obj, BOOL taken)
{
    obj->_references++;
    if (taken) obj->_predicts++;
}
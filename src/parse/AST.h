
#include "tt.hpp"

class RTree {
public:
    RTree(NonTerminal* nt);

    RTree(BasicToken* bt);

    bool isLeaf ();

    void insertSubT (RTree *t);

    void printT ();

protected:
    void printBranch (int depth);
private:
    BasicToken *val;
    bool leaf;
    vector <RTree*> branches;
};

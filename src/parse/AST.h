
#include "tt.hpp"

class RTree {
public:
    RTree(BasicToken* bt);

    bool isLeaf ();

    void insertSubT (RTree *t);

    void printT ();

    std::string printVal ();

protected:
    void printBranch (int depth);
private:
    BasicToken *val;
    bool leaf;
    vector <RTree*> branches;
};

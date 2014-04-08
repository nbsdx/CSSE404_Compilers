
#include "tt.hpp"

class RTree {
public:
    RTree(BasicToken* bt);

    bool isLeaf ();

    void insertSubT (RTree *t);

    void printT ();

    std::string printVal ();

    int degree ();

protected:
    void printBranch (int depth);
private:
    int cnt;
    BasicToken *val;
    bool leaf;
    vector <RTree*> branches;
};

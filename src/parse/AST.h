#include <string>
#include "tt.hpp"

class RTree {
public:
    RTree(BasicToken* bt);

    bool isLeaf ();

    void insertSubT (RTree *t);

    void printT ();

    std::string printVal ();

    int degree ();

    vector <RTree*> getBranches ();

    BasicToken *getVal();

    void setType (string str);

    string getType ();

protected:
    void printBranch (int depth);
    string type;
private:
    int cnt;
    BasicToken *val;
    bool leaf;
    vector <RTree*> branches;
    bool typed;
};


#include <vector>
using namespace std;

#include "work.h"

class Worklist
{
private:
    vector<Work> worklist;

public:
    Worklist();
    ~Worklist();

    int addWork(const int parameter);
};
#include "treeProlog.hpp"


int main(int argc, char* argv[]) {
    set<string> factsSet;
    if(argc < 2)
    {
        cout << "Using program is:\n"
                "\ttreeProlog.exe pathToTheFile/file.txt" << endl;
        return 1;
    }
    auto lines = readFromFile(argv[1]);
    vector<TreeBranch> rulesVector = parseFromLine(factsSet, lines);
    bool changed;
    do {
        changed = false;
        auto it = remove_if(rulesVector.begin(), rulesVector.end(), [&factsSet, &changed](TreeBranch &rule) {
            auto result = checkRule(factsSet, rule);
            if (result) {
                cout << rule.descendants[1].entity << endl;
            }
            changed |= result;
            return result;
        });
        rulesVector.erase(it, rulesVector.end());
    }
    while (changed);
}
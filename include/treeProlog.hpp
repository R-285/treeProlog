#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

static const auto OR = "|";
static const auto AND = ",";
static const auto DERIVATION = "->";

struct TreeBranch {
    string entity;
    vector<TreeBranch> descendants;

    TreeBranch(string getEntiry) {
        entity = getEntiry;
    }

    TreeBranch(string getEntiry, vector<TreeBranch> getVector) {
        entity = getEntiry; descendants = getVector;
    }

    TreeBranch() {
    }
};

set<string> parseLine(string &line) {
    set<string> entity;
    while (!line.empty()) {
        constexpr auto quote = '\"';
        if (line[0] == quote) {
            line.erase(0, 1);
            int pos = line.find(quote, 0);
            entity.insert(line.substr(0, pos));
            line.erase(0, pos + 1);
        } else line.erase(0, 1);
    }
    return entity;
}


void parseOperator(const string &rule, const std::string& op, TreeBranch& newNode, int endPart);

TreeBranch madeTree(const string &rule) {
    TreeBranch newNode;
    int beginPart = 0, endPart = 0;
    if ((endPart = rule.find(DERIVATION, beginPart)) != -1) {
        newNode.entity = DERIVATION;
        newNode.descendants.push_back(madeTree(rule.substr(beginPart, endPart)));
        newNode.descendants.push_back(madeTree(rule.substr(endPart + 2, rule.length())));
    }
    else {
        if ((endPart = rule.find(OR, 0)) != -1) {
            parseOperator(rule, OR, newNode, endPart);
        }
        else {
            if ((endPart = rule.find(AND, 0)) != -1) {
                parseOperator(rule, AND, newNode, endPart);
            }
            else {
                newNode.entity = rule;
            }
        }
    }
    return newNode;
}

void parseOperator(const string &rule, const std::string& op, TreeBranch& newNode, int endPart) {
    int beginPart = 0;
    newNode.entity = op;
    newNode.descendants.push_back(madeTree(rule.substr(0, endPart)));
    beginPart = endPart + 1;
    while ((endPart = rule.find(op, beginPart)) != -1) {
        newNode.descendants.push_back(madeTree(rule.substr(beginPart, endPart - beginPart)));
        beginPart = endPart + 1;
    }
    newNode.descendants.push_back(madeTree(rule.substr(beginPart, rule.length())));
}

vector<string> readFromFile(const string& fileName) {
    ifstream readedFile(fileName);
    string line;
    vector<string> result;
    if (readedFile.is_open()) {
        getline(readedFile, line);
        result.push_back(line);
        getline(readedFile, line);
        result.push_back(line);
        readedFile.close();
    }
    return result;
}


vector<TreeBranch> parseFromLine(set<string>& factsSet, vector<string>& file) {
    vector<TreeBranch> rulesVector;
    factsSet = parseLine(file[0]);
    set<string> rulesSet = parseLine(file[1]);
    for (const string& currentRule : rulesSet) {
        auto firstDerivation = currentRule.find(DERIVATION);
        auto ff = 0;
        ff = currentRule.substr(firstDerivation + 2, currentRule.length()).find(DERIVATION);
        if ((firstDerivation != -1) && ( ff == -1 ))
            rulesVector.push_back(madeTree(currentRule)); 
    }
    return rulesVector;
}

bool checkRule(set<string> &factsSet, const TreeBranch &rulesVector) {
    if (rulesVector.entity == DERIVATION) {
        if (checkRule(factsSet, rulesVector.descendants[0])) {
            factsSet.insert(rulesVector.descendants[1].entity);
            return true;
        } else {
            return false;
        }
    }

    if (rulesVector.entity == OR) {
        for (const auto &rule : rulesVector.descendants) {
            if (checkRule(factsSet, rule))
                return true;
        }
        return false;
    }

    if (rulesVector.entity == AND) {
        for (const auto &rule : rulesVector.descendants) {
            if (!checkRule(factsSet, rule))
                return false;
        }
        return true;
    }

    auto myIterator = find(factsSet.begin(), factsSet.end(), rulesVector.entity);
    return myIterator != factsSet.end();
}
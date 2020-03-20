#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "treeProlog.hpp"

TEST_CASE("checkParseLine") {
    SECTION("Normal entries") {
        set<string> temp { "ABC", "BCD", "CDE" };
        string line = "\"ABC\", \"BCD\", \"CDE\"";
        CHECK(parseLine(line) == temp);
    }

    SECTION("Wrong entries") {
        set<string> temp{ "Jjdf93", ",|.ds984395j", "J(fj90455 351 sdf sd    " };
        string line = "\"Jjdf93\", \",|.ds984395j\", \"J(fj90455 351 sdf sd    \"";
        CHECK(parseLine(line) == temp);
    }

    SECTION("Wrong space") {
        set<string> temp{ "ABC", "BCD", "CDE", "DEF" };
        string line = "\"ABC\"\"BCD\"                                                     \"CDE\"fjld;skjfopiwej\"DEF\"";
        CHECK(parseLine(line) == temp);
    }

}

TEST_CASE("checkMadeTree") {
    SECTION("Normal entries") {
        TreeBranch myTree = madeTree("A,B|C->D");
        CHECK(myTree.entity == DERIVATION);
        CHECK(myTree.descendants[1].entity == "D");
        CHECK(myTree.descendants[0].entity == "|");
        CHECK(myTree.descendants[0].descendants[0].entity == ",");
        CHECK(myTree.descendants[0].descendants[1].entity == "C");
        CHECK(myTree.descendants[0].descendants[0].descendants[0].entity == "A");
        CHECK(myTree.descendants[0].descendants[0].descendants[1].entity == "B");
    }
    SECTION("Strange facts") {
        TreeBranch myTree = madeTree("jpdsf84|15df     646,][4]($*&$^]->P$uj39jfi");
        CHECK(myTree.entity == DERIVATION);
        CHECK(myTree.descendants[1].entity == "P$uj39jfi");
        CHECK(myTree.descendants[0].entity == "|");
        CHECK(myTree.descendants[0].descendants[0].entity == "jpdsf84");
        CHECK(myTree.descendants[0].descendants[1].entity == ",");
        CHECK(myTree.descendants[0].descendants[1].descendants[0].entity == "15df     646");
        CHECK(myTree.descendants[0].descendants[1].descendants[1].entity == "][4]($*&$^]");
    }

    SECTION("No facts left") {
        TreeBranch myTree = madeTree("->8");
        CHECK(myTree.entity == DERIVATION);
        CHECK(myTree.descendants[1].entity == "8");
        CHECK(myTree.descendants[0].entity == "");
    }

    SECTION("No facts right") {
        TreeBranch myTree = madeTree("UIOfd->");
        CHECK(myTree.entity == DERIVATION);
        CHECK(myTree.descendants[1].entity == "");
        CHECK(myTree.descendants[0].entity == "UIOfd");
    }

    SECTION("No facts right") {
        TreeBranch myTree = madeTree("UIOfd->");
        CHECK(myTree.entity == DERIVATION);
        CHECK(myTree.descendants[1].entity == "");
        CHECK(myTree.descendants[0].entity == "UIOfd");
    }

    SECTION("No dirivation") {
        TreeBranch myTree = madeTree("A B");
        CHECK(myTree.entity == "A B");
    }

    SECTION("No OR") {
        TreeBranch myTree = madeTree("A,B,C,D->F");
        CHECK(myTree.entity == DERIVATION);
        CHECK(myTree.descendants[1].entity == "F");
        CHECK(myTree.descendants[0].entity == AND);
        CHECK(myTree.descendants[0].descendants[0].entity == "A");
        CHECK(myTree.descendants[0].descendants[1].entity == "B");
        CHECK(myTree.descendants[0].descendants[2].entity == "C");
        CHECK(myTree.descendants[0].descendants[3].entity == "D");
    }

    SECTION("No AND") {
        TreeBranch myTree = madeTree("A|B|C|D->F");
        CHECK(myTree.entity == DERIVATION);
        CHECK(myTree.descendants[1].entity == "F");
        CHECK(myTree.descendants[0].entity == OR);
        CHECK(myTree.descendants[0].descendants[0].entity == "A");
        CHECK(myTree.descendants[0].descendants[1].entity == "B");
        CHECK(myTree.descendants[0].descendants[2].entity == "C");
        CHECK(myTree.descendants[0].descendants[3].entity == "D");
    }

    SECTION("Difficult entries") {
        TreeBranch myTree = madeTree("A,B,C|D|E,F,G->Result");
        CHECK(myTree.entity == DERIVATION);
        CHECK(myTree.descendants[1].entity == "Result");
        CHECK(myTree.descendants[0].entity == OR);
        
        CHECK(myTree.descendants[0].descendants[0].entity == ",");
        CHECK(myTree.descendants[0].descendants[1].entity == "D");
        CHECK(myTree.descendants[0].descendants[2].entity == ",");
        
        CHECK(myTree.descendants[0].descendants[0].descendants[0].entity == "A");
        CHECK(myTree.descendants[0].descendants[0].descendants[1].entity == "B");
        CHECK(myTree.descendants[0].descendants[0].descendants[2].entity == "C");

        CHECK(myTree.descendants[0].descendants[2].descendants[0].entity == "E");
        CHECK(myTree.descendants[0].descendants[2].descendants[1].entity == "F");
        CHECK(myTree.descendants[0].descendants[2].descendants[2].entity == "G");
    }

}

TEST_CASE("checkCheckRule") {
    SECTION("Simple right rule") {
        set<string> factsSet = { "A" };
        CHECK(checkRule(factsSet, madeTree("A->G")));
        CHECK(factsSet.find("G") != factsSet.end());
    }

    SECTION("Simple wrong rule") {
        set<string> factsSet = { "B" };
        CHECK_FALSE(checkRule(factsSet, madeTree("A->G")));
        CHECK_FALSE(factsSet.find("G") != factsSet.end());
    }

    SECTION("Simple rule with empty facts") {
        set<string> factsSet = {  };
        CHECK_FALSE(checkRule(factsSet, madeTree("A->G")));
        CHECK_FALSE(factsSet.find("G") != factsSet.end());
    }

    SECTION("Simple rule with empty facts") {
        set<string> factsSet = {  };
        CHECK_FALSE(checkRule(factsSet, madeTree("A->G")));
        CHECK_FALSE(factsSet.find("G") != factsSet.end());
    }

    SECTION("Empty facts") {
        set<string> factsSet = { "A" };

        CHECK_FALSE(checkRule(factsSet, madeTree("G")));
        CHECK_FALSE(factsSet.find("G") != factsSet.end());

        CHECK_FALSE(checkRule(factsSet, madeTree("")));
        CHECK_FALSE(factsSet.find("G") != factsSet.end());

        CHECK_FALSE(checkRule(factsSet, madeTree("->G")));
        CHECK_FALSE(factsSet.find("G") != factsSet.end());

        CHECK_FALSE(checkRule(factsSet, madeTree("->")));
        CHECK_FALSE(factsSet.find("G") != factsSet.end());
    }

    SECTION("Rule with only AND") {
        set<string> factsSet = { "A", "B" };

        CHECK(checkRule(factsSet, madeTree("A,B->C")));
        CHECK(factsSet.find("C") != factsSet.end());

        CHECK_FALSE(checkRule(factsSet, madeTree("A,Z->D")));
        CHECK_FALSE(factsSet.find("D") != factsSet.end());

        CHECK_FALSE(checkRule(factsSet, madeTree("Z,A->E")));
        CHECK_FALSE(factsSet.find("E") != factsSet.end());

        CHECK_FALSE(checkRule(factsSet, madeTree("Z,X->F")));
        CHECK_FALSE(factsSet.find("F") != factsSet.end());
    }

    SECTION("Rule with only OR") {
        set<string> factsSet = { "A", "B" };

        CHECK(checkRule(factsSet, madeTree("A|B->C")));
        CHECK(factsSet.find("C") != factsSet.end());

        CHECK(checkRule(factsSet, madeTree("A|Z->D")));
        CHECK(factsSet.find("D") != factsSet.end());

        CHECK(checkRule(factsSet, madeTree("Z|A->E")));
        CHECK(factsSet.find("E") != factsSet.end());

        CHECK_FALSE(checkRule(factsSet, madeTree("Z|X->F")));
        CHECK_FALSE(factsSet.find("F") != factsSet.end());
    }

    SECTION("Rule with getting new facts") {
        set<string> factsSet = { "A", "B" };

        CHECK_FALSE(checkRule(factsSet, madeTree("A,C->Z")));
        CHECK_FALSE(factsSet.find("Z") != factsSet.end());

        CHECK(checkRule(factsSet, madeTree("A|C->C")));
        CHECK(factsSet.find("C") != factsSet.end());

        CHECK(checkRule(factsSet, madeTree("A,C->Z")));
        CHECK(factsSet.find("Z") != factsSet.end());

    }

    SECTION("Check some difficult rule") {
        set<string> factsSet = { "Z", "X" };

        CHECK(checkRule(factsSet, madeTree("A,B,C|D|E|F,G|Z,X->Result")));
        CHECK(factsSet.find("Result") != factsSet.end());

        CHECK(checkRule(factsSet, madeTree("Result|IO|OI|XOR|ROX->H")));
        CHECK(factsSet.find("H") != factsSet.end());

        CHECK(checkRule(factsSet, madeTree("H,Result,Z,X->End)")));
        CHECK(factsSet.find("End)") != factsSet.end());

    }
}
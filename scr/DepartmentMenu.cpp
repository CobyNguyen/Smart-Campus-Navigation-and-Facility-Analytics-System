// Read department names safely and send menu choices to the tree.
// Author: Member C

#include "DepartmentMenu.h"
#include "DepartmentTree.h"

#include <istream>
#include <ostream>
#include <sstream>
#include <string>

namespace {
// A blank name returns to the menu before asking any more questions.
bool readName(const std::string& prompt, std::string& name,
              std::istream& in, std::ostream& out) {
    out << prompt;
    if (!std::getline(in, name)) {
        return false;
    }
    if (name.find_first_not_of(" \t\r\n\v\f") == std::string::npos) {
        out << "Error: name cannot be blank.\n";
        return false;
    }
    return true;
}

// The first department is the root, so it does not need a parent prompt.
void addDepartment(DepartmentTree& tree, std::istream& in, std::ostream& out) {
    std::string name;
    std::string parent;
    if (!readName("Department name: ", name, in, out)) {
        return;
    }
    if (!tree.empty() && !readName("Parent department: ", parent, in, out)) {
        return;
    }
    const std::string error = tree.addDepartment(name, parent);
    if (error.empty()) {
        out << "Department added. This change lasts until you exit.\n";
    } else {
        out << "Error: " << error << '\n';
    }
}
}  // namespace

bool readMenuChoice(std::istream& in, int& choice) {
    std::string line;
    if (!std::getline(in, line)) {
        return false;
    }
    std::istringstream input(line);
    if (!(input >> choice)) {
        return false;
    }
    input >> std::ws;
    return input.eof();
}

bool handleDepartmentOption(int choice, DepartmentTree& tree, MenuPhase phase,
                            std::istream& in, std::ostream& out) {
    // The department options are consecutive in both required menus.
    const int first = phase == MenuPhase::Milestone1 ? 6 : 9;
    const int last = phase == MenuPhase::Milestone1 ? 9 : 15;
    if (choice < first || choice > last) {
        return false;
    }

    switch (choice - first) {
        case 0:
            addDepartment(tree, in, out);
            break;
        case 1: {
            std::string name;
            if (readName("Department name: ", name, in, out)) {
                tree.printDepartment(name, out);
            }
            break;
        }
        case 2:
            tree.printHierarchy(out);
            break;
        case 3:
            tree.printTraversal(DepartmentTree::Traversal::Preorder, out);
            break;
        case 4:
            tree.printTraversal(DepartmentTree::Traversal::Postorder, out);
            break;
        case 5:
            tree.printTraversal(DepartmentTree::Traversal::LevelOrder, out);
            break;
        case 6:
            tree.printStatistics(out);
            break;
    }
    return true;
}

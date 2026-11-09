// Department file loading, searches, traversals, and statistics.
// Author: Member C

#include "DepartmentTree.h"

#include <algorithm>
#include <fstream>
#include <ostream>
#include <queue>
#include <utility>

namespace {
// Remove outer whitespace while keeping spaces inside a name.
std::string trim(const std::string& text) {
    constexpr char whitespace[] = " \t\r\n\v\f";
    const auto first = text.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        return {};
    }
    const auto last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

// Compare ASCII letters without changing the original spelling or internal spaces.
std::string nameKey(std::string text) {
    for (char& letter : text) {
        if (letter >= 'A' && letter <= 'Z') {
            letter = static_cast<char>(letter - 'A' + 'a');
        }
    }
    return text;
}
}  // namespace

void DepartmentTree::loadFile(const std::string& filename, std::ostream& out) {
    root_.reset();
    std::ifstream input(filename);
    std::size_t accepted = 0;
    std::size_t rejected = 0;
    std::size_t lineNumber = 0;
    if (!input) {
        out << "Error: cannot open " << filename << '\n';
    }

    std::string line;
    while (std::getline(input, line)) {
        ++lineNumber;
        line = trim(line);
        if (line.empty() || line.front() == '#') {
            continue;
        }

        const std::string error = addRecord(line);
        if (error.empty()) {
            ++accepted;
        } else {
            ++rejected;
            out << "Warning: " << filename << ':' << lineNumber << ": "
                << error << '\n';
        }
    }
    out << filename << ": " << accepted << " accepted, " << rejected
        << " rejected\n";
}

std::string DepartmentTree::addRecord(const std::string& line) {
    if (std::count(line.begin(), line.end(), '|') != 1) {
        return "expected two fields";
    }

    const auto separator = line.find('|');
    const std::string name = trim(line.substr(0, separator));
    const std::string parent = trim(line.substr(separator + 1));
    if (name.empty() || parent.empty()) {
        return "department and parent fields are required";
    }
    if (empty() && nameKey(parent) != "none") {
        return "first department must use NONE as its parent";
    }
    return addDepartment(name, parent);
}

std::string DepartmentTree::addDepartment(const std::string& name,
                                         const std::string& parentName) {
    std::string displayName = trim(name);
    if (displayName.empty()) {
        return "Department name cannot be blank";
    }
    std::string key = nameKey(displayName);
    if (find(root_.get(), key)) {
        return "Duplicate department: " + displayName;
    }

    Node* parent = nullptr;
    if (!empty()) {
        const std::string parentDisplayName = trim(parentName);
        if (parentDisplayName.empty()) {
            return "Parent name cannot be blank";
        }
        parent = find(root_.get(), nameKey(parentDisplayName));
        if (!parent) {
            return "Unknown parent department: " + parentDisplayName;
        }
    }

    auto node = std::make_unique<Node>();
    node->name = std::move(displayName);
    node->key = std::move(key);
    node->parent = parent;
    if (parent) {
        parent->children.push_back(std::move(node));
    } else {
        root_ = std::move(node);
    }
    return {};
}

DepartmentTree::Node* DepartmentTree::find(Node* node, const std::string& key) {
    if (!node || node->key == key) {
        return node;
    }
    for (const auto& child : node->children) {
        if (Node* match = find(child.get(), key)) {
            return match;
        }
    }
    return nullptr;
}

void DepartmentTree::printDepartment(const std::string& name, std::ostream& out) const {
    const std::string query = trim(name);
    if (query.empty()) {
        out << "Error: department name cannot be blank\n";
        return;
    }
    const Node* node = find(root_.get(), nameKey(query));
    if (!node) {
        if (empty()) {
            out << "No departments are loaded.\n";
        }
        out << "Department not found: " << query << '\n';
        return;
    }

    std::size_t depth = 0;
    for (const Node* parent = node->parent; parent; parent = parent->parent) {
        ++depth;
    }
    out << "Department: " << node->name << '\n'
        << "Parent: " << (node->parent ? node->parent->name : "None") << '\n'
        << "Direct children: " << node->children.size() << '\n'
        << "Depth: " << depth << '\n';
}

void DepartmentTree::printHierarchy(std::ostream& out) const {
    if (empty()) {
        out << "No departments are loaded.\n";
        return;
    }
    printPreorder(*root_, 0, 4, out);
}

void DepartmentTree::printTraversal(Traversal order, std::ostream& out) const {
    if (empty()) {
        out << "No departments are loaded.\n";
        return;
    }

    switch (order) {
        case Traversal::Preorder:
            printPreorder(*root_, 0, 0, out);
            break;
        case Traversal::Postorder:
            printPostorder(*root_, out);
            break;
        case Traversal::LevelOrder: {
            std::queue<const Node*> pending;
            pending.push(root_.get());
            while (!pending.empty()) {
                const Node* node = pending.front();
                pending.pop();
                out << node->name << '\n';
                for (const auto& child : node->children) {
                    pending.push(child.get());
                }
            }
            break;
        }
    }
}

void DepartmentTree::printPreorder(const Node& node, std::size_t depth,
                                   std::size_t indentWidth, std::ostream& out) {
    out << std::string(depth * indentWidth, ' ') << node.name << '\n';
    for (const auto& child : node.children) {
        printPreorder(*child, depth + 1, indentWidth, out);
    }
}

void DepartmentTree::printPostorder(const Node& node, std::ostream& out) {
    for (const auto& child : node.children) {
        printPostorder(*child, out);
    }
    out << node.name << '\n';
}

void DepartmentTree::printStatistics(std::ostream& out) const {
    Statistics stats;
    if (empty()) {
        out << "No departments are loaded.\n";
    } else {
        collectStatistics(*root_, 0, stats);
    }

    out << "Total departments: " << stats.departments << '\n'
        << "Leaf departments: " << stats.leaves << '\n'
        << "Tree height: " << (empty() ? "N/A" : std::to_string(stats.height)) << '\n';
}

void DepartmentTree::collectStatistics(const Node& node, std::size_t depth,
                                      Statistics& stats) {
    ++stats.departments;
    if (node.children.empty()) {
        ++stats.leaves;
    }
    stats.height = std::max(stats.height, depth);
    for (const auto& child : node.children) {
        collectStatistics(*child, depth + 1, stats);
    }
}

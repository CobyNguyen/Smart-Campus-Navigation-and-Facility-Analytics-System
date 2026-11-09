// Department tree and the operations used by the campus menu.
// Author: Member C

#ifndef DEPARTMENT_TREE_H
#define DEPARTMENT_TREE_H

#include <cstddef>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

class DepartmentTree {
public:
    enum class Traversal { Preorder, Postorder, LevelOrder };

    // Loading replaces the current tree, even if the file is missing or empty.
    void loadFile(const std::string& filename, std::ostream& out);

    // Check this before asking the user for a parent.
    bool empty() const { return !root_; }

    // Returns an error message on failure, or an empty string on success.
    // The first department becomes the root and needs no parent.
    std::string addDepartment(const std::string& name,
                              const std::string& parentName = "");

    // Find a department and show its parent, direct-child count, and depth.
    void printDepartment(const std::string& name, std::ostream& out) const;
    // Show the tree with four spaces for each level.
    void printHierarchy(std::ostream& out) const;
    // Print one name per line in the chosen order, without indentation.
    void printTraversal(Traversal order, std::ostream& out) const;
    // Count departments and leaves, and find the largest depth.
    void printStatistics(std::ostream& out) const;

private:
    struct Node {
        std::string name;
        std::string key;
        const Node* parent = nullptr;
        // Each node owns its children. The parent pointer only refers back up the tree.
        std::vector<std::unique_ptr<Node>> children;
    };

    struct Statistics {
        std::size_t departments = 0;
        std::size_t leaves = 0;
        std::size_t height = 0;
    };

    std::unique_ptr<Node> root_;

    // Validate one file row before adding it to the tree.
    std::string addRecord(const std::string& line);
    // Search this subtree using the normalized name.
    static Node* find(Node* node, const std::string& key);

    // Use an indent width of 4 for the hierarchy and 0 for a flat traversal.
    static void printPreorder(const Node& node, std::size_t depth,
                              std::size_t indentWidth, std::ostream& out);
    // Visit each child's subtree before printing its parent.
    static void printPostorder(const Node& node, std::ostream& out);
    // Visit each node once and update all three counts together.
    static void collectStatistics(const Node& node, std::size_t depth,
                                  Statistics& stats);
};

#endif

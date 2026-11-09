// Member C menu actions shared by the demo and the team's main menu.
// Author: Member C

#ifndef DEPARTMENT_MENU_H
#define DEPARTMENT_MENU_H

#include <iosfwd>

class DepartmentTree;

enum class MenuPhase { Milestone1, Final };

// Run a department option. Return false if another module owns this option.
bool handleDepartmentOption(int choice, DepartmentTree& tree, MenuPhase phase,
                            std::istream& in, std::ostream& out);

// Read a whole line as one integer. Return false for blank or invalid input.
bool readMenuChoice(std::istream& in, int& choice);

#endif

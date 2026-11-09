Smart Campus Navigation and Facility Analytics System

Purpose
-------
This C++ program loads campus buildings, walkway connections, facilities, and
departments from text files. It demonstrates a custom hash table, an
undirected graph with BFS reachability, and a general department tree.

Build and run
-------------
From the repository root:

    make.exe all
    .\smart_campus.exe

The program requires the four input files in data\:

    buildings.txt
    connections.txt
    facilities.txt
    departments.txt

Directory layout
----------------
    include\  Header files and class declarations.

    src\      C++ implementations and main.cpp.

    data\     The four input data files.

    README.txt            This document.

    CONTRIBUTIONS.txt     Team ownership and shared-work notes.

    design_document.txt   Data-structure and implementation design.

    test_results.txt      Milestone 1 test records.

    Makefile              Build commands.

    .gitignore             Ignored generated files.


Milestone 1 menu
-----------------
1. View all facilities
2. Search facility by ID
3. View campus graph
4. Display graph counts and degrees
5. Find reachable buildings (BFS)
6. Add department
7. Search department
8. Display department hierarchy
9. Run preorder traversal
0. Exit

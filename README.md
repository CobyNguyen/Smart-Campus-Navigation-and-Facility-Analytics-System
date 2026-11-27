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

Pass a data directory as the first argument to run with another set of inputs.
You can also build with `make` and run `./smart_campus data`.

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


Final menu
----------
| Options | Operations |
| --- | --- |
| 1–4 | List facilities and search by ID, name, or category |
| 5–8 | Graph display, counts and degrees, BFS reachability, and shortest path |
| 9–11 | Add a department, search by name, and show the hierarchy |
| 12–15 | Preorder, postorder, level-order, and department statistics |
| 16–17 | Facility category and graph connectivity reports |
| 18 | Exit (`0` also exits) |

Department additions last for the current run. The first addition to an empty
tree creates the root without asking for a parent.

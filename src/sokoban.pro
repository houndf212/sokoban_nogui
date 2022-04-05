TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

SOURCES += \
        board.cpp \
        boardgraph.cpp \
        boardparam.cpp \
        check_matrix_iterator.cpp \
        debug_print.cpp \
        globals.cpp \
        hungarianalg.cpp \
        main.cpp \
        matrix.cpp \
        matrixgraph.cpp \
        movelist.cpp \
        preprocess.cpp \
        roomslice.cpp \
        test_assignmentproblem.cpp \
        test_boardgraph.cpp \
        test_boxmover.cpp \
        test_console_board.cpp \
        test_hungarianalg.cpp \
        test_matrix_iterator.cpp \
        test_matrix_resize.cpp \
        test_matrixgraph.cpp \
        test_munkres.cpp \
        test_print.cpp \
        util.cpp \
        xsb.cpp

HEADERS += \
    assignmentproblem.h \
    basic_matrix_col_iterator.h \
    basic_matrix_index_iterator.h \
    basic_matrix_iterator.h \
    basic_matrix_row_iterator.h \
    basic_pos.h \
    basic_resize_matrix.h \
    board.h \
    boardgraph.h \
    boardparam.h \
    debug_print.h \
    dijkstra.h \
    elements.h \
    global.h \
    globals.h \
    hungarianalg.h \
    iteratorrange.h \
    maskmatrix.h \
    matrix.h \
    matrixgraph.h \
    movedirection.h \
    movelist.h \
    munkres.h \
    preprocess.h \
    roomslice.h \
    types.h \
    util.h \
    xsb.h

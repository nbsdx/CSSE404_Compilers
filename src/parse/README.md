##Build
    python table_parser.py
    g++ -std=c++11 -g main.cpp tt.hpp ../util/token.{h,cpp} ../lex/lexer.{h,cpp} -o parser
    ./parser

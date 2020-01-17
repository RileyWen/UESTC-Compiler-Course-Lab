#include "driver.hpp"
#include "parser.hpp"

driver::driver()
        : trace_parsing(false), trace_scanning(false) {
}

int
driver::parse(const std::string f) {
    file = f;
    location.initialize(&file);
    std::cout << "File Name: " << file << std::endl;
    scan_begin();
    yy::parser parse(*this);
    parse.set_debug_level(trace_parsing);
    int res = parse();
    scan_end();
    return res;
}

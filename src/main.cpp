#include <iostream>
#include <cstdlib>
#include <getopt.h>

#include "graphics_mandelbrot.h"
#include "computing_testing.h"
#include "params.h"

int main(int argc, char* argv[]) {

    int n_tests = n_tests_default;
    int option = 0;
    struct option long_options[] = {
        {"testing", optional_argument, nullptr, 't'},
        {"graph", 0, nullptr, 'g'},
        {nullptr, 0, nullptr, 0}
    };

    const char opts[] = "t:g";

    while ((option = getopt_long(argc, argv, opts, long_options, nullptr)) != -1) {
        switch (option) {
            case 't': {
                if (optarg) { n_tests = std::stoi(optarg); }
                printf("Number of tests: %u\n", n_tests);
                run_tests(n_tests);
                break;
            }
            case 'g': {
                launch_Mandelbrot();
                break;
            }
            default: { break; }
        }
    }


    return 0;
}

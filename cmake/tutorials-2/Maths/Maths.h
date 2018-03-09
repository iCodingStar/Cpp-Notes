//
// Created by star on 18-3-9.
//

#ifndef CPP_NOTES_MATHS_H
#define CPP_NOTES_MATHS_H

#include <stdio.h>

class Maths {

public:
    double sqrt(double x) {
        if (x == 0) {
            return 0;
        }

        double result;
        double delta;
        result = x;
        for (int i = 0; i < 10; ++i) {
            if (result <= 0) {
                result = 0.1;
            }
            delta = x - (result * result);
            result = result + 0.5 * delta / result;
            printf(stdout, "Computing sqrt of %g to be %g\n", x, result);
        }
        return result;
    }
};


#endif //CPP_NOTES_MATHS_H

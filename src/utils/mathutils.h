#include <math.h>

#ifndef MATHUTILS_H_
#define MATHUTILS_H_

inline double nChooseK(int n, int k) {
    if (n < k) return 0.0;

    const int iK = (k << 1) > n ? n - k : k;
    const double dNSubK = (n - iK);
    int i = 1;
    double dRes = i > iK ? 1.0 : (dNSubK + i);

    for (++i; i <= iK; ++i) {
        dRes *= dNSubK + i;
        dRes /= i;
    }
    return dRes;
}

inline double toMW(double dbm) { return pow(10, dbm / 10); }

inline double toDBM(double mw) { return 10 * log10(mw); }

#endif
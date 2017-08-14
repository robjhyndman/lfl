/*
 * File name: algebra.cpp
 * Date:      2016/01/29 10:49
 * Author:    Michal Burda
 */


#include <Rcpp.h>


using namespace Rcpp;


inline void testInvalids(double x) {
    if ((x) < 0 || (x) > 1) {
        stop("argument out of range 0..1");
    }
    if (R_IsNaN((x))) {
        stop("NaN argument");
    }
}


// [[Rcpp::export(name=".goedel.tnorm")]]
double goedel_tnorm(NumericVector vals, LogicalVector naRm)
{
    double res = 1.0;
    bool na = false;
    for (int i = 0; i < vals.size(); ++i) {
        testInvalids(vals[i]);
        if (R_IsNA(vals[i])) {
            na = true;
        } else if (vals[i] < res) {
            res = vals[i];
        }
    }
    if (!naRm[0] & na & res > 0) {
        return NA_REAL;
    }
    return res;
}


// [[Rcpp::export(name=".lukas.tnorm")]]
double lukas_tnorm(NumericVector vals, LogicalVector naRm)
{
    double res = 1.0;
    bool na = false;
    for (int i = 0; i < vals.size(); ++i) {
        testInvalids(vals[i]);
        if (R_IsNA(vals[i])) {
            na = true;
            ++res;
        } else {
            res += vals[i];
        }
    }
    res -= vals.size();
    if (res <= 0) {
        return 0.0;
    } else if (!naRm[0] & na) {
        return NA_REAL;
    }
    return res;
}


// [[Rcpp::export(name=".goguen.tnorm")]]
double goguen_tnorm(NumericVector vals, LogicalVector naRm)
{
    double res = 1.0;
    bool na = false;
    for (int i = 0; i < vals.size(); ++i) {
        testInvalids(vals[i]);
        if (R_IsNA(vals[i])) {
            na = true;
        } else {
            res = res * vals[i];
        }
    }
    if (!naRm[0] & na & res > 0) {
        return NA_REAL;
    }
    return res;
}


// [[Rcpp::export(name=".goedel.tconorm")]]
double goedel_tconorm(NumericVector vals, LogicalVector naRm)
{
    double res = 0.0;
    bool na = false;
    for (int i = 0; i < vals.size(); ++i) {
        testInvalids(vals[i]);
        if (R_IsNA(vals[i])) {
            na = true;
        } else if (vals[i] > res) {
            res = vals[i];
        }
    }
    if (!naRm[0] & na & res < 1) {
        return NA_REAL;
    }
    return res;
}


// [[Rcpp::export(name=".lukas.tconorm")]]
double lukas_tconorm(NumericVector vals, LogicalVector naRm)
{
    double res = 0.0;
    bool na = false;
    for (int i = 0; i < vals.size(); ++i) {
        testInvalids(vals[i]);
        if (R_IsNA(vals[i])) {
            na = true;
        } else {
            res += vals[i];
        }
    }
    if (res >= 1) {
        return 1.0;
    } else if (!naRm[0] & na) {
        return NA_REAL;
    }
    return res;
}


// [[Rcpp::export(name=".goguen.tconorm")]]
double goguen_tconorm(NumericVector vals, LogicalVector naRm)
{
    double res = 0.0;
    bool na = false;
    for (int i = 0; i < vals.size(); ++i) {
        testInvalids(vals[i]);
        if (R_IsNA(vals[i])) {
            na = true;
        } else {
            res = res + vals[i] - res * vals[i];
        }
    }
    if (!naRm[0] & na & res < 1) {
        return NA_REAL;
    }
    return res;
}


// [[Rcpp::export(name=".goedel.residuum")]]
NumericVector goedel_residuum(NumericVector x, NumericVector y)
{
    int n = x.size() > y.size() ? x.size() : y.size();
    NumericVector res(n);
    for (int i = 0; i < n; ++i) {
        int xi = i % x.size();
        int yi = i % y.size();
        testInvalids(x[xi]);
        testInvalids(y[yi]);
        if (x[xi] == 0) {
            res[i] = 1;
        } else if (R_IsNA(x[xi]) || R_IsNA(y[yi])) {
            res[i] = NA_REAL;
        } else if (x[xi] <= y[yi]) {
            res[i] = 1;
        } else {
            res[i] = y[yi];
        }
    }
    return res;
}


// [[Rcpp::export(name=".lukas.residuum")]]
NumericVector lukas_residuum(NumericVector x, NumericVector y)
{
    int n = x.size() > y.size() ? x.size() : y.size();
    NumericVector res(n);
    for (int i = 0; i < n; ++i) {
        int xi = i % x.size();
        int yi = i % y.size();
        testInvalids(x[xi]);
        testInvalids(y[yi]);
        if (x[xi] == 0) {
            res[i] = 1;
        } else if (R_IsNA(x[xi]) || R_IsNA(y[yi])) {
            res[i] = NA_REAL;
        } else if (x[xi] <= y[yi]) {
            res[i] = 1;
        } else {
            res[i] = 1-x[xi] + y[yi];
        }
    }
    return res;
}


// [[Rcpp::export(name=".goguen.residuum")]]
NumericVector goguen_residuum(NumericVector x, NumericVector y)
{
    int n = x.size() > y.size() ? x.size() : y.size();
    NumericVector res(n);
    for (int i = 0; i < n; ++i) {
        int xi = i % x.size();
        int yi = i % y.size();
        testInvalids(x[xi]);
        testInvalids(y[yi]);
        if (x[xi] == 0) {
            res[i] = 1;
        } else if (R_IsNA(x[xi]) || R_IsNA(y[yi])) {
            res[i] = NA_REAL;
        } else if (x[xi] <= y[yi]) {
            res[i] = 1;
        } else {
            res[i] = y[yi] / x[xi];
        }
    }
    return res;
}


// [[Rcpp::export(name=".invol.neg")]]
NumericVector invol_neg(NumericVector x)
{
    NumericVector res(x.size());
    for (int i = 0; i < x.size(); ++i) {
        testInvalids(x[i]);
        if (R_IsNA(x[i])) {
            res[i] = NA_REAL;
        } else {
            res[i] = 1 - x[i];
        }
    }
    return res;
}


// [[Rcpp::export(name=".strict.neg")]]
NumericVector strict_neg(NumericVector x)
{
    NumericVector res(x.size());
    for (int i = 0; i < x.size(); ++i) {
        testInvalids(x[i]);
        if (R_IsNA(x[i])) {
            res[i] = NA_REAL;
        } else if (x[i] == 0) {
            res[i] = 1;
        } else {
            res[i] = 0;
        }
    }
    return res;
}

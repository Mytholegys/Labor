#pragma once

/**
TrustedVal ald TrustedObject are (double) values or composed objects which
includes extra information about the certainty of the contained data. 
Certainty 1 means the contained values are certain and can be trusted.
Certainty 0 means the contained values are random (ony noise), do not use.

The certainty may be set by user. Operations between ThrustedValues
generate updated values and adjust certainty according to operations and values.
Certainty may increase (going closer to 1) or decrease (going closer to 0).

Author: Sergio Montenegro, January 2015

*/

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#define PLEASE_UNDEF_MIN
#endif


#include "math.h"


static const double CERTAINTY_SENSIBILITY = 0.5; // 0 .. 1: 0: no certainty adaptions

struct TrustedVal {
    double val;
    double certainty; // 0 (no infor, random data, do not use) ... 1(absoltle certain)
    double maxVal;    // limits for value, used when we have use (internally) normalized values


    TrustedVal() { val = certainty = 0; maxVal = 1.0;}
    TrustedVal(double v, double c = 1.0, double mv = 1.0) { val = v; certainty = c; maxVal = mv; }

    inline void decreaseCertainty() { 
        double newCertainty = certainty*certainty;
        certainty = certainty - (certainty - newCertainty)*CERTAINTY_SENSIBILITY;
    }

    inline void mergeVal(const TrustedVal& other) {
        double oldVal = val;
        val = (val*certainty + other.val*other.certainty) / (certainty + other.certainty);
        if (isnan(val)) { val = oldVal; certainty = 0; }
    }

    inline void mergeValAndCertainty(const TrustedVal& other) {
        mergeVal(other);

	// increase certainity according to conformity of both values
        // easier to compute using uncertainty u = (1 - certainty). 
        // range of new unvertainty = u1*u2 ... min(u1,u2)
        // smallest uncertainty (u1*u2) if both values are identical
        // biggest uncertainty (mnin(u1,u2) if values diverge by a lot
        double u1 = 1 - certainty;  
        double u2 = 1 - other.certainty;
        double distance = (val - other.val) / maxVal; // nomalized
        distance = distance*distance;
        if(distance > 1) distance = 1;
        double newCertainty = 1 - (u1*u2 + distance*(MIN(u1,u2) - u1*u2));
        if (isnan(newCertainty)) {  newCertainty = 0; }
        certainty = certainty - (certainty - newCertainty)*CERTAINTY_SENSIBILITY;
    }
};

inline TrustedVal mergeVal(const TrustedVal& a, const TrustedVal& b) {
    TrustedVal result = a;
    result.mergeVal(b);
    return result;
}
inline TrustedVal mergeValAndCertainty(const TrustedVal& a, const TrustedVal& b) {
    TrustedVal result = a;
    result.mergeValAndCertainty(b);
    return result;
}


/** 
  The given type has to implement:
  operator*(Type, double), operator+(Type, Type), operator- (Type, Type), 
  double Tpye::distance(Type other) : a value from 0 to 1
**/


template <typename Type> struct TrustedObject {
    Type val;
    double certainty; // 0 (no infor, random data, do not use) ... 1(absoltle certain)

    TrustedObject(double c = 0) { certainty = c;}
    TrustedObject(Type& t, double c = 1.0) { val = t; certainty = c;}

    inline void decreaseCertainty() { 
        double newCertainty = certainty*certainty;
        certainty = certainty - (certainty - newCertainty)*CERTAINTY_SENSIBILITY;
    }

    inline void mergeObject(const TrustedObject<Type>& other) {
        val = (val*certainty + other.val*other.certainty) * (1/(certainty + other.certainty));
    }

    inline void mergeObjectAndCertainty(const TrustedObject<Type>& other) {
        mergeVal(other);

	// increase certainity according to conformity of both values
        // easier to compute using uncertainty u = (1 - certainty). 
        // range of new unvertainty = u1*u2 ... min(u1,u2)
        // smallest uncertainty (u1*u2) if both values are identical
        // biggest uncertainty (mnin(u1,u2) if values diverge by a lot
        double u1 = 1 - certainty;  
        double u2 = 1 - other.certainty;
        double distance = val.distance(other.val);
        distance = distance*distance;
        double newCertainty = 1 - (u1*u2 + distance*(MIN(u1,u2) - u1*u2));
        if (isnan(newCertainty)) {  newCertainty = 0; }
        certainty = certainty - (certainty - newCertainty)*CERTAINTY_SENSIBILITY;
    }
};



#ifdef PLEASE_UNDEF_MIN
#undef MIN
#endif


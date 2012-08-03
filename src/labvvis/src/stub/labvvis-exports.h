//
//  labvvis-exports.h
//  labvvis
//
//  Created by Johnny Lai on 8/2/12.
//
//

#ifdef __cplusplus
extern "C" {
#endif

void plus_saturated(void * const in1, void * const in2, void * const out);
void minus_saturated(void * const in1, void * const in2, void * const out);
void multiplies(void * const in1, void * const in2, void * const out);
void divides(void * const in1, void * const in2, void * const out);
void modulus(void * const in1, void * const in2, void * const out);

void equal_to(void * const in1, void * const in2, void * const out);
void not_equal_to(void * const in1, void * const in2, void * const out);
void greater(void * const in1, void * const in2, void * const out);
void greater_equal(void * const in1, void * const in2, void * const out);
void less(void * const in1, void * const in2, void * const out);
void less_equal(void * const in1, void * const in2, void * const out);

void logical_and(void * const in1, void * const in2, void * const out);
void logical_or(void * const in1, void * const in2, void * const out);

void bitwise_and(void * const in1, void * const in2, void * const out);
void bitwise_or(void * const in1, void * const in2, void * const out);

void testObjectiveC(const int in, int *out);

#ifdef __cplusplus
}
#endif
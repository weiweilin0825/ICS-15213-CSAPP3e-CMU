/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
    //(X or Y) & (X and Y)
    return (~(~x & ~y) & ~(x & y));
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
    // move 1 for 31 digits to get 10000...0
    int x = 0x01;
    return x << 31;

}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
    // Check whether y is Tmax or -1, if True y3 = 0;
    int y1 = x + 1;
    int y2 = ~x;
    int y3 = y1 ^ y2;

    // Check whether x is -1, if x == -1, z2 = 1;
    int z1 = ~x;
    int z2 = !z1;
    
    return !(y3 | z2);
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
    // y1 == 1010 1010;
    int y1 = 0xAA;
    // y2 == 1010 1010 1010 1010
    int y2 = (y1 << 8) + y1;
    int y3 = (y2 << 16) + y2;
    return !((y3 & x) ^ y3);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
    return (~x + 1);
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
    // 30 = 0011 0000
    // 39 = 0011 1001
    int y1 = !((x >> 4) ^ 3);
    int y2 = (x & 8) >> 3;
    int y3 = (x & 4) >> 2;
    int y4 = (x & 2) >> 1;
    return y1 & !(y2 & (y3 | y4));
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
    int flag = !x;
    int a;
    int b;
    flag = ~flag; //x != 0 => 1111 or x = 0 => 1110
    flag = ~(flag + 1);
    a = (flag & y);
    b = (~flag & z);
    return (a | b);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
    // case 1: different sign and y >= x
    int yh = y >> 31;
    int xh = x >> 31;
    int z1, z2, z3, sum;
    yh = !yh; // if yh == 000...000(positive), !yh = 1
    xh = !xh; // if xh == 111...111(negative), !xh = 0
    z1 = yh & !xh ; //only consider y >= 0, x < 0 is true

    // case 2: same sign for y and x
    // x - y <= 0;
    // x + (-y) <= 0;
    // -x + y >= 0 ->1;
    sum = (~x + 1) + y;
    // if sum < 0; it become 111...111, not z = 0, otherwise it become 1;
    sum = sum >> 31;
    z2 = !sum & !(yh ^ xh); 
    
    // case 3: same sign and x == Tmin
    z3 = !((~x + 1) ^ x) & !(yh ^ xh);

    return (z1 | z2 | z3);
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
    // want to made 0 become 000...000 then it will become 1 after plus 1
    // 1 become 111...111 then it will become 0 after plus 1
    // the first digit of -x | x will become 1 because it will change sign except for 0
    // the first digit when x = Tmin will also become 1
    // shift right for 31 digits then it will become the desired outcome
    int x1 = (~x + 1) | x;
    x1 = x1 >> 31;
    return x1 + 1;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
    // 0 and -1 are special case (will be all 0 and 1)
    // if x < 0, change it to -x then count the left most 1 location
    // location plus 1 will be the result?
    //
    // change x to positve first
    int neg = x >> 31;
    int cnt, mask, left0, plus, zero;
    x = (~neg & x) | (neg & (~x + 1));

    cnt = 0;
    mask = (~0) << 16;
    // if x first left 1 > 16, left 1 will be 1111...111
    left0 = ~(!(x & mask)) + 1;

    mask = (mask << 8) | (left0 & (mask >> 8));
    cnt = (~left0 & (cnt + 16)) | cnt;

    left0 = ~(!(x & mask)) + 1;
    mask = (mask << 4) | (left0 & (mask >> 4));
    cnt = (~left0 & (cnt + 8)) | cnt;
    
    left0 = ~(!(x & mask)) + 1;
    mask = (mask << 2) | (left0 & (mask >> 2));
    cnt = (~left0 & (cnt + 4)) | cnt;

    left0 = ~(!(x & mask)) + 1;
    mask = (mask << 1) | (left0 & (mask >> 1));
    cnt = (~left0 & (cnt + 2)) | cnt;

    left0 = ~(!(x & mask)) + 1;
    mask = mask | (left0 & (mask >> 1));
    cnt = (~left0 & (cnt + 1)) | cnt;
    cnt = cnt + 1;

    // see if there is 1 in the right part
    mask = ~mask;
    plus = ~(!(x & mask)) + 1;
    plus = ~neg | (neg & ~plus);

    zero = !x;
    zero = ~zero + 1;
    cnt = (~zero & ((plus & (cnt + 1)) | (~plus & cnt))) | (zero & cnt);

    return cnt;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
    // three conditions
    // 1. exp normalized
    // 2. exp = 000...000 
    // 3. exp = 111...111 return uf
    // seperate the s, exp and frac part then compose them in the end
    unsigned umax = 1 << 31;
    unsigned exp = (uf << 1) >> 24; //push exp to right most 8 digits
    unsigned sign = uf & umax; //get the first digit
    unsigned fracmask = (0xff << 8) + 0xff; 
    unsigned frac;
    int nan, all_zero;
    fracmask = (fracmask << 7) + 0x7f; //make right most 23 digits become 1

    frac = uf & fracmask;
    exp = exp & 0xff;
    nan = !(exp ^ 0xff); //check if uf is Nan
    if (nan) return uf; // if exp all 1 then Nan
    
    all_zero = !(exp ^ 0); //check all 0 case
    if (all_zero) {
        frac = frac << 1; // if exp = 000...000 then left shift frac to get scale2
    }
    else{
        exp = exp + 1;
        exp = exp << 23;    // if exp == others, exp + 1 to get scale2;
    }
    return sign + exp + frac;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */

int floatFloat2Int(unsigned uf) {
    // Implement round to even
    // Cant operate as floating point number, how to do that in unsigned/int ? 
    // first find out the sign and exp part
    int ans, positive, exp, frac, max;
    
    max = 0x80000000;
    positive = !(uf & max);
    exp = (uf >> 23) & 0xff;
    exp = exp - 127;

    if (exp  > 30) return max; //if uf == Nan or Inf
    if (exp < 0) return 0;
    // find out the frac part and find out its floating point part
    frac = (uf & 0x7fffff) << 7;
    frac = frac + 0x40000000;
    // shift it find out the answer
    frac = frac >> (30 - exp); 

    ans = frac;

    if (!positive) ans = -frac;

    return ans;
}
/*
int floatFloat2Int(unsigned uf) {
    // Implement round to even
    // Cant operate as floating point number, how to do that in unsigned/int ? 
    // first find out the sign and exp part
    int ans, positive, exp, roundup, half, even, frac, max, upper;
    
    roundup = 0;
    half = 0;
    max = 0x80000000;
    upper = 0x20000000;
    positive = !(uf & max);
    exp = (uf >> 23) & 0xff;
    exp = exp - 127;

    if (exp  > 30) return max; //if uf == Nan or Inf
    if (exp < -1) return 0;
    // find out the frac part and find out its floating point part
    frac = (uf & 0x7fffff) << 7;
    if (frac > upper) roundup = 1;
    if (frac == upper) half = 1;
    frac = frac + 0x40000000;
    // shift it find out the answer
    frac = frac >> (30 - exp);
    
    even = !(frac & 0x1);
    if (roundup || (!even && half)) frac ++;
    if (!positive) ans = -frac;
    else ans = frac;

    return ans;
}
*/
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
    int exp;
    exp = x + 127;
    if (exp < 0) return 0;
    else if (exp > 255) return 0x7f800000;
    exp = exp << 23;
    return exp;
}

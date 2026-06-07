#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#define FIXED_SHIFT 12

// Convert integer to 12-bit fixed point
#define I2F(i) ((i) << FIXED_SHIFT)

// Convert 12-bit fixed point to integer (rounding towards zero)
#define F2I(f) ((f) >> FIXED_SHIFT)

#define F2Ix(f) ((f) * 0x10 >> 0x10)

#define I2F_RND(i) (i > 0) ? (I2F(i) + 0.5f) : (I2F(i) - 0.5f)

#endif // FIXED_POINT_H
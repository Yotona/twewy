/**
 * @file fx_matrix.h
 * @brief Header for FX matrix operations, providing 3x3, 4x3, and 4x4 matrix manipulation functions for Nintendo DS.
 *
 * This module offers a comprehensive set of functions for creating, transforming, and manipulating
 * matrices used in 3D graphics calculations. It includes operations for identity matrices, scaling,
 * rotation, concatenation, inversion, and specialized transformations like look-at and orthographic
 * projections. All operations use fixed-point arithmetic suitable for the Nintendo DS hardware.
 */

#ifndef FX_MATRIX_H
#define FX_MATRIX_H

#include <NitroSDK/fx/fx_vector.h>
#include <types.h>

/**
 * @typedef Mtx33
 * @brief 3x3 matrix struct for 3D rotations and scaling transformations.
 *
 * This struct provides access to a 3x3 matrix used for 3D transformations
 * that preserve the origin (rotations, scaling, reflections). All matrix elements
 * are 32-bit fixed-point numbers with 12 bits of fractional precision, allowing
 * values from approximately -8.0 to 7.999...
 *
 * The matrix can be accessed via struct members (_00 through _22).
 */
typedef struct {
    s32 _00, _01, _02;
    s32 _10, _11, _12;
    s32 _20, _21, _22;
} Mtx33;

/**
 * @typedef Mtx43
 * @brief 4x3 matrix struct for 3D affine transformations including translation.
 *
 * This struct represents a 4x3 transformation matrix commonly used in 3D graphics
 * for affine transformations (rotations, scaling, translations). The matrix has
 * 4 rows and 3 columns, where the last row represents translation components.
 * All values use 32-bit fixed-point arithmetic with 12-bit fractional precision.
 *
 * Access via struct members _00-_32 for individual elements.
 */
typedef struct {
    s32 _00, _01, _02;
    s32 _10, _11, _12;
    s32 _20, _21, _22;
    s32 _30, _31, _32;
} Mtx43;

/**
 * @typedef Mtx44
 * @brief 4x4 matrix struct for 3D projective and general transformations.
 *
 * This struct provides access to a full 4x4 transformation matrix used for
 * perspective projections, general linear transformations, and operations
 * in homogeneous coordinates. Essential for 3D graphics pipelines that
 * require projective transformations.
 *
 * Access via struct members _00-_33.
 */
typedef struct {
    s32 _00, _01, _02, _03;
    s32 _10, _11, _12, _13;
    s32 _20, _21, _22, _23;
    s32 _30, _31, _32, _33;
} Mtx44;

/**
 * @brief Creates an identity 3x3 matrix.
 *
 * Initializes the destination matrix to the identity transformation, where
 * diagonal elements are set to 1.0 (0x1000) and all others to 0. The identity
 * matrix represents no transformation.
 *
 * @param dest Pointer to the 3x3 matrix to initialize as identity.
 */
void Mtx_Identity33(Mtx33* dest);

/**
 * @brief Scales a 3x3 matrix by independent X, Y, and Z factors.
 *
 * Applies scaling transformation to the source matrix, multiplying each column
 * by the corresponding scale factor. The result is stored in the destination matrix.
 * Scale factors are in fixed-point format (0x1000 = 1.0).
 *
 * @param src Pointer to the source 3x3 matrix.
 * @param dest Pointer to the destination 3x3 matrix.
 * @param scaleX X-axis scaling factor.
 * @param scaleY Y-axis scaling factor.
 * @param scaleZ Z-axis scaling factor.
 */
void Mtx_Scale33(const Mtx33* src, Mtx33* dest, s32 scaleX, s32 scaleY, s32 scaleZ);

/**
 * @brief Creates a 3x3 matrix representing rotation around the Y-axis.
 *
 * Initializes the matrix with the rotation transformation for the given sine
 * and cosine values of the rotation angle. This creates a rotation matrix
 * that rotates points around the Y-axis by the specified angle.
 *
 * @param matrix Pointer to the 3x3 matrix to initialize.
 * @param sin Sine of the rotation angle (fixed-point).
 * @param cos Cosine of the rotation angle (fixed-point).
 */
void Mtx_YRot33(Mtx33* matrix, s32 sin, s32 cos);

/**
 * @brief Creates an identity 4x3 matrix.
 *
 * Sets the destination matrix to the identity transformation for 4x3 matrices,
 * with the 3x3 rotation part as identity and translation components as zero.
 *
 * @param matrix Pointer to the 4x3 matrix to initialize.
 */
void Mtx_Identity43(Mtx43* matrix);

/**
 * @brief Copies a 4x3 matrix to a 4x4 matrix with homogeneous coordinates.
 *
 * Converts a 4x3 affine transformation matrix to a 4x4 matrix by copying
 * the existing elements and adding [0, 0, 0, 1] as the last row for
 * homogeneous coordinate representation.
 *
 * @param src Pointer to the source 4x3 matrix.
 * @param dest Pointer to the destination 4x4 matrix.
 */
void Mtx_Copy43To44(const Mtx43* src, Mtx44* dest);

/**
 * @brief Scales a 4x3 matrix by independent X, Y, and Z factors.
 *
 * Applies scaling to the rotational part of the 4x3 matrix while preserving
 * the translation components. Each column of the 3x3 rotation matrix is
 * scaled by the corresponding factor.
 *
 * @param src Pointer to the source 4x3 matrix.
 * @param dest Pointer to the destination 4x3 matrix.
 * @param scaleX X-axis scaling factor.
 * @param scaleY Y-axis scaling factor.
 * @param scaleZ Z-axis scaling factor.
 */
void Mtx_Scale43(const Mtx43* src, Mtx43* dest, s32 scaleX, s32 scaleY, s32 scaleZ);

/**
 * @brief Computes the inverse of a 4x3 transformation matrix.
 *
 * Calculates the inverse transformation matrix. If the matrix is singular
 * (non-invertible), returns -1. Otherwise returns 0 and stores the inverse
 * in the destination matrix. Uses Gaussian elimination with fixed-point arithmetic.
 *
 * @param src Pointer to the source 4x3 matrix.
 * @param dest Pointer to the destination 4x3 matrix for the inverse.
 * @return 0 on success, -1 if the matrix is singular.
 */
s32 MTX_Inverse43(const Mtx43* src, Mtx43* dest);

/**
 * @brief Multiplies two 4x3 transformation matrices.
 *
 * Performs matrix multiplication dest = a * b, where the result represents
 * the composition of transformations. Matrix multiplication is performed
 * with proper handling of the translation components.
 *
 * @param a Pointer to the left-hand matrix.
 * @param b Pointer to the right-hand matrix.
 * @param dest Pointer to the destination matrix for the result.
 */
void Mtx_Concat43(const Mtx43* a, const Mtx43* b, Mtx43* dest);

/**
 * @brief Creates a view transformation matrix from camera parameters.
 *
 * Generates a 4x3 view matrix that transforms world coordinates to camera space,
 * given the camera position, up direction, and target position. The resulting
 * matrix represents the inverse of the camera's transformation.
 *
 * @param camPosition Position of the camera in world space.
 * @param upDirection Up direction vector of the camera.
 * @param targetPosition Position the camera is looking at.
 * @param matrix Pointer to the 4x3 matrix to store the view transformation.
 */
void Mtx_LookAt(const Vec* camPosition, const Vec* upDirection, const Vec* targetPosition, Mtx43* matrix);

/**
 * @brief Creates an identity 4x4 matrix.
 *
 * Initializes the matrix to the 4x4 identity matrix, representing no transformation
 * in homogeneous coordinates.
 *
 * @param matrix Pointer to the 4x4 matrix to initialize.
 */
void Mtx_Identity44(Mtx44* matrix);

/**
 * @brief Copies a 4x4 matrix to a 4x3 matrix, discarding homogeneous row.
 *
 * Converts a 4x4 projective matrix to a 4x3 affine matrix by copying the
 * upper-left 4x3 portion and discarding the last row (homogeneous coordinates).
 *
 * @param src Pointer to the source 4x4 matrix.
 * @param dest Pointer to the destination 4x3 matrix.
 */
void Mtx_Copy44To43(const Mtx44* src, Mtx43* dest);

/**
 * @brief Multiplies two 4x4 transformation matrices.
 *
 * Performs matrix multiplication dest = a * b for 4x4 matrices, suitable for
 * composing projective transformations and other 4D operations.
 *
 * @param a Pointer to the left-hand 4x4 matrix.
 * @param b Pointer to the right-hand 4x4 matrix.
 * @param dest Pointer to the destination 4x4 matrix.
 */
void Mtx_Concat44(const Mtx44* a, const Mtx44* b, Mtx44* dest);

/**
 * @brief Creates an orthographic projection matrix with scaling.
 *
 * Generates a 4x4 orthographic projection matrix that maps a viewing volume
 * to normalized device coordinates. The 'W' variant includes a scaling factor
 * for the W component, useful for certain projection setups.
 *
 * @param top Top coordinate of the viewing volume.
 * @param bottom Bottom coordinate of the viewing volume.
 * @param left Left coordinate of the viewing volume.
 * @param right Right coordinate of the viewing volume.
 * @param near Near clipping plane distance.
 * @param far Far clipping plane distance.
 * @param scale Scaling factor for the projection.
 * @param mtx Pointer to the 4x4 matrix to store the projection.
 */
void Mtx_OrthoW(s32 top, s32 bottom, s32 left, s32 right, s32 near, s32 far, s32 scale, Mtx44* mtx);

#endif // FX_MATRIX_H
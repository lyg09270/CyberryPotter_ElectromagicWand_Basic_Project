#include "nnom.h"

/* Weights, bias and Q format */
#define TENSOR_CONV1D_KERNEL_0 {-26, 43, 62, 15, -12, -24, 71, -42, 45, -34, 65, -85, -27, 43, -35, -47, -31, -26, 39, 44, 53, 64, 0, 9, 80, 16, 41, 59, 10, 25, 19, -27, -44, 14, -12, -11, -21, -64, -6, -45, 0, 27, 4, -79, 76, 2, -17, 8, -19, -40, 19, -53, -2, 26, 27, -30, -26, 75, -30, 70, 89, 12, 28, -19, 11, -62, 7, 15, -82, -65, 7, 33, -43, 75, 39, -33, -10, 32, 37, 65, 47, -45, 33, 15, -52, -19, 7, -35, 49, -22, -62, 5, 13, -4, -8, -14, -35, -36, 24, 30, 17, 3, 53, -51, 77, -52, -32, 77, 70, -21, -61, 97, 30, 42, 60, 51, 31, -32, -23, -39, 6, 2, 22, 25, -61, -5, 26, 8, -26, -36, -21, 52, -26, -61, -7, -28, -38, 74, -67, -10, 64, -65, -24, 60, -28, 62, -32, 51, -26, -68, -59, 63, 26, -90, -74, -52, -40, 35, -51, -62, 37, 66, 48, 22, 62, 10, -38, 41, -11, 38, 62, -67, -47, -22, -78, -70, 41, -57, -11, 23, 29, -24, -55, -78, -65, -52, -17, 43, 38, 37, -27, 21, -9, 24, -44, 50, -38, -43, -32, 13, 30, -53, 27, -17, -43, 12, 25, -31, -7, 35, 43, -57, 24, -51, 8, -54, 33, -32, -51, 37, 70, 49, -32, 45, 37, 67, 8, 23, 33, 28, -23, 80, 59, -5, 6, -38, -82, 20, 68, 16, 30, 35, -37, 31, 25, -27, -5, -68, 6, 29, -54, 43, 39, -29, 62, 51, -29, 17, -1, -9, 46, 2, -2, -36, -18, -9, 56, 27, 22, 63}

#define TENSOR_CONV1D_KERNEL_0_DEC_BITS {8}

#define TENSOR_CONV1D_BIAS_0 {-8, 42, 7, 4, 20, -2, 29, -11, 70, 38, -8, 13, 76, 24, -19, 15, -26, 64, -9, -7, 40, 3, -19, -18, -7, -19, 18, 61, 0, -8}

#define TENSOR_CONV1D_BIAS_0_DEC_BITS {9}

#define CONV1D_BIAS_LSHIFT {4}

#define CONV1D_OUTPUT_RSHIFT {9}

#define TENSOR_CONV1D_1_KERNEL_0 {46, -21, -33, -14, -33, -10, -44, 22, -5, 63, 4, -8, -37, 67, 57, -27, 20, 22, 9, 63, 29, 26, 16, 7, -18, -52, -45, 28, -37, -37, -54, 20, 5, 33, -33, 13, -49, 7, 50, -35, 35, 17, -21, 3, 49, -22, 10, -2, -33, 21, 3, -54, 14, -39, 4, -29, -45, 34, -46, -8, -17, -35, 19, 16, 10, -26, 14, -59, 68, 2, 10, 2, 42, -56, 22, -54, 25, 41, -44, 1, -55, -39, 15, 19, -15, -17, -19, -13, 23, -25, -26, 20, 6, 16, 52, 9, 28, 46, -34, 58, 18, 17, -43, -43, -4, 10, 12, 30, 7, 15, 25, 12, 57, -49, 16, 6, -65, -63, 3, 14, -50, -49, -66, 16, 51, -39, -58, -5, 69, 52, 1, -1, -11, 18, -7, 20, 31, 4, -30, 43, -54, 17, 13, 47, 51, 29, -31, -6, 2, 7, -6, -27, -43, 21, 29, 13, -35, -3, 37, -15, -1, -19, -48, -22, 43, 68, 34, 37, 15, -28, -59, 26, -10, -35, -7, 7, -19, -39, -39, -19, 25, -26, 35, -5, -14, 36, 9, -51, -30, -32, 2, -17, 55, 36, 17, -36, -65, 17, -52, 40, -17, 40, 49, 51, -7, -24, 42, 27, 4, -26, 35, -19, -23, 69, 67, 30, 51, 11, 37, 14, -11, 20, 38, 21, 43, 17, -17, 45, -25, 24, -4, 46, 15, -19, 34, 61, -2, -40, -9, 29, -54, -27, 13, -5, -15, -10, 19, 8, -30, 20, 19, -55, 20, -58, 18, -38, -18, -58, -37, 53, -34, 1, 10, 9, 15, 52, 37, 47, -53, -47, 42, 44, -36, -47, -12, -39, 38, 4, -15, 69, 23, -11, -54, -4, -26, -10, -48, 27, -14, 11, 50, 22, 58, 24, -55, -64, -31, -61, 41, 26, 2, 51, -18, -28, -15, 36, -7, -40, -31, -26, 60, -27, 33, 27, 26, 43, 40, 7, 11, 15, 26, 10, 36, 47, 15, 50, -7, 22, -30, -40, 50, 0, 25, 15, 21, -4, -21, -52, 39, -18, 62, 28, -31, -35, 28, 58, 23, -2, 2, 5, 28, -4, 24, 10, -26, 40, 21, 47, 3, -5, 49, 19, 53, 56, 53, 29, 71, 3, -13, -57, -31, 70, 23, 2, -42, 47, 20, 20, 44, 30, -27, 42, 3, -30, 4, 74, 7, -25, -6, 38, 63, 12, 25, 28, -14, 8, 80, 26, 60, -22, -31, 62, 60, -21, -33, 53, 34, -40, -15, -22, -64, -13, 35, 38, 56, 30, 0, 1, 33, -4, 26, -34, 31, -10, 38, -34, 24, 9, 43, -15, 31, 36, -11, -36, 11, 40, 43, -69, 6, -21, 0, -32, -10, -43, 63, 37, 16, -11, 55, -9, -30, 34, 10, -49, -13, -11, -24, 41, -21, 51, 1, -39, -15, 50, -29, -41, -16, -13, 28, 8, -26, -22, -4, 17, -48, -18, -33, 37, -51, -9, 28, -52, -32, 38, 69, 17, -14, 40, -40, -13, 18, 4, 34, 18, 8, 4, 10, 19, -62, 3, 56, -50, 20, -49, -11, 11, -51, 72, -12, -23, -5, -11, 48, -47, 69, -24, 25, -16, -5, -11, 2, 7, -11, 24, -4, -12, 14, 24, 11, -17, 30, 53, -54, -46, -32, -32, -11, 39, -10, -40, 75, 25, 2, -50, 12, 14, -9, 63, -47, 21, -15, 33, -30, 1, 5, -6, 22, -15, -13, -15, -8, 15, 1, 14, 16, 8, 28, 5, -1, 48, 10, 40, -13, 10, 15, 11, -7, 63, -12, -28, 16, 39, 7, -47, 47, 60, 8, -31, 48, 49, 27, 10, 31, 12, 1, 54, -28, 44, 10, 32, -33, 15, 11, -11, 32, 76, 45, 33, -39, -28, -36, 14, -45, -38, 52, 63, 41, 61, 12, -3, -6, 6, -25, 26, -16, -44, 59, 54, 18, 18, -20, -6, -21, 24, 52, 2, 24, -40, 75, 86, 28, 32, -53, -12, -39, 42, -40, 1, -8, 65, -25, -39, -4, 25, -58, 23, -50, 49, 9, -24, 58, 23, 15, 32, -7, -19, -27, 18, 71, 32, 31, -18, 55, 38, 14, -25, -48, 36, -31, -30, 60, 13, -32, -12, 10, -54, -30, -4, -8, -42, -38, 12, -62, 29, 54, -2, -31, -3, 65, -27, -13, 11, 18, 0, -2, 8, 46, 54, -54, 7, -50, -32, -25, -29, 14, 21, 30, 60, 47, -18, -22, 40, 16, -36, 40, 45, -21, 31, 54, 57, -9, -63, 32, 31, -8, 42, -32, 13, -10, 15, -11, -60, 4, 33, 51, 20, 19, -8, -36, 23, -45, 14, -6, -58, -36, -15, -18, -3, 39, 52, -42, -26, -35, 36, -59, -5, 38, -12, -50, -67, -18, -61, 21, -55, -10, 34, 35, 40, 10, 30, -22, -2, 22, -14, -1, -17, -38, -37, -17, 53, -10, -26, 20, 12, -14, -12, 4, 9, 27, -21, 28, -34, -40, -57, 36, -32, -10, 59, 4, 0, -18, -45, 26, 67, 24, 22, -5, 5, -35, -22, 16, -23, 38, 6, -46, -24, 6, 15, -18, 39, -50, 73, -1, 27, -26, 23, -8, -17, 13, 24, 34, -8, 53, -20, -25, -63, 77, 24, -34, -32, -1, 23, 71, -21, -44, -39, -29, 14, 21, 40, -56, 3, -54, -45, -16, 78, 25, 71, -48, 7, -50, -12, -16, 72, -15, 2, -11, 36, -57, 14, -22, -46, 20, -52, 3, -63, -41, -30, 3, -9, -7, 31, -4, 44, -25, -3, 66, 10, 12, 36, 13, 15, 45, 3, 1, -28, 32, 66, -31, -10, 62, 34, 19, -24, 61, 2, -43, 56, -62, 36, 54, -15, -26, -23, -25, -18, -36, -71, -15, -21, -31, -5, -4, 22, 51, 54, 1, 3, -19, 24, -8, -39, -55, 8, -34, 19, -48, 56, -10, 65, 47, 18, 47, -32, 6, 38, -39, -35, -10, -38, 9, 40, -2, 32, 37, 23, -20, 24, -18, 70, 44, -29, -59, 55, -27, 58, 33, -1, -65, 5, 8, -50, 41, 37, 2, 4, 30, 27, -58, -65, -22, -41, 6, 56, -10, -52, 28, 12, -1, -4, -31, -17, 68, 43, -8, 49, -5, -60, 34, 47, -4, 3, 35, 46, 51, 16, -19, 71, 55, -22, 39, -19, 54, 14, 32, -59, 68, -79, 10, -39, 15, -44, 42, 50, -49, 5, -28, -7, -35, 44, -27, 51, -34, 35, 37, 5, 18, 64, 49, -26, 44, -67, 41, -9, -46, 23, 4, -21, -16, -13, 35, -19, 6, 10, 3, 5, -36, 49, 9, -4, 49, 21, 3, -22, 6, 49, -22, -6, -9, -39, 17, -65, -22, -42, -14, 77, -15, 4, -18, -1, 32, -19, -1, 34, -26, 51, -54, 8, -2, 56, 11, -53, 16, -27, -20, 5, -18, -68, -20, -10, 37, -24, 33, 23, 4, -12, -39, -57, 68, 23, 63, -11, -33, 16, -25, 58, 15, 47, -46, 63, -30, 33, 61, -23, 38, -50, -50, -31, 45, 49, -37, 6, 36, 1, 18, -9, -8, -5, 27, 36, 39, 23, -35, 47, -15, 4, 31, 35, -49, 27, 16, -31, 46, -8, 8, 31, -3, -6, 40, 2, 12, 54, -44, -6, 38, 30, -33, -17, -20, -7, -26, -44, 10, 5, -8, -32, 71, 63, 2, 59, 47, 6, 51, 23, 10, -44, -12, -61, -32, 30, -20, -24, -16, -19, -49, 54, 4, 34, 26, -3, -13, -26, -54, -39, -6, 24, 75, 44, 9, -27, 23, -10, 41, 59, -53, 12, -6, -66, 17, 44, 46, 43, -18, -34, -20, 40, 14, 69, 27, -9, -5, 54, -35, -23, -44, -7, 18, 60, -35, 12, -14, -8, 29, 19, -59, -39, 50, -58, 41, -33, -32, -16, 31, 6, -21, -13, 20, -28, 3, -61, -30, 71, 70, 8, -35, 34, -30, 42, 26, 11, -42, 29, -13, 28, 12, -1, -8, -15, -17, 70, -37, -75, 25, 40, 28, 28, 24, -57, 39, 50, -18, 67, -15, -15, 37, 46, -27, -27, -29, -22, 5, -22, 26, 23, 3, 14, -46, -17, -7, 13, -51, -21, -56, -27, -18, 23, 0, -50, -31, -17, 37, 0, -52, 3, -10, 17, -56, 60, -40, 32, 28, 43, -11, 68, 10, 52, -33, 23, -29, -8}

#define TENSOR_CONV1D_1_KERNEL_0_DEC_BITS {8}

#define TENSOR_CONV1D_1_BIAS_0 {8, -24, -2, 0, -24, 22, 10, 62, 50, 2, -24, 37, -11, 25, 67}

#define TENSOR_CONV1D_1_BIAS_0_DEC_BITS {9}

#define CONV1D_1_BIAS_LSHIFT {3}

#define CONV1D_1_OUTPUT_RSHIFT {8}

#define TENSOR_DENSE_KERNEL_0 {43, -31, -59, -15, -42, -4, -32, -20, -49, 12, -31, 54, 56, 80, -25, -44, -28, 42, -33, 63, 67, 12, -24, 35, -28, -50, 20, 53, 33, 38, 60, 55, 42, 78, -1, 33, -65, 38, -70, -27, -65, -46, -92, -30, -77, -12, 4, -79, -64, -14, -71, 41, 42, -32, 5, 15, 30, 4, 64, -63, -3, 56, -38, -7, -61, 0, 31, -3, -71, -19, 20, -58, 11, -72, -66, -33, -74, -81, -17, 14, 19, -25, -18, 34, -62, -37, -78, -3, 5, -4, 24, 4, 56, 68, 10, 88, -18, -84, -45, -31, 33, -35, -73, 15, 68, 5, -2, 28, -6, -18, -61, 8, 64, 46, 89, -22, -47, 64, -71, -19, 75, -36, -14, 23, -29, -69, 76, -63, 51, 11, -26, -20, -85, 34, 8, -10, 68, -50, 51, 54, 54, 19, -23, -17, -3, 16, -29, 21, 18, 22, -39, 29, 1, 12, -57, -42, -47, -80, -47, -33, 26, -29, 53, -81, -52, -67, 4, -96, 41, -48, -29, -39, -2, 75, 31, 1, 40, -99, -23, -73, 41, -82, 12, 32, -4, 39, 18, -76, -80, -87, -80, -13, -12, -6, 26, 10, -31, -78, 38, -31, -73, -63, -87, -85, 74, 46, 58, 3, 62, -51, 6, 11, -50, -40, -67, 36, 34, -36, -23, -76, 27, -60, -63, -6, 30, 24, 27, -62, 3, -11, 1, -88, -46, -73, 21, -19, 24, 25, -13, 48, -61, -32, 7, 23, 20, -29, -73, 22, -76, -34, -60, 33, 2, -54, 18, 20, 4, 53, -38, 40, 26, -52, -31, -40, 3, 44, 81, -14, 42, -30, 9, -50, -45, 33, -61, -76, 52, 43, -66, 36, -90, 2, 17, 70, -54, 38, -43, -21, -36, -18, -47, -5, -2, -8, -20, 28, -46, 41, -50, 29, 46, -7, 64, 21, -65, -39, 11, 20, 78, -51, 78, -29, 20, -59, 17, 25, -78, 22, 41, -97, -97, -64, 10, 49, 47, -80, -27, 12, -21, -28, -46, -44, 37, -50, 23, -69, 27, 43, 25, 49, -22, 7, -27, 61, -53, 18, -55, 56, -21, -11, -18, 23, 16, 14, 4, 44, 56, -58, -39, 46, 52, -28, 57, -58, 6, 58, -27, 49, 13, 2, 9, -56, -20, -31, -77, 11, 12, -53, -41, 12, -75, -70, 54, -68, 22, -36, -6, -20, 31, -75, 15, -36, -90, 19, -35, 24, 36, -58, -2, -61, 57, -43, 43, -16, -22, 34, 9, -28, -71, -50, -5, -42, -84, 54, 56, -28, 16, -16, -52, -70, -31, -105, 58, -55, -50, -47, -60, 26, 2, -92, -57, 23, -40, 5, -20, -8, -34, 39, 11, -54, -74, -68, -21, -21, 29, -1, -27, 29, -37, -53, 50, -17, -33, -65, 0, -44, 12, -51, 53, -12, 79, 30, -46, 19, 26, 14, 46, 28, 52, 51, -52, -15, 38, 56, 67, -11, 29, 10, -14, -12, 2, 71, -72, 30, 8, -24, -19, -21, 48, 66, 35, 36, -52, 24, 60, -34, 63, 8, -18, -75, -16, -9, 9, -47, 60, -5, 28, -45, 54, 53, -37, 43, -43, 71, 66, -55, -23, -43, 2, 79, -9, 76, -3, -10, -57, -14, 0, 62, 7, 64, -40, 57, -48, -35, 24, -16, 30, 51, -85, 1, -46, -31, -92, -32, -1, -35, 66, 12, -53, 25, -32, 36, 23, 51, -78, 76, -43, -47, -27, 32, -23, -22, -19, -59, 5, 21, -79, -69, 18, -6, -74, -42, -21, -2, -74, -28, -75, -59, 45, 41, -39, -13, -69, -42, 7, -9, 72, -2, -75, -69, 42, 75, -79, -15, -89, -9, -84, 65, -6, -40, 13, 19, -51, 38, -5, 24, -79, 23, -65, 6, -23, -15, -35, -51, 78, 3, -51, 65, 19, -54, 48, -59, 84, 30, -60, 55, -46, -74, -27, 31, -5, -76, -92, 27, -30, 15, -78, -2, -7, -87, 27, -33, 13, 55, 60, 51, -9, -20, -55, -98, 12, -25, -87, -6, -13, -74, -78, -17, 15, -24, -72, -38, -33, -17, -5, -6, 6, 5, -34, -5, -60, -22, 10, -39, 67, 15, 26, 41, 33, -19, 59, -5, 30, -68, -59, 1, 55, 48, -5, 26, -76, -61, 28, -34, -59, -25, 7, 55, -12, -36, 58, 5, 28, 39, 56, 31, 43, -20, -76, 10, -54, 25, 2, -35, -76, 85, -48, -72, -38, 19, -21, 47, -32, -34, 57, 46, 51, 29, -13, -23, 19, -33, -65, 59, -7, 34, -42, -19, -47, -22, 0, -22, -2, -55, 10, 0, -2, 52, 32, -40, -44, -34, -45, 21, 89, -26, -101, -27, -2, 57, -14, 61, -10, 67, -49, -6, -56, -90, 62, 41, -76, -21, -19, -23, 71, 6, -76, -85, -68, 2, -5, -12, 17, 35, 1, 26, -11, 36, 48, 15, 16, -2, 41, 25, -13, -66, -78, 8, -32, 24, -17, 7, -7, -102, 5, -55, 1, 9, -107, -20, 4, 24, -59, -62, -43, -38, -63, -57, -54, -94, 58, -27, 30, 70, 27, 36, 21, 3, -45, -50, -22, 2, -102, -11, -23, -88, 24, 28, 16, 22, -44, 33, -13, -1, 20, -25, 43, 57, 8, -91, -5, 8, -45, -7, -54, 13, -80, -7, 62, 3, -52, -52, -56, 11, 38, -54, -42, -18, -62, 43, 53, -41, -22, -18, 57, -58, -66, -27, -74, 6, 58, 37, -62, 1, 1, 7, -69, -58, -61, -102, 8, 48, 43, 24, 20, -1, -53, 28, -65, -34, 0, -51, 21, 23, -31, 18, 21, -24, -12, -100, -21, 43, 10, 18, 20, 59, 19, -32, -86, -35, 22, -43, -14, 73, -86, -3, 69, 5, -51, 80, -20, -67, 12, -6, 3, 32, -27, 44, 79, 1, -67, 14, 39, -30, -7, 12, -20, 25, 25, -74, 74, -1, 67, 23, -65, -1, -41, -53, -17, 50, 5, 40, -40, 56, -44, 34, 58, 83, -25, -29, 39, 59, -29, -33, -6}

#define TENSOR_DENSE_KERNEL_0_DEC_BITS {8}

#define TENSOR_DENSE_BIAS_0 {87, 14, 4, 26, -39, -32, -73, -60, 26, -65, -10, -81, 100}

#define TENSOR_DENSE_BIAS_0_DEC_BITS {10}

#define DENSE_BIAS_LSHIFT {2}

#define DENSE_OUTPUT_RSHIFT {8}


/* output q format for each layer */
#define INPUT_1_OUTPUT_DEC 5
#define INPUT_1_OUTPUT_OFFSET 0
#define CONV1D_OUTPUT_DEC 4
#define CONV1D_OUTPUT_OFFSET 0
#define RE_LU_OUTPUT_DEC 4
#define RE_LU_OUTPUT_OFFSET 0
#define CONV1D_1_OUTPUT_DEC 4
#define CONV1D_1_OUTPUT_OFFSET 0
#define RE_LU_1_OUTPUT_DEC 4
#define RE_LU_1_OUTPUT_OFFSET 0
#define MAX_POOLING1D_OUTPUT_DEC 4
#define MAX_POOLING1D_OUTPUT_OFFSET 0
#define FLATTEN_OUTPUT_DEC 4
#define FLATTEN_OUTPUT_OFFSET 0
#define DENSE_OUTPUT_DEC 4
#define DENSE_OUTPUT_OFFSET 0
#define DROPOUT_OUTPUT_DEC 4
#define DROPOUT_OUTPUT_OFFSET 0
#define SOFTMAX_OUTPUT_DEC 7
#define SOFTMAX_OUTPUT_OFFSET 0

/* bias shift and output shift for none-weighted layer */

/* tensors and configurations for each layer */
static int8_t nnom_input_data[450] = {0};

const nnom_shape_data_t tensor_input_1_dim[] = {150, 3};
const nnom_qformat_param_t tensor_input_1_dec[] = {5};
const nnom_qformat_param_t tensor_input_1_offset[] = {0};
const nnom_tensor_t tensor_input_1 = {
    .p_data = (void*)nnom_input_data,
    .dim = (nnom_shape_data_t*)tensor_input_1_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_input_1_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_input_1_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};

const nnom_io_config_t input_1_config = {
    .super = {.name = "input_1"},
    .tensor = (nnom_tensor_t*)&tensor_input_1
};
const int8_t tensor_conv1d_kernel_0_data[] = TENSOR_CONV1D_KERNEL_0;

const nnom_shape_data_t tensor_conv1d_kernel_0_dim[] = {3, 3, 30};
const nnom_qformat_param_t tensor_conv1d_kernel_0_dec[] = TENSOR_CONV1D_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_conv1d_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_conv1d_kernel_0 = {
    .p_data = (void*)tensor_conv1d_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_conv1d_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_conv1d_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_conv1d_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 3,
    .bitwidth = 8
};
const int8_t tensor_conv1d_bias_0_data[] = TENSOR_CONV1D_BIAS_0;

const nnom_shape_data_t tensor_conv1d_bias_0_dim[] = {30};
const nnom_qformat_param_t tensor_conv1d_bias_0_dec[] = TENSOR_CONV1D_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_conv1d_bias_0_offset[] = {0};
const nnom_tensor_t tensor_conv1d_bias_0 = {
    .p_data = (void*)tensor_conv1d_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_conv1d_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_conv1d_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_conv1d_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t conv1d_output_shift[] = CONV1D_OUTPUT_RSHIFT;
const nnom_qformat_param_t conv1d_bias_shift[] = CONV1D_BIAS_LSHIFT;
const nnom_conv2d_config_t conv1d_config = {
    .super = {.name = "conv1d"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_conv1d_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_conv1d_bias_0,
    .output_shift = (nnom_qformat_param_t *)&conv1d_output_shift, 
    .bias_shift = (nnom_qformat_param_t *)&conv1d_bias_shift, 
    .filter_size = 30,
    .kernel_size = {3},
    .stride_size = {3},
    .padding_size = {0, 0},
    .dilation_size = {1},
    .padding_type = PADDING_VALID
};
const int8_t tensor_conv1d_1_kernel_0_data[] = TENSOR_CONV1D_1_KERNEL_0;

const nnom_shape_data_t tensor_conv1d_1_kernel_0_dim[] = {3, 30, 15};
const nnom_qformat_param_t tensor_conv1d_1_kernel_0_dec[] = TENSOR_CONV1D_1_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_conv1d_1_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_conv1d_1_kernel_0 = {
    .p_data = (void*)tensor_conv1d_1_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_conv1d_1_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_conv1d_1_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_conv1d_1_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 3,
    .bitwidth = 8
};
const int8_t tensor_conv1d_1_bias_0_data[] = TENSOR_CONV1D_1_BIAS_0;

const nnom_shape_data_t tensor_conv1d_1_bias_0_dim[] = {15};
const nnom_qformat_param_t tensor_conv1d_1_bias_0_dec[] = TENSOR_CONV1D_1_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_conv1d_1_bias_0_offset[] = {0};
const nnom_tensor_t tensor_conv1d_1_bias_0 = {
    .p_data = (void*)tensor_conv1d_1_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_conv1d_1_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_conv1d_1_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_conv1d_1_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t conv1d_1_output_shift[] = CONV1D_1_OUTPUT_RSHIFT;
const nnom_qformat_param_t conv1d_1_bias_shift[] = CONV1D_1_BIAS_LSHIFT;
const nnom_conv2d_config_t conv1d_1_config = {
    .super = {.name = "conv1d_1"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_conv1d_1_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_conv1d_1_bias_0,
    .output_shift = (nnom_qformat_param_t *)&conv1d_1_output_shift, 
    .bias_shift = (nnom_qformat_param_t *)&conv1d_1_bias_shift, 
    .filter_size = 15,
    .kernel_size = {3},
    .stride_size = {3},
    .padding_size = {0, 0},
    .dilation_size = {1},
    .padding_type = PADDING_VALID
};

const nnom_pool_config_t max_pooling1d_config = {
    .super = {.name = "max_pooling1d"},
    .padding_type = PADDING_VALID,
    .output_shift = 0,
    .kernel_size = {3},
    .stride_size = {3},
    .num_dim = 1
};

const nnom_flatten_config_t flatten_config = {
    .super = {.name = "flatten"}
};
const int8_t tensor_dense_kernel_0_data[] = TENSOR_DENSE_KERNEL_0;

const nnom_shape_data_t tensor_dense_kernel_0_dim[] = {75, 13};
const nnom_qformat_param_t tensor_dense_kernel_0_dec[] = TENSOR_DENSE_KERNEL_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_kernel_0_offset[] = {0};
const nnom_tensor_t tensor_dense_kernel_0 = {
    .p_data = (void*)tensor_dense_kernel_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_kernel_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_kernel_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_kernel_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 2,
    .bitwidth = 8
};
const int8_t tensor_dense_bias_0_data[] = TENSOR_DENSE_BIAS_0;

const nnom_shape_data_t tensor_dense_bias_0_dim[] = {13};
const nnom_qformat_param_t tensor_dense_bias_0_dec[] = TENSOR_DENSE_BIAS_0_DEC_BITS;
const nnom_qformat_param_t tensor_dense_bias_0_offset[] = {0};
const nnom_tensor_t tensor_dense_bias_0 = {
    .p_data = (void*)tensor_dense_bias_0_data,
    .dim = (nnom_shape_data_t*)tensor_dense_bias_0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_dense_bias_0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_dense_bias_0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_qformat_param_t dense_output_shift[] = DENSE_OUTPUT_RSHIFT;
const nnom_qformat_param_t dense_bias_shift[] = DENSE_BIAS_LSHIFT;
const nnom_dense_config_t dense_config = {
    .super = {.name = "dense"},
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .weight = (nnom_tensor_t*)&tensor_dense_kernel_0,
    .bias = (nnom_tensor_t*)&tensor_dense_bias_0,
    .output_shift = (nnom_qformat_param_t *)&dense_output_shift,
    .bias_shift = (nnom_qformat_param_t *)&dense_bias_shift
};

const nnom_softmax_config_t softmax_config = {
    .super = {.name = "softmax"}
};
static int8_t nnom_output_data[13] = {0};

const nnom_shape_data_t tensor_output0_dim[] = {13};
const nnom_qformat_param_t tensor_output0_dec[] = {SOFTMAX_OUTPUT_DEC};
const nnom_qformat_param_t tensor_output0_offset[] = {0};
const nnom_tensor_t tensor_output0 = {
    .p_data = (void*)nnom_output_data,
    .dim = (nnom_shape_data_t*)tensor_output0_dim,
    .q_dec = (nnom_qformat_param_t*)tensor_output0_dec,
    .q_offset = (nnom_qformat_param_t*)tensor_output0_offset,
    .qtype = NNOM_QTYPE_PER_TENSOR,
    .num_dim = 1,
    .bitwidth = 8
};

const nnom_io_config_t output0_config = {
    .super = {.name = "output0"},
    .tensor = (nnom_tensor_t*)&tensor_output0
};
/* model version */
#define NNOM_MODEL_VERSION (10000*0 + 100*4 + 3)

/* nnom model */
static nnom_model_t* nnom_model_create(void)
{
	static nnom_model_t model;
	nnom_layer_t* layer[10];

	check_model_version(NNOM_MODEL_VERSION);
	new_model(&model);

	layer[0] = input_s(&input_1_config);
	layer[1] = model.hook(conv2d_s(&conv1d_config), layer[0]);
	layer[2] = model.active(act_relu(), layer[1]);
	layer[3] = model.hook(conv2d_s(&conv1d_1_config), layer[2]);
	layer[4] = model.active(act_relu(), layer[3]);
	layer[5] = model.hook(maxpool_s(&max_pooling1d_config), layer[4]);
	layer[6] = model.hook(flatten_s(&flatten_config), layer[5]);
	layer[7] = model.hook(dense_s(&dense_config), layer[6]);
	layer[8] = model.hook(softmax_s(&softmax_config), layer[7]);
	layer[9] = model.hook(output_s(&output0_config), layer[8]);
	model_compile(&model, layer[0], layer[9]);
	return &model;
}

﻿#pragma once
#include "ShowOffNVSE.h"
#include <armadillo>
#include <optional>
#include "NiTypes.h"


#if _DEBUG



// Extracted 2D numbers, for use as matrices, quaternions, etc.
template <typename T>
struct array_2d
{
	std::vector<std::vector<T>> numbers;
	array_2d() = default;

	//Returns num_rows (first), then num_cols
	[[nodiscard]] std::pair<size_t, size_t> GetDimensions() const;
	[[nodiscard]] bool IsEmpty() const { return numbers.empty(); }
	NVSEArrayVar* CreateArray(Script* callingScript);
};

//Based off of https://stackoverflow.com/a/17569578
template <typename T, size_t rows, size_t cols>
array_2d<T> process_2d_array(const T (&array)[rows][cols]);


// Ignores min/maxCol/Rows checks if == 0
// Assumes array is packed
template <typename T>
std::optional<array_2d<T>> TryGetArrayNumbers(NVSEArrayVar* arr, size_t const maxCols = 0, size_t const minCols = 0,
                                              size_t const maxRows = 0, size_t const minRows = 0);

std::optional<NiMatrix33> Get3x3MatrixFromArray(NVSEArrayVar* arr);

std::optional<NiQuaternion> GetQuatFromArray(NVSEArrayVar* arr);

//TODO: use array_2d instead of having copied code
std::optional<arma::Mat<double>> GetMatrixFromArray(NVSEArrayVar* arr);

//todo: swap for generic version (CreateArrayFromNumbers)
NVSEArrayVar* GetMatrixAsArray(arma::Mat<double>& matrix, Script* callingScript);

NVSEArrayVar* QuatToArray(NiQuaternion const& quat, Script* callingScript);


DEFINE_COMMAND_ALT_PLUGIN(Matrix_MultiplyByMatrix, Mat_MultByMat,
                          "Returns the matrix multiplication result of two matrix arrays.", false, kParams_TwoArrayIDs);
DEFINE_COMMAND_ALT_PLUGIN(Matrix_AddMatrix, Mat_AddMat, "Returns the addition of two matrices with equal # of rows and columns.", false, kParams_TwoArrayIDs);
DEFINE_COMMAND_ALT_PLUGIN(Matrix_MultiplyByScalar, Mat_MultByScal, "Returns the matrix multiplied by a scalar.", false, kParams_OneArrayID_OneDouble);
DEFINE_COMMAND_ALT_PLUGIN(Matrix_Transpose, Mat_T, "Returns the transpose of the array matrix.", false, kParams_OneArrayID);
DEFINE_COMMAND_ALT_PLUGIN(Matrix_IsMatrix, Mat_IsMat, "Checks if an array is convertible to a matrix.", false, kParams_OneArrayID);
DEFINE_COMMAND_ALT_PLUGIN(Matrix3x3_GetQuaternion, Mat_GetQuat, "Returns a quaternion from a 3x3 matrix.", false, kParams_OneArrayID);
DEFINE_COMMAND_ALT_PLUGIN(Matrix_Dump, Mat_Dump, "Dumps the matrix array in console, in matrix notation.", false, kParams_OneArrayID);
DEFINE_COMMAND_PLUGIN(TestMatrix, "debug matrix function", false, NULL);

DEFINE_COMMAND_ALT_PLUGIN(Quaternion_GetMatrix, Quat_GetMat, "Returns a 3x3 rotation matrix from a quaternion array.", false, kParams_OneArrayID);







/*When comparing two float values for equality, due to internal conversions between singleand double precision,
 *it's better to check if the absolute difference is less than epsilon (0.0001)
 *
 *(The Double Precision Fix from JIP LN) alleviates the problem with operator ==,
 *but doesn't eliminate it entirely. Checking absolute difference against an epsilon is far more computationally expensive than a simple comparison, obviously
 *-Jazzisparis */
DEFINE_COMMAND_ALT_PLUGIN(Flt_Equals, Float_Equals, "Returns true if the absolute difference between two floats is less than epsilon (0.0001)", false, kParams_TwoDoubles);

#endif
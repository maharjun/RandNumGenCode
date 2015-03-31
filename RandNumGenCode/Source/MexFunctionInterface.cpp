#include <mex.h>
#include <matrix.h>
#include <algorithm>
#include <vector>
#include <cstring>
#include <chrono>
#include <type_traits>
#include "..\Headers\MexMem.hpp"
#include "..\Headers\FiltRandomTBB.hpp"

using namespace std;

template<typename T> mxArray * assignmxArray(T &ScalarOut, mxClassID ClassID){

	mxArray * ReturnPointer;
	if (is_arithmetic<T>::value){
		ReturnPointer = mxCreateNumericMatrix_730(1, 1, ClassID, mxREAL);
		*reinterpret_cast<T *>(mxGetData(ReturnPointer)) = ScalarOut;
	}
	else{
		ReturnPointer = mxCreateNumericMatrix_730(0, 0, ClassID, mxREAL);
	}

	return ReturnPointer;
}

template<typename T> mxArray * assignmxArray(MexMatrix<T> &VectorOut, mxClassID ClassID){

	mxArray * ReturnPointer = mxCreateNumericMatrix_730(0, 0, ClassID, mxREAL);
	if (VectorOut.ncols() && VectorOut.nrows()){
		mxSetM(ReturnPointer, VectorOut.ncols());
		mxSetN(ReturnPointer, VectorOut.nrows());
		mxSetData(ReturnPointer, VectorOut.releaseArray());
	}

	return ReturnPointer;
}

template<typename T> mxArray * assignmxArray(MexVector<T> &VectorOut, mxClassID ClassID){

	mxArray * ReturnPointer = mxCreateNumericMatrix_730(0, 0, ClassID, mxREAL);
	if (VectorOut.size()){
		mxSetM(ReturnPointer, VectorOut.size());
		mxSetN(ReturnPointer, 1);
		mxSetData(ReturnPointer, VectorOut.releaseArray());
	}
	return ReturnPointer;
}

template<typename T> mxArray * assignmxArray(MexVector<MexVector<T> > &VectorOut, mxClassID ClassID){
	
	mxArray * ReturnPointer;
	if (VectorOut.size()){
		ReturnPointer = mxCreateCellMatrix(VectorOut.size(), 1);
		
		size_t VectVectSize = VectorOut.size();
		for (int i = 0; i < VectVectSize; ++i){
			mxSetCell(ReturnPointer, i, assignmxArray(VectorOut[i], ClassID));
		}
	}
	else{
		ReturnPointer = mxCreateCellMatrix_730(0, 0);
	}
	return ReturnPointer;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]){
	// NOTE THAT THERE IS NO DATA VALIDATION AS THIS IS EXPECTED TO HAVE 
	// BEEN DONE IN THE MATLAB SIDE OF THE INTERFACE TO THIS MEX FUNCTION

	// Test 1: 
	// 1. Generate a series of random numbers with some seed.
	// 2. Now Store the state.
	// 3. Generate a series of random numbers further. call this Set1
	// 4. reset the seed and generate above set again. call this Set2.
	// 5. Pass to matlab and verify the equality.

	int N = 10000;	// Number of elements in the vector for each time instant.
	uint32_t InitSeed = 0x183bef10;

	// initialinzing Generators
	XorShiftPlus G1(InitSeed), G2(InitSeed ^ (InitSeed >> 5));
	BandLimGaussVect RandVect(N, G1, G2, 0.4);
	MexMatrix<float> Set1(100, N), Set2(100,N);

	for (int i = 0; i < 100; ++i){
		RandVect.generate();
	}
	// Saving generators
	BandLimGaussVect::StateStruct IntermediateState;
	RandVect.getstate(IntermediateState);
	// generating 100 sets of rnos
	for (int i = 0; i < 100; ++i){
		RandVect.generate();
		Set1[i] = RandVect;
	}
	
	// resetting State
	RandVect.setstate(IntermediateState);
	// generating 100 sets of rnos
	for (int i = 0; i < 100; ++i){
		RandVect.generate();
		Set2[i] = RandVect;
	}

	// returning these matrices to matlab.
	plhs[0] = assignmxArray(Set1, mxSINGLE_CLASS);
	plhs[1] = assignmxArray(Set2, mxSINGLE_CLASS);
}
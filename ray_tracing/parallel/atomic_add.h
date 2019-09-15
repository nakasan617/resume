// the following code I took from 
// https://devtalk.nvidia.com/default/topic/1017902/optix/atomic-add-with-doubles/

#if __CUDA_ARCH__ < 600 
__device__ double atomicAdd_(double * address, double val) {
	unsigned long long int* address_as_ull = (unsigned long long int*)address;
	unsigned long long int old = *address_as_ull, assumed;
	do {
		assumed = old;
		old = atomicCAS(address_as_ull, assumed, __double_as_longlong(val + __longlong_as_double(assumed)));
		// Note: uses integer comparison to avoid hang in case of NaN (since NaN != NaN) }
	} while (assumed != old); 
	return __longlong_as_double(old);
}
#endif 

/**
 * For some unknown reason, std::atomic<unsigned> generates linker errors.
 * See there: https://github.com/devkitPro/libnds/issues/59
*/
extern "C" unsigned __atomic_fetch_add_4(volatile void *ptr, unsigned val, int memmodel) {
	(void)memmodel;
	unsigned tmp = *(unsigned*)ptr;
	*(unsigned*)ptr = tmp + val;
	return tmp;
}
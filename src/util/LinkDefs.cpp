typedef struct {
	const char *name;
	void *address;
} linkdef;

extern "C" {
	#include <linkdefs.inc>
}

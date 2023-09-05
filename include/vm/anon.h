#ifndef VM_ANON_H
#define VM_ANON_H
#include "vm/vm.h"
#include "kernel/bitmap.h"
struct page;
enum vm_type;

struct anon_page {
	/* Initiate the contets of the page */
	vm_initializer *init;
	enum vm_type type;
	void *aux;
	/* Initiate the struct page and maps the pa to the va */
	bool (*page_initializer) (struct page *, enum vm_type, void *kva);
	int swap_index;  // 06.30
};
void vm_anon_init (void);
bool anon_initializer (struct page *page, enum vm_type type, void *kva);

#endif

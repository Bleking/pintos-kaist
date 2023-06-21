/* vm.c: Generic interface for virtual memory objects. */

#include "threads/malloc.h"
#include "vm/vm.h"
#include "vm/inspect.h"
#include "threads/mmu.h"
#include "lib/string.h"

/* Initializes the virtual memory subsystem by invoking each subsystem's
 * intialize codes. */
void
vm_init (void) {
	vm_anon_init ();
	vm_file_init ();
#ifdef EFILESYS  /* For project 4 */
	pagecache_init ();
#endif
	register_inspect_intr ();
	/* DO NOT MODIFY UPPER LINES. */
	/* TODO: Your code goes here. */
}

/* Get the type of the page. This function is useful if you want to know the
 * type of the page after it will be initialized.
 * This function is fully implemented now. */
enum vm_type
page_get_type (struct page *page) {
	int ty = VM_TYPE (page->operations->type);
	switch (ty) {
		case VM_UNINIT:
			return VM_TYPE (page->uninit.type);
		default:
			return ty;
	}
}

/* Helpers */
static struct frame *vm_get_victim (void);
static bool vm_do_claim_page (struct page *page);
static struct frame *vm_evict_frame (void);
// void destructor_func (struct hash_elem, void *aux);

/* Create the pending page object with initializer. If you want to create a
 * page, do not create it directly and make it through this function or
 * `vm_alloc_page`. */
bool
vm_alloc_page_with_initializer (enum vm_type type, void *par_va, bool writable,
		vm_initializer *init, void *aux) {

	ASSERT (VM_TYPE(type) != VM_UNINIT)

	struct supplemental_page_table *spt = &thread_current ()->spt;

	/* Check wheter the par_va is already occupied or not. */
	if (spt_find_page (spt, par_va) == NULL) {
		/* TODO: Create the page, fetch the initialier according to the VM type,
		 * TODO: and then create "uninit" page struct by calling uninit_new. You
		 * TODO: should modify the field after calling the uninit_new. */
		struct page *p = (struct page *) malloc(sizeof(struct page)); // 06.18
		if (p== NULL){
			return false;
		}
		switch (VM_TYPE(type)){ // 06.20
			case VM_ANON:
				uninit_new(p, par_va, init, type, aux, anon_initializer);
				break;
			case VM_FILE:
				uninit_new(p, par_va, init, type, aux, file_backed_initializer);
				break;
			// case VM_PAGE_CACHE:
			default:
				return false;
		}
		p->writable = writable;
		/* TODO: Insert the page into the spt. */
		return spt_insert_page(spt, p);
	}
err:
	return false;
}

/* Find VA from spt and return page. On error, return NULL. */
struct page *
spt_find_page (struct supplemental_page_table *spt UNUSED, void *va UNUSED) {
    struct page *page = NULL;
    /* TODO: Fill this function. */
	
	page = (struct page *) malloc(sizeof(struct page));
	if (page == NULL) return NULL;

	page->va = pg_round_down(va);

    struct hash_elem *e = hash_find(&spt->table, &page->hash_elem);
    e = (e != NULL ? hash_entry(e, struct page, hash_elem) : NULL);
	
	free(page);
	return e;
}

/* Insert PAGE into spt with validation. */
bool
spt_insert_page (struct supplemental_page_table *spt UNUSED,
		struct page *page UNUSED) {
	int succ = false;
	/* TODO: Fill this function. */
	if(hash_insert(&spt->table, &page->hash_elem) == NULL) // 삽입 성공하면 NULL 리턴 (기존에 있으면 NULL 아님)
	{
        succ = true;
	}
	return succ;
}

void
spt_remove_page (struct supplemental_page_table *spt, struct page *page) {
	vm_dealloc_page (page);
	return true;
}

/* Get the struct frame, that will be evicted. */
static struct frame *
vm_get_victim (void) {
	struct frame *victim = NULL;
	 /* TODO: The policy for eviction is up to you. */

	return victim;
}

/* Evict one page and return the corresponding frame.
 * Return NULL on error.*/
static struct frame *
vm_evict_frame (void) {
	struct frame *victim UNUSED = vm_get_victim ();
	/* TODO: swap out the victim and return the evicted frame. */

	return NULL;
}

/* palloc() and get frame. If there is no available page, evict the page
 * and return it. This always return valid address. That is, if the user pool
 * memory is full, this function evicts the frame to get the available memory
 * space.*/
static struct frame *
vm_get_frame (void) {
	struct frame *frame = (struct frame *) malloc(sizeof(struct frame));

    /* TODO: Fill this function. */
    // palloc_get_page()를 호출해서 새로운 물리 메모리 페이지를 가져온다.
    // 성공 : 프레임 할당 -> 프레임 구조체의 멤버들을 초기화 한 후 -> 해당 프레임을 반환한다.
    // 이후 모든 유저 공간 페이지들은 이 함수를 통해 할당한다.
    // 실패 : PANIC("todo")

	// malloc() -> kva = palloc_get_page

    frame->kva = palloc_get_page(PAL_USER); // 물리 프레임의 주소(kva)를 반환하는 것
	if (frame->kva == NULL){
        PANIC("todo");
    }
    frame->page = NULL;
    ASSERT (frame != NULL);
    ASSERT (frame->page == NULL);
    return frame;
}

/* Growing the stack. */
static void
vm_stack_growth (void *addr UNUSED) {
}

/* Handle the fault on write_protected page */
static bool
vm_handle_wp (struct page *page UNUSED) {
}

/* Return true on success */
bool
vm_try_handle_fault (struct intr_frame *f UNUSED, void *addr UNUSED,
		bool user UNUSED, bool write UNUSED, bool not_present UNUSED) {
	struct supplemental_page_table *spt UNUSED = &thread_current ()->spt;
	struct page *page = NULL;
	/* TODO: Validate the fault */
	/* TODO: Your code goes here */
	if (is_kernel_vaddr(addr) || !addr)	{
		return false;
	}
	page = spt_find_page(spt, addr);

	return vm_do_claim_page(page);
}

/* Free the page.
 * DO NOT MODIFY THIS FUNCTION. */
void
vm_dealloc_page (struct page *page) {
	destroy (page);
	free (page);
}

/* Claim the page that allocate on VA. */
bool
vm_claim_page (void *va UNUSED) {
	struct page *page = NULL;
	/* TODO: Fill this function */
	page = spt_find_page(&thread_current()->spt, va);

	if(page == NULL){
		return false;
		// vm_alloc_page(VM_ANON, va, 1); // 06.19 나중에 변경!
		// page = spt_find_page(&thread_current()->spt, va);
	}

	return vm_do_claim_page (page);
}

/* Claim the PAGE and set up the mmu. */
static bool
vm_do_claim_page (struct page *page) {
	struct frame *frame = vm_get_frame ();

	if (frame == NULL)
		return false;

	/* Set links */
	frame->page = page;
	page->frame = frame;

	/* TODO: Insert page table entry to map page's VA to frame's PA. */
	if (pml4_get_page(thread_current()->pml4, page->va) == NULL && pml4_set_page(thread_current()->pml4, page->va, frame->kva, page->writable)){
		return swap_in (page, frame->kva);
	}
	return false;
}

/* Initialize new supplemental page table */
void
supplemental_page_table_init (struct supplemental_page_table *spt UNUSED) {
	hash_init(&spt->table, hash_hash, hash_less, NULL);
}

/* Copy supplemental page table from src to dst */
bool
supplemental_page_table_copy (struct supplemental_page_table *dst ,
		struct supplemental_page_table *src ) {
	bool succ = true;
	struct hash_iterator i;

	hash_first (&i, &src->table);
	while (hash_next (&i))
	{
		struct page *par_page = hash_entry (hash_cur (&i), struct page, hash_elem);

		// par_page의 정보를 저장
		void *par_va = par_page->va;
		bool writable = par_page->writable;

		// par_page의 타입
		enum vm_type ty = page_get_type(par_page);

		// par_page의 타입에 따라 다른 방식으로 할당.
		
		switch (ty) {
			case (VM_UNINIT):
				vm_alloc_page(VM_ANON, par_va, writable);
				struct frame *child_page_frame1 = spt_find_page(dst, par_va)->frame;
				child_page_frame1 = vm_get_frame();
				memcpy(child_page_frame1->kva, par_page->frame->kva, PGSIZE);
				break;

			case (VM_ANON):
				vm_alloc_page(VM_ANON, par_va, writable);
				vm_claim_page(par_va);
				memcpy(spt_find_page(dst, par_va)->frame->kva, par_page->frame->kva, PGSIZE);
				break;
			// case (VM_FILE):
			default:
				break;
		};

	};
	return succ;

}

/* Free the resource hold by the supplemental page table */
void
supplemental_page_table_kill (struct supplemental_page_table *spt UNUSED) {
	/* TODO: Destroy all the supplemental_page_table hold by thread and
	 * TODO: writeback all the modified contents to the storage. */
	
}

// void destructor_func (struct hash_elem hash_elem, void *aux) {
// 	hash_entry(hash_elem, struct page, hash_elem);

// } 
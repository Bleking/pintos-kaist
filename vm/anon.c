/* anon.c: Implementation of page for non-disk image (a.k.a. anonymous page). */

#include "vm/vm.h"
#include "devices/disk.h"
#include "threads/mmu.h"

/* DO NOT MODIFY BELOW LINE */
static struct disk *swap_disk;
struct bitmap *swap_bitmap;
static bool anon_swap_in (struct page *page, void *kva);
static bool anon_swap_out (struct page *page);
static void anon_destroy (struct page *page);

/* DO NOT MODIFY this struct */
static const struct page_operations anon_ops = {
	.swap_in = anon_swap_in,
	.swap_out = anon_swap_out,
	.destroy = anon_destroy,
	.type = VM_ANON,
};

// 06.29
struct bitmap *swap_table;  // each bit belongs to the swap slots; 1 means swap-out
const size_t SECTORS_PER_PAGE = PGSIZE / DISK_SECTOR_SIZE;  // 한 페이지마다 들어갈 수 있는 디스크 섹터는 8개

/* Initialize the data for anonymous pages */
void
vm_anon_init (void) {  // anonymous page를 위한 디스크 내 swap 영역을 생성하고 관리하는 swap table을 생성
	/* TODO: Set up the swap_disk. */
	// a data structure to manage free and used areas in the swap disk
	swap_disk = disk_get(1, 1);
	size_t swap_disk_size = disk_size(swap_disk) / SECTORS_PER_PAGE;  // the number of swap slots (4096 / 8 = 512)
	swap_table = bitmap_create(swap_disk_size);
}

/* Initialize the file mapping */
bool
anon_initializer (struct page *page, enum vm_type type, void *kva) {
	/* Set up the handler */
	
	struct uninit_page *uninit = &page->uninit;  // 07.01
	memset(uninit, 0, sizeof(struct uninit_page));  // 07.01
	
	page->operations = &anon_ops;

	struct anon_page *anon_page = &page->anon;
	anon_page->swap_index = -1;  // 07.01
	
	return true;  // 07.01
}

/* Swap in the page by read contents from the swap disk. */
static bool
anon_swap_in (struct page *page, void *kva) {  // swap-out된 페이지를 디스크에서 다시 메모리로
	struct anon_page *anon_page = &page->anon;
	
	// swap in an anon page from the disk to memory: read the data contents from the disk to the memory
	int the_slot = anon_page->swap_index;

	if (bitmap_test(swap_table, the_slot) == false)
		return false;
	
	for (int i = 0; i < SECTORS_PER_PAGE; i++) {
		disk_read(swap_disk, i + the_slot*SECTORS_PER_PAGE, kva + i*DISK_SECTOR_SIZE);
	}

	bitmap_set(swap_table, the_slot, false);

	return true;
}

/* Swap out the page by writing contents to the swap disk. */
static bool
anon_swap_out (struct page *page) {  // 메모리 내용을(anon page) 디스크의 swap 영역으로 보내기
	struct anon_page *anon_page = &page->anon;
	// 07.01
	// anonymous page -> swap disk: copy the contents from the memory to the disk
	// find a free slot in the disk using the swap table
	int the_slot = bitmap_scan(swap_table, 0, 1, false);  // bitmap_scan(const struct bitmap *b, start, cnt, bool value)
	
	// no more free slot in the disk -> panic the kernel
	if (the_slot == BITMAP_ERROR)
		return false;
	
	// copy the page of data into the slot
	for (int i = 0; i < SECTORS_PER_PAGE; i++) {
		disk_write(swap_disk, i + the_slot*SECTORS_PER_PAGE, page->va + i*DISK_SECTOR_SIZE);
	}
	
	bitmap_set(swap_table, the_slot, true);
	pml4_clear_page(thread_current()->pml4, page->va);
	
	// the data location is in the page structure
	anon_page->swap_index = the_slot;

	return true;
}

/* Destroy the anonymous page. PAGE will be freed by the caller. */
static void
anon_destroy (struct page *page) {
	struct anon_page *anon_page = &page->anon;
}

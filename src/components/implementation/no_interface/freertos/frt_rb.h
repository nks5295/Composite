#ifndef FRT_RB_H
#define FRT_RB_H

#define RB_ALLOCSZ 4096

#define MIN(a,b)				\
	({ __typeof__ (a) _a = (a);		\
		__typeof__ (b) _b = (b);	\
		_a < _b ? _a : _b; })

struct rb_head {
	unsigned short int prod, cons;
	char ring[0];
};

#define RB_SZ (RB_ALLOCSZ - sizeof(struct rb_head))

struct rb_item_head {
	unsigned short int item_sz, type;
};

static inline unsigned short int
__rb_inc(int curr, int amnt)
{ return ((curr + amnt) % RB_SZ); }

inline unsigned short int
rb_amnt(struct rb_head *rb)
{
	assert(rb);
	if (rb->prod >= rb->cons) {
		return rb->prod - rb->cons;
	} else {
		return RB_SZ - (rb->cons - rb->prod);
	}
}

static int 
__rb_isfull(struct rb_head *rb, int amnt) 
{ 
	assert(rb);
	if (amnt < 0) return -1;
	if ((unsigned short int)amnt > RB_SZ) return 1;
	return (rb_amnt(rb) + (unsigned short int)amnt) >= (RB_SZ-1); 
}

static int
__rb_isempty(struct rb_head *rb)
{
	unsigned short int amnt;

	assert(rb);
	amnt = rb_amnt(rb);
	assert(amnt == 0 || amnt > sizeof(struct rb_item_head));
	return amnt == 0;
}

static inline void
__rb_insert(struct rb_head *rb, void *mem, unsigned int sz)
{
	int real_sz = sz;
	unsigned short int prod_old, top;

        assert(rb);
	prod_old = rb->prod;
	rb->prod = __rb_inc(prod_old, real_sz);
	top      = MIN(RB_SZ - prod_old, sz);
	memcpy(&rb->ring[prod_old], mem, top);
	if (top < real_sz) {
		memcpy(&rb->ring[0], ((char*)mem) + top, real_sz - top);
	}
}

int
rb_insert(struct rb_head *rb, void *mem, int sz)
{
	int real_sz = sz + sizeof(struct rb_item_head);
	struct rb_item_head h;

        assert(rb && mem);
	h.item_sz = sz;
	if (__rb_isfull(rb, real_sz)) return -1;
	__rb_insert(rb, &h, sizeof(struct rb_item_head));
	__rb_insert(rb, mem, (unsigned int)sz);
	return 0;
}

int
__rb_itemhdr(struct rb_head *rb, struct rb_item_head *rbih)
{
	unsigned short int cons_old, top;
	

	if (__rb_isempty(rb)) return -1;

	cons_old = rb->cons;
	top      = MIN(RB_SZ - cons_old, sizeof(struct rb_item_head));
	memcpy((char*)rbih, &rb->ring[cons_old], top);
	if (top < sizeof(struct rb_item_head)) {
		memcpy((char*)rbih + top, &rb->ring[0], sizeof(struct rb_item_head) - top);
	}
	return 0;
}

unsigned short int 
rb_peek_sz(struct rb_head *rb)
{
	struct rb_item_head rbih;
	__rb_itemhdr(rb, &rbih);
	return rbih.item_sz;
}

unsigned short int 
rb_peek_type(struct rb_head *rb)
{
	struct rb_item_head rbih;
	__rb_itemhdr(rb, &rbih);
	return rbih.type;
}


int
rb_fastremove(struct rb_head *rb, int sz)
{
        unsigned short int item_sz;

        item_sz = rb_peek_sz(rb);
        if ((unsigned short int)sz < item_sz) return -1;

        rb->cons = __rb_inc(rb->cons, sizeof(struct rb_item_head) + item_sz);
        return 0;
}



int 
rb_remove(struct rb_head *rb, void *mem, int sz)
{
	unsigned short int item_sz, cons_old, top;

	item_sz = rb_peek_sz(rb);
	if ((unsigned short int)sz < item_sz) return -1;

	rb->cons = __rb_inc(rb->cons, sizeof(struct rb_item_head));
	cons_old = rb->cons;
	rb->cons = __rb_inc(cons_old, item_sz);
	top      = MIN(RB_SZ - cons_old, item_sz);
	memcpy(mem, &rb->ring[cons_old], top);
	if (top < item_sz) {
		memcpy((char*)mem + top, &rb->ring[0], item_sz - top);
	}

	return 0;
}

void
rb_alloc(struct rb_head *rb, vaddr_t m)
{
	rb       = m;
	rb->prod = rb->cons = 0;
}

void
rb_free(struct rb_head *rb)
{

}

/*
#define TEST_SZ (4096/sizeof(int))
#define TEST_ITEM_SZ (sizeof(int) * 10)

int main(void)
{
	struct rb_head *rb;
	int data[TEST_SZ], output[TEST_SZ];
	unsigned int i;

	for (i = 0 ; i < TEST_SZ ; i++) {
		data[i] = i;
	}

	rb = rb_alloc();	
	for (i = 0 ; i < 12354 ; i++) {
		rb_insert(rb, data, TEST_ITEM_SZ);
		assert(rb_peek_sz(rb) == TEST_ITEM_SZ);
		rb_remove(rb, output, TEST_ITEM_SZ);
		assert(memcmp(data, output, TEST_ITEM_SZ) == 0);
	}
	while (!rb_insert(rb, data, TEST_ITEM_SZ)) {
		assert(rb_peek_sz(rb) == TEST_ITEM_SZ);
	}
	while (!rb_remove(rb, output, TEST_ITEM_SZ)) {
		for (i = 0 ; i < 10 ; i++) {
			if (((char*)data)[i] != ((char*)output)[i]) {
				printc("WTF\n");
			}
		}
		assert(memcmp(data, output, TEST_ITEM_SZ) == 0);
	}
        //off by one error here
	rb_free(rb);

	return 0;
}
*/

#endif

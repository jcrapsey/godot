#ifndef HILBERT_HOTEL_H
#define HILBERT_HOTEL_H

#include "core/list.h"
#include "core/object.h"
#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "core/rid.h"
#include "core/set.h"
#include "core/variant.h"
#include "infinite_bus.h"

class HilbertHotel : public Object {
	GDCLASS(HilbertHotel, Object);

	static HilbertHotel *singleton;
	static void thread_func(void *p_udata);

private:
	bool thread_exited;
	mutable bool exit_thread;
	Thread *thread;
	Mutex *mutex; // Mutual Exclusion Object, allows multiple threads to share the same resource

public:
	static HilbertHotel *get_singleton();
	Error init();
	void lock();
	void unlock();
	void finish();

protected:
	static void _bind_methods();

private:
	uint64_t counter;
	RID_Owner<InfiniteBus> bus_owner;
	Set<RID> buses;
	void _emit_occupy_room(uint64_t room, RID rid);

public:
	RID create_bus();
	Variant get_bus_info(RID id);
	bool empty();
	bool delete_bus(RID id);
	void clear();
	void register_rooms();
	HilbertHotel();

};

#endif

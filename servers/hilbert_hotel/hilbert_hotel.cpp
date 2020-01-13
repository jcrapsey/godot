#include "hilbert_hotel.h"
#include "infinite_bus.h"
#include "../hilbert_hotel_server.h"

#include "core/dictionary.h"
#include "core/list.h"
#include "core/os/os.h"
#include "core/variant.h"

#include "infinite_bus.h"
#include "prime_225.h"

void HilbertHotel::thread_func(void *p_update) {
	HilbertHotel *ac = (HilbertHotel *)p_update;
	uint64_t msdelay = 1000;

	while (!ac->exit_thread) {
		if (!ac->empty()) {
			ac->lock();
			ac->register_rooms();
			ac->unlock();
		}

		OS::get_singleton()->delay_usec(msdelay * 1000);
	}
}

Error HilbertHotel::init() {
	thread_exited = false;
	counter = 0;
	mutex = Mutex::create();
	thread = Thread::create(HilbertHotel::thread_func, this);
	return OK;
}

HilbertHotel *HilbertHotel::singleton = NULL;

HilbertHotel *HilbertHotel::get_singleton() {
	return singleton;
}

void HilbertHotel::register_rooms () {
	for (Set<RID>::Element *e = buses.front(); e; e = e->next()) {
		auto bus = bus_owner.getornull(e->get());

		if (bus) {
			uint64_t room = bus->next_room();
			_emit_occupy_room(room, bus->get_self());
		}
	}
}

void HilbertHotel::unlock() {
	if (!thread || !mutex) {
		return;
	}

	mutex->unlock();
}

void HilbertHotel::lock() {
	if (!thread || !mutex) {
		return;
	}

	mutex->lock();
}

void HilbertHotel::_emit_occupy_room(uint64_t room, RID rid) {
	HilbertHotelServer::get_singleton()->_occupy_room(room, rid);
}

 Variant HilbertHotel::get_bus_info(RID id) {
	InfiniteBus *bus = bus_owner.getornull(id);

	if (bus) {
		Dictionary d;
		d["prime"] = bus->get_bus_num();
		d["current_room"] = bus->get_current_room();
		return d;
	}

	return Variant();
}

void HilbertHotel::finish() {
	if (!thread) {
		return;
	}

	exit_thread = true;
	Thread::wait_to_finish(thread);

	memdelete(thread);

	if (mutex) {
		memdelete(mutex);
	}

	thread = NULL;
}

RID HilbertHotel::create_bus() {
	lock();
	InfiniteBus *ptr = memnew(InfiniteBus(PRIME[counter++]));
	RID ret = bus_owner.make_rid(ptr);
	ptr->set_self(ret);
	buses.insert(ret);
	unlock();

	return ret;
}

bool HilbertHotel::delete_bus(RID id) {
	if (bus_owner.owns(id)) {
		lock();
		InfiniteBus *b = bus_owner.get(id);
		bus_owner.free(id);
		buses.erase(id);
		memdelete(b);
		unlock();
		return true;
	}

	return false;
}

void HilbertHotel::clear() {
	for (Set<RID>::Element *e = buses.front(); e; e = e->next()) {
		delete_bus(e->get());
	}
}

bool HilbertHotel::empty() {
	return buses.size() <= 0;
}

void HilbertHotel::_bind_methods() {}

HilbertHotel::HilbertHotel() {
	singleton = this;
}

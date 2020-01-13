#ifndef INFINITE_BUS_H
#define INFINITE_BUS_H

#include "core/rid.h"
#include "prime_225.h"

class InfiniteBus : public RID_Data {
	RID self;

private:
	uint64_t prime_num;
	uint64_t num;

public:
	uint64_t next_room() {
		return prime_num * num++;
	}

	uint64_t get_bus_num() const {
		return prime_num;
	}

	uint64_t get_current_room() const {
		return prime_num * num;
	}

	_FORCE_INLINE_ void set_self(const RID &p_self) {
		self = p_self;
	}

	_FORCE_INLINE_ RID get_self() const {
		return self;
	}

	InfiniteBus(uint64_t prime) : prime_num(prime), num(1){};
	~InfiniteBus(){};
};

#endif

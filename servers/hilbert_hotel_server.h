#ifndef HILBERT_HOTEL_SERVER
#define HILBERT_HOTEL_SERVER

#include "core/object.h"
#include "hilbert_hotel/hilbert_hotel.h"

class HilbertHotelServer : public Object {
	GDCLASS(HilbertHotelServer, Object);

	friend class HilbertHotel;

protected:
	static HilbertHotelServer *singleton;
	static void _bind_methods();

public:
	static HilbertHotelServer *get_singleton();
	void _occupy_room(int room_number, RID bus);
	RID create_bus();
	void connect_signals();
	bool delete_bus(RID id);
	Variant get_bus_info(RID id);

	HilbertHotelServer();
	~HilbertHotelServer();

};

#endif

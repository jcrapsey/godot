#include "hilbert_hotel_server.h"
#include "hilbert_hotel/hilbert_hotel.h"

HilbertHotelServer *HilbertHotelServer::singleton = NULL;
HilbertHotelServer *HilbertHotelServer::get_singleton() {
	return singleton;
}

void HilbertHotelServer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_bus_info", "r_id"), &HilbertHotelServer::get_bus_info);
	ClassDB::bind_method(D_METHOD("create_bus"), &HilbertHotelServer::create_bus);
	ClassDB::bind_method(D_METHOD("delete_bus"), &HilbertHotelServer::delete_bus);
	ADD_SIGNAL(MethodInfo("occupy_room", PropertyInfo(Variant::INT, "room_number"), PropertyInfo(Variant::_RID, "rid")));
}

RID HilbertHotelServer::create_bus() {
	return HilbertHotel::get_singleton()->create_bus();
}

bool HilbertHotelServer::delete_bus(RID rid) {
	return HilbertHotel::get_singleton()->delete_bus(rid);
}

void HilbertHotelServer::_occupy_room(int room_number, RID bus) {
	emit_signal("occupy_room", room_number, bus);
}

 Variant HilbertHotelServer::get_bus_info(RID id) {
	return HilbertHotel::get_singleton()->get_bus_info(id);
}

void HilbertHotelServer::connect_signals() {
	HilbertHotel::get_singleton()->connect("occupy_room", this, "_occupy_room");
}

HilbertHotelServer::HilbertHotelServer() {
	singleton = this;
}

HilbertHotelServer::~HilbertHotelServer() {
	singleton = NULL;
}

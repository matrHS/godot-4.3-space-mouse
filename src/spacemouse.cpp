/*
Copyright (c) 2022 Andres Hernandez

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/



#include <string.h>
#include "spacemouse.h"
#ifndef HID_API_MAKE_VERSION
#define HID_API_MAKE_VERSION(mj, mn, p) (((mj) << 24) | ((mn) << 8) | (p))
#endif
#ifndef HID_API_VERSION
#define HID_API_VERSION HID_API_MAKE_VERSION(HID_API_VERSION_MAJOR, HID_API_VERSION_MINOR, HID_API_VERSION_PATCH)
#endif

#if defined(__APPLE__) && HID_API_VERSION >= HID_API_MAKE_VERSION(0, 12, 0)
#include <hidapi_darwin.h>
#endif

#if defined(_WIN32) && HID_API_VERSION >= HID_API_MAKE_VERSION(0, 12, 0)
#include <hidapi_winapi.h>
#endif

#if defined(USING_HIDAPI_LIBUSB) && HID_API_VERSION >= HID_API_MAKE_VERSION(0, 12, 0)
#include <hidapi_libusb.h>
#endif



void Spacemouse::_bind_methods(){
		godot::ClassDB::bind_method(godot::D_METHOD("connect"), &Spacemouse::spacemouse_connect);
		godot::ClassDB::bind_method(godot::D_METHOD("poll"), &Spacemouse::spacemouse_poll);
		godot::ClassDB::bind_method(godot::D_METHOD("translation"), &Spacemouse::spacemouse_translation);
		godot::ClassDB::bind_method(godot::D_METHOD("rotation"), &Spacemouse::spacemouse_rotation);
}


Spacemouse::Spacemouse(){
        motionData = SpaceMotion();
        motionData.translation = godot::Vector3(0, 0, 0);
        motionData.rotation = godot::Vector3(0, 0, 0);
}

Spacemouse::~Spacemouse(){
        hid_close(space_device);
        hid_exit();
}
    
bool Spacemouse::spacemouse_connect(){

	if (hid_init()) {
		space_connected = 0;
		return false;
	}


    #if defined(__APPLE__) && HID_API_VERSION >= HID_API_MAKE_VERSION(0, 12, 0)
	hid_darwin_set_open_exclusive(1);
    #endif

    
	struct hid_device_info *hid_devices, *current_hid;	
	hid_devices = hid_enumerate(0x0, 0x0);
	current_hid = hid_devices;
	int not_found = 1;
	
	while (current_hid && not_found) {
		for (int model = 0; model < MODELS; model++) {
			if (model_ids[model][0] == current_hid->vendor_id &&
					model_ids[model][1] == current_hid->product_id) {
				current_model = model;
				not_found = 0;
			}
		}
		current_hid = current_hid->next;
	}
	
	hid_free_enumeration(hid_devices);

	space_device = hid_open(model_ids[current_model][0], model_ids[current_model][1], NULL);
	
	if (!space_device) {
 		space_connected = 0;
		return false;
	}
    
    hid_set_nonblocking(space_device, 1);
    
	memset(space_buffer, 0x00, sizeof(space_buffer));
	space_buffer[0] = 0x01;
	space_buffer[1] = 0x81;
	
	space_connected = 1;
	
	return true;
}
bool Spacemouse::spacemouse_poll() {
	bool result = false;

	godot::Vector3 translation = godot::Vector3(0, 0, 0);
	godot::Vector3 rotation = godot::Vector3(0, 0, 0);

	godot::Vector3 space_trans;
	godot::Vector3 space_rot;

	
	int poll_count = 2;

	if (model_ids[current_model][2] == 0)
		poll_count *= 2;
		
	space_data.px = 0;
	space_data.py = 0;
	space_data.pz = 0;
	space_data.rx = 0;
	space_data.ry = 0;
	space_data.rz = 0;

	while(space_connected && poll_count-- > 0) {
		int read_len = hid_read(space_device, space_buffer, sizeof(space_buffer));
		if (read_len > 0) {
			switch(model_ids[current_model][2]) {
				case Original:
					if (space_buffer[0] == 1) {
						space_data.px += to_int(&space_buffer[1]);
						space_data.py += to_int(&space_buffer[5]);
						space_data.pz += to_int(&space_buffer[3]);
					} else if (space_buffer[0] == 2) {
						space_data.rx += to_int(&space_buffer[1]);
						space_data.ry += to_int(&space_buffer[5]);
						space_data.rz += to_int(&space_buffer[3]);
					}
					break;
				case Current:
					if (space_buffer[0] == 1) {
						space_data.px += to_int(&space_buffer[1]);
						space_data.py += to_int(&space_buffer[5]);
						space_data.pz += to_int(&space_buffer[3]);
						space_data.rx += to_int(&space_buffer[7]);
						space_data.ry += to_int(&space_buffer[11]);
						space_data.rz += to_int(&space_buffer[9]);
					}
					break;
			}
		}
	}
	if (space_connected) {
		space_trans = godot::Vector3(space_data.px, space_data.py, space_data.pz);
		translation = translation + space_trans;

		space_rot = godot::Vector3(space_data.rx, space_data.ry, space_data.rz);
		rotation = rotation + space_rot;

		result = true;
	}
	motionData.translation = translation;
	motionData.rotation = rotation;
	
    return result;
}

	godot::Vector3 Spacemouse::spacemouse_translation() {
	return motionData.translation;
}

	godot::Vector3 Spacemouse::spacemouse_rotation() {
	return motionData.rotation;
}

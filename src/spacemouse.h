#ifndef SPACEMOUSE_H
#define SPACEMOUSE_H

// Include any necessary libraries or headers here
#include <godot_cpp/core/class_db.hpp>

#include <hidapi.h>

#define MODELS 7
#define IDS 3

//Define your class for the space mouse
class Spacemouse : public godot::Object{
	GDCLASS(Spacemouse, godot::Object)
public:
	int model_ids[MODELS][IDS] = {
		// space navigator
		{ 0x046d, 0xc626, 0x00 },
		// space mouse compact
		{ 0x256f, 0xc635, 0x00 },
		// space mouse pro wireless
		{ 0x256f, 0xc632, 0x01 },
		// space mouse pro
		{ 0x046d, 0xc62b, 0x00 },
		// space mouse wireless
		{ 0x256f, 0xc62e, 0x01 },
		// universal receiver
		{ 0x256f, 0xc652, 0x01 },
		// space pilot pro
		{ 0x046d, 0xc629, 0x00 }
	};

	enum Format {
		Original = 0,
		Current = 1
	};

	int current_model = -1;

	typedef struct SpaceData {
		int px, py, pz, rx, ry, rz;
	} SpaceData;

	typedef struct SpaceMotion {
		godot::Vector3 translation;
		godot::Vector3 rotation;
	} SpaceMotion;
	int to_int(unsigned char* buffer)
	{
		int val = (buffer[1] << 8) + buffer[0];
		if (val >= 32768)
			val = -(65536 - val);
		return val;
	}
    // Constructor and destructor
    Spacemouse();
    ~Spacemouse();
    bool spacemouse_poll();
    bool spacemouse_connect();
    godot::Vector3 spacemouse_translation();
    godot::Vector3 spacemouse_rotation();
    // Declare any member functions or variables here
protected:
    static void _bind_methods();
    

private:
	SpaceMotion motionData;	
	hid_device *space_device;
	unsigned char space_buffer[256];
	SpaceData space_data = {0, 0, 0, 0, 0, 0};
	int space_connected = 0;

};

#endif // SPACEMOUSE_H
//
// Created by vfs on 5/28/2020.
//

#ifndef ZOMBIES_MATHS_H
#define ZOMBIES_MATHS_H

struct Vec2 {
	union {
		struct {
			float x;
			float y;
		};

		float components[2];
	};
};

struct Vec3 {
	union {
		struct {
			float x;
			float y;
			float z;
		};

		struct {
			float pitch;
			float yaw;
			float roll;
		};

		float components[3];
	};

	Vec3 operator+(Vec3 vector) {
		return {x + vector.x, y + vector.y, z + vector.z};
	}

	Vec3 operator*(float scalar) {
		return {x * scalar, y * scalar, z * scalar};
	}

	ZOMBIESCOMMON_API
	static void blend(float alpha, Vec3 *blended, Vec3 *previous, Vec3 *current);
};

struct Vec4 {
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};

		float components[4];
	};
};

struct Quat {
	union {
		struct {
			float w;
			float i;
			float j;
			float k;
		};

		float components[4];
	};
};

ZOMBIESCOMMON_API
float fclamp(float x, float upper, float lower);

#endif

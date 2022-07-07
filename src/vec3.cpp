module;

#include <stdlib.h>
#include <cmath>

export module vec3;

template <typename T> int sign_(T val) {
    return (T(0) < val) - (val < T(0));
}

export struct ivec3 {
	int x, y, z;

	ivec3 operator&(ivec3 const &obj) {
		return {.x = this->x & obj.x, .y = this->y & obj.y, .z = this->z & obj.z};
	}

	ivec3 operator+(ivec3 const &obj) {
		return {.x = this->x + obj.x, .y = this->y + obj.y, .z = this->z + obj.z};
	}

	ivec3 operator-(ivec3 const &obj) {
		return {.x = this->x - obj.x, .y = this->y - obj.y, .z = this->z - obj.z};
	}

	ivec3 operator*(ivec3 const &obj) {
		return {.x = this->x * obj.x, .y = this->y * obj.y, .z = this->z * obj.z};
	}

	ivec3 operator/(ivec3 const &obj) {
		return {.x = this->x / obj.x, .y = this->y / obj.y, .z = this->z / obj.z};
	}

	ivec3 operator-(int const i) {
		return {.x = this->x - i, .y = this->y - i, .z = this->z - i};
	}

	ivec3 operator/(int const i) {
		return {.x = this->x / i, .y = this->y / i, .z = this->z / i};
	}

	ivec3 operator&(int const i) {
		return {.x = this->x & i, .y = this->y & i, .z = this->z & i};
	}

	bool operator!=(ivec3 const &obj) {
		return this->x != obj.x || this->y != obj.y || this->z != obj.z;
	}

	ivec3 abs() {
		return {.x = std::abs(this->x), .y = std::abs(this->y), .z = std::abs(this->z)};
	}
};

export struct vec3 {
	float x, y, z;

	vec3 operator+(vec3 const &obj) {
		return {.x = this->x + obj.x, .y = this->y + obj.y, .z = this->z + obj.z};
	}

	vec3 operator+(float val) {
		return {.x = this->x + val, .y = this->y + val, .z = this->z + val};
	}

	void operator+=(vec3 const &obj) {
		this->x = this->x + obj.x;
		this->y = this->y + obj.y;
		this->z = this->z + obj.z;
	}

	vec3 operator-(vec3 const &obj) {
		return {.x = this->x - obj.x, .y = this->y - obj.y, .z = this->z - obj.z};
	}

	vec3 operator*(vec3 const &obj) {
		return {.x = this->x * obj.x, .y = this->y * obj.y, .z = this->z * obj.z};
	}

	vec3 operator*(ivec3 const &obj) {
		return {.x = this->x * obj.x, .y = this->y * obj.y, .z = this->z * obj.z};
	}

	vec3 operator/(vec3 const &obj) {
		return {.x = this->x / obj.x, .y = this->y / obj.y, .z = this->z / obj.z};
	}

	vec3 operator*(float fac) {
		return {.x = this->x*fac, .y = this->y*fac, .z = this->z*fac};
	}

	vec3 abs() {
		return {.x = std::abs(this->x), .y = std::abs(this->y), .z = std::abs(this->z)};
	}

	vec3 floor() {
		return {.x = std::floor(this->x), .y = std::floor(this->y), .z = std::floor(this->z)};
	}

	ivec3 sign() {
		return {.x = sign_(this->x), .y = sign_(this->y), .z = sign_(this->z)};
	}

	void normalize() {
		float val = sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
		this->x /= val;
		this->y /= val;
		this->z /= val;
	}
};

export vec3 vec3FromFloat(float val) {
	return {.x = val, .y = val, .z = val};
}

export ivec3 ivec3FromInt(int val) {
	return {.x = val, .y = val, .z = val};
}

export vec3 min(vec3 const &obj1, vec3 const &obj2) {
	return {.x = std::min(obj1.x, obj2.x), .y = std::min(obj1.y, obj2.y), .z = std::min(obj1.z, obj2.z)};
}

export vec3 max(vec3 const &obj1, vec3 const &obj2) {
	return {.x = std::max(obj1.x, obj2.x), .y = std::max(obj1.y, obj2.y), .z = std::max(obj1.z, obj2.z)};
}

export ivec3 min(ivec3 const &obj1, ivec3 const &obj2) {
	return {.x = std::min(obj1.x, obj2.x), .y = std::min(obj1.y, obj2.y), .z = std::min(obj1.z, obj2.z)};
}

export ivec3 max(ivec3 const &obj1, ivec3 const &obj2) {
	return {.x = std::max(obj1.x, obj2.x), .y = std::max(obj1.y, obj2.y), .z = std::max(obj1.z, obj2.z)};
}

export ivec3 vec3_to_ivec3(vec3 const &obj) {
	return {.x = (int)obj.x, .y = (int)obj.y, .z = (int)obj.z};
}
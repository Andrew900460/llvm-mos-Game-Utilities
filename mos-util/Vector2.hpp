

template<typename T>
struct Vector2 {
	T x;
	T y;

	constexpr Vector2() : x(),y() {}
	constexpr explicit Vector2(const T x, const T y): x(x),y(y) {}
	constexpr explicit Vector2(const T v): x(v),y(v) {}

	// ADD
	Vector2 operator + (const Vector2& rhs) const { // 6502 clock time of ~88
		return Vector2(x+rhs.x, y+rhs.y);
	}
	Vector2& operator += (const Vector2& rhs) {
		(*this) = (*this)+rhs;
		return *this;
	}

	// SUBTRACT
	Vector2 operator - (const Vector2& rhs) const { // 6502 clock time of ~88
		return Vector2(x-rhs.x, y-rhs.y);
	}
	Vector2& operator -= (const Vector2& rhs) {
		(*this) = (*this)+rhs;
		return *this;
	}

	// MULTIPLY SCALAR
	Vector2 operator * (const T& scalarRHS) const { // 6502 clock time of ~[1790,2224]
		return Vector2(x*scalarRHS, y*scalarRHS);
	}
	Vector2& operator *= (const T& scalarRHS) {
		(*this) = (*this)*scalarRHS;
		return *this;
	}

	// DIVIDE SCALAR
	Vector2 operator / (const T& scalarRHS) const { // 6502 clock time of ~[3524,3903]
		return Vector2(x/scalarRHS, y/scalarRHS);
	}
	Vector2& operator /= (const T& scalarRHS) {
		(*this) = (*this)/scalarRHS;
		return *this;
	}

	// MULTIPLY VECTOR
	Vector2 operator * (const Vector2& rhs) const {
		return Vector2(x*rhs.x, y*rhs.y);
	}
	Vector2& operator *= (const Vector2& rhs) {
		(*this) = (*this)*rhs;
		return *this;
	}

	// DIVIDE VECTOR
	Vector2 operator / (const Vector2& rhs) const {
		return Vector2(x/rhs.x, y/rhs.y);
	}
	Vector2& operator /= (const Vector2& rhs) {
		(*this) = (*this)/rhs;
		return *this;
	}

	// COMPARE
	bool operator == (const Vector2& rhs) const {
		return x==rhs.x && y==rhs.y;
	}

	// TRANSFORM
	T Magnitude() const {
		return (x*x+y*y).sqrt();
	}

	Vector2 Normalize() const {
		return (*this)/Magnitude();
	}
};
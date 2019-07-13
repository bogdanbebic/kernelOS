#ifndef _COMPARE_FUNCTORS_H_
#define _COMPARE_FUNCTORS_H_

// UPDATE: CEO FAJL JE DODAT

template<class T>
class EqualTo {
public:
	bool operator ()(const T & lhs, const T & rhs) {
		return lhs == rhs;
	}

};

template<class T>
class NotEqualTo {
public:
	bool operator ()(const T & lhs, const T & rhs) {
		return lhs != rhs;
	}

};

template<class T>
class Greater {
public:
	bool operator ()(const T & lhs, const T & rhs) {
		return lhs > rhs;
	}
	
};

template<class T>
class Less {
public:
	bool operator ()(const T & lhs, const T & rhs) {
		return lhs < rhs;
	}

};

template<class T>
class GreaterEqual {
public:
	bool operator ()(const T & lhs, const T & rhs) {
		return lhs >= rhs;
	}
	
};

template<class T>
class LessEqual {
public:
	bool operator ()(const T & lhs, const T & rhs) {
		return lhs > rhs;
	}
	
};

#endif

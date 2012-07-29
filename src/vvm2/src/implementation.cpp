#include <vvm/vvm.h>

namespace vvm {
	const vector<uint8> pack(const vector<uint16>& a, const vector<uint16>& b) {
		return priv::pack<uint8, uint16>::exec(a, b);
	}
	const vector<sint8> pack(const vector<sint16>& a, const vector<sint16>& b);
	const vector<bint8> pack(const vector<bint16>& a, const vector<bint16>& b);

	const vector<uint16> pack(const vector<uint32>& a, const vector<uint32>& b);
	const vector<sint16> pack(const vector<sint32>& a, const vector<sint32>& b);
	const vector<bint16> pack(const vector<bint32>& a, const vector<bint32>& b);
	const vector<uint16> pack(const vector<uint8>& a, const vector<uint8>& b) {
		return priv::pack<uint16, uint8>::exec(a, b);
	}
	const vector<uint32> pack(const vector<uint16>& a, const vector<uint16>& b) {
		return priv::pack<uint32, uint16>::exec(a, b);
	}
	const vector<sint16> pack(const vector<sint8>& a, const vector<sint8>& b) {
		return priv::pack<sint16, sint8>::exec(a, b);
	}
	const vector<sint32> pack(const vector<sint16>& a, const vector<sint16>& b) {
		return priv::pack<sint32, sint16>::exec(a, b);
	}
} // End of vvm namespace

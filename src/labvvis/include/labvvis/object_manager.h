/*
 *  object_manager.h
 *  labvvis
 *
 *  Created by Lai Bing-Chang on 2006/8/3.
 *  Copyright 2006. All rights reserved.
 *
 */

#include <map>

template<typename idT, typename objectT>
class object_manager : public std::map<idT, objectT> {
public:
	object_manager() : _last_id(0) {
	}
public:
	idT new_object(const objectT &o) {
		++_last_id;
		insert(std::make_pair(_last_id, o));
		return _last_id;
	}
private:
	idT _last_id;
};

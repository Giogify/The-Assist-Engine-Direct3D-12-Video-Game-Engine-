#pragma once
#include "IndexedTriangleList.h"
#include <string>
#include <map>

class ITL_Collection {
	
	static inline std::map<std::string, IndexedTriangleList> ITLCollection{};

public:

	static auto& getCollection() noexcept { return ITLCollection; }

	static void add(std::string& name, IndexedTriangleList& itl) noexcept {
		ITLCollection.insert(std::pair<std::string, IndexedTriangleList>(name, itl));
	}

};
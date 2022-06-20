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

class ActorID_Collection {

	static inline std::vector<std::pair<std::string, UINT>> mRegisteredActorIDs{};

public:

	static auto& getCollection() noexcept { return mRegisteredActorIDs; }
	static UINT addCollection(std::string& actorID) noexcept {
		
		for (auto& s : mRegisteredActorIDs) {
			if (actorID == s.first) {
				s.second++;
				return s.second;
			}
		}
		std::pair<std::string, UINT> newActorID{ actorID, 1u };
		mRegisteredActorIDs.push_back(newActorID);
		return 1u;
	}

};
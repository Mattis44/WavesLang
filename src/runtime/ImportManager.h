#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <filesystem>
#include "common/Entries.h"

class ImportManager {
public:
	void addImport(const std::string& alias, const ImportEntry& entry) {
		imports[alias] = entry;
	}

	const ImportEntry* get(const std::string& alias) const {
		auto it = imports.find(alias);
		if (it != imports.end()) return &it->second;
		return nullptr;
	}

private:
	std::unordered_map<std::string, ImportEntry> imports;
};
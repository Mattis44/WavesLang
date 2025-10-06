#include "Interpreter.h"
#include <iostream>
#include <filesystem>

void Interpreter::interpret(const std::vector<std::unique_ptr<Stmt>>& statements) {
	for (const auto& stmt : statements) {
		if (stmt) stmt->accept(*this);
	}
}

void Interpreter::visitImportStmt(ImportStmt& stmt) {
	for (const auto& entry : stmt.entries) {
		std::string path = "vendor/" + entry.name + ".wav";

		if (!std::filesystem::exists(path)) {
			std::cerr << "[ImportError] file not found: " << path << "\n";
			continue;
		}

		ImportEntry record {
			entry.name,
			entry.alias,
			path
		};

		importManager.addImport(entry.alias, record);
		std::cout << "Imported " << entry.name << " as " << entry.alias << "\n";
	}
}

void Interpreter::visitPlayStmt(PlayStmt& stmt) {
	const ImportEntry* entry = importManager.get(stmt.alias);
	if (!entry) {
		std::cerr << "[RuntimeError] Unknown alias: " << stmt.alias << "\n";
		return;
	}

	std::cout << "Playing sample: " << stmt.alias << " (" << entry->path << ")\n";
}
#include "Interpreter.h"
#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <audio/engine.h>

Interpreter::Interpreter() {
	initParamHandlers();
	initLoopActions();
}

void Interpreter::initParamHandlers() {
	paramHandlers["sample"] = [this](const ParamEntry& p) {
		currentSample = p.value;
		std::cout << "  [set] sample -> " << currentSample << "\n";
	};

	paramHandlers["volume"] = [this](const ParamEntry& p) {
		currentVolume = std::stod(p.value);
		std::cout << "  [set] volume -> " << currentVolume << "\n";
	};

	paramHandlers["pitch"] = [this](const ParamEntry& p) {
		currentPitch = std::stod(p.value);
		std::cout << "  [set] pitch -> " << currentPitch << "\n";
	};
}

void Interpreter::initLoopActions() {
	loopActions["play"] = [this](const ParamEntry& p) {
		const ImportEntry* entry = importManager.get(p.value);
		if (!entry) {
			std::cerr << "[RuntimeError] Unknown alias: " << p.value << "\n";
			return;
		}
		playWav(entry->path);
		std::cout << "  [loop] Playing " << p.value
			<< " -> " << entry->path
			<< " (vol=" << currentVolume
			<< ", pitch=" << currentPitch << ")\n";
	};

	/*loopActions["stop"] = [this](const ParamEntry& p) {
		std::cout << "  [loop] Stopping " << p.value << "\n";
		};

	loopActions["wait"] = [this](const ParamEntry& p) {
		int ms = std::stoi(p.value);
		std::cout << "  [loop] Waiting " << ms << " ms\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		};*/
}


void Interpreter::interpret(const std::vector<std::unique_ptr<Stmt>>& statements) {
	for (const auto& stmt : statements) {
		if (stmt) stmt->accept(*this);
	}
}

void Interpreter::visitImportStmt(ImportStmt& stmt) {
	for (const auto& entry : stmt.entries) {
		std::string path = "src/vendor/" + entry.name + ".wav";
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

void Interpreter::visitSetStmt(SetStmt& stmt) {
	std::cout << "Setting " << stmt.alias << ":\n";
	for (const auto& param : stmt.params) {
		auto it = paramHandlers.find(param.name);
		if (it != paramHandlers.end()) {
			it->second(param);
		}else {
			std::cerr << "[Warning] Unknown parameter: " << param.name << "\n";
		}
	}
}

void Interpreter::visitCpmStmt(CpmStmt& stmt) {
	cpm = stmt.value;
	std::cout << "[CPM] CPM set to " << cpm << "\n";
}

void Interpreter::visitLoopStmt(LoopStmt& stmt) {
	std::cout << "[LOOP] Starting loop at " << cpm << " CPM.\n";

	const double beatDurationMs = (60.0 / cpm) * 1000.0;

	const size_t stepCount = stmt.params.size();
	if (stepCount == 0) {
		std::cerr << "[LoopError] Empty loop block.\n";
		return;
	}

	const double stepDurationMs = beatDurationMs / stepCount;

	while (true) {
		int index = 0;
		for (const auto& action : stmt.params) {
			auto it = loopActions.find(action.name);
			if (it == loopActions.end()) {
				std::cerr << "[Warning] Unknown loop action: " << action.name << "\n";
				continue;
			}

			std::thread([=]() {
				std::this_thread::sleep_for(std::chrono::milliseconds((int)(index * stepDurationMs)));
				it->second(action);
				}).detach();

			index++;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds((int)beatDurationMs));
	}

	std::cout << "[LOOP] End of loop.\n";
}

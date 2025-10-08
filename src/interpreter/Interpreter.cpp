#include "Interpreter.h"
#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <algorithm>
#include <audio/engine.h>
#include <cmath>
#include <exception>

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

        while (true) {
            double offsetBeats = 0.0;
            double maxBeats = 0.0;

            for (const auto& action : stmt.params) {
                if (action.name == "wait") {
                    double beatsToWait = parseBeatValue(action.value);
                    if (beatsToWait < 0.0) {
                        std::cerr << "[LoopError] Invalid wait value: " << action.value << "\n";
                        continue;
                    }

                    offsetBeats += beatsToWait;
                    maxBeats = (std::max)(maxBeats, offsetBeats);

                    std::cout << "  [loop] Waiting " << beatsToWait << " beat(s)\n";
                    continue;
                }

                auto it = loopActions.find(action.name);
                if (it == loopActions.end()) {
                    std::cerr << "[Warning] Unknown loop action: " << action.name << "\n";
                    continue;
                }

                double actionOffsetMs = offsetBeats * beatDurationMs;

                std::thread([=]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(std::round(actionOffsetMs))));
                    it->second(action);
                    }).detach();

                offsetBeats += 1.0;
                maxBeats = (std::max)(maxBeats, offsetBeats);
            }

            double loopDurationBeats = (std::max)(1.0, maxBeats);
            auto sleepDurationMs = static_cast<int>(std::round(loopDurationBeats * beatDurationMs));

            std::this_thread::sleep_for(std::chrono::milliseconds(sleepDurationMs));
        }

        std::cout << "[LOOP] End of loop.\n";
}

double Interpreter::parseBeatValue(const std::string& value) const {
    if (value.empty()) {
        return 0.0;
    }

    const auto slashPos = value.find('/');
    if (slashPos != std::string::npos) {
        try {
            double numerator = std::stod(value.substr(0, slashPos));
            double denominator = std::stod(value.substr(slashPos + 1));
            if (denominator == 0.0) {
                 return -1.0;
            }
            return numerator / denominator;
        } catch (const std::exception&) {
                return -1.0;
        }
    }

    try {
        return std::stod(value);
    } catch (const std::exception&) {
         return -1.0;
    }
}

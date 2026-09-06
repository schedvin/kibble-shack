#pragma once
#include <string>
#include <vector>
#include <Preferences.h>
#include <Arduino.h>

class ConfigurationManager {
private:
    Preferences preferences;
    std::string spaceName;

public:
    ConfigurationManager(std::string spaceName = "config") 
        : spaceName(spaceName) {}

    void begin(bool readOnly = false) {
        preferences.begin(spaceName.c_str(), readOnly);
    }

    void end() {
        preferences.end();
    }

    bool hasKey(const std::string& key) {
        return preferences.isKey(key.c_str());
    }

    bool hasKeys(const std::vector<std::string>& keys) {
        for (const auto& key : keys) {
            if (!hasKey(key)) return false;
        }
        return true;
    }

    void clearAll() {
        preferences.clear();
    }

    int get(const std::string& key, int defaultValue) {
        return preferences.getInt(key.c_str(), defaultValue);
    }

    bool get(const std::string& key, bool defaultValue) {
        return preferences.getBool(key.c_str(), defaultValue);
    }

    float get(const std::string& key, float defaultValue) {
        return preferences.getFloat(key.c_str(), defaultValue);
    }

    // Handles std::string default values
    std::string get(const std::string& key, const std::string& defaultValue) {
        String val = preferences.getString(key.c_str(), defaultValue.c_str());
        return std::string(val.c_str());
    }

    // ADD THIS: Handles string literals (const char*) directly without triggering the bool trap
    std::string get(const std::string& key, const char* defaultValue) {
        String val = preferences.getString(key.c_str(), defaultValue);
        return std::string(val.c_str());
    }

    // --- PUT OVERLOADS ---
    void put(const std::string& key, int value) {
        preferences.putInt(key.c_str(), value);
    }

    void put(const std::string& key, bool value) {
        preferences.putBool(key.c_str(), value);
    }

    void put(const std::string& key, float value) {
        preferences.putFloat(key.c_str(), value);
    }

    void put(const std::string& key, const std::string& value) {
        preferences.putString(key.c_str(), value.c_str());
    }

    // Allows passing string literals directly like config.put("ssid", "HomeWiFi");
    void put(const std::string& key, const char* value) {
        preferences.putString(key.c_str(), value);
    }
};
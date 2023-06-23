#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

// The structure of a ticker stat event
struct StatTickerParams {
	// person who got a stat
	uintptr_t Receiver;
	// person who is victim of a stat (only exists for demos afaik)
	uintptr_t Victim;
	// wrapper for the stat event
	uintptr_t StatEvent;
};

// structure of a stat event
struct StatEventParams {
	// always primary player
	uintptr_t PRI;
	// wrapper for the stat event
	uintptr_t StatEvent;
};

struct FString
{
public:
    using ElementType = const wchar_t;
    using ElementPointer = ElementType*;

private:
    ElementPointer ArrayData;
    int32_t ArrayCount;
    int32_t ArrayMax;

public:
    FString() {
        ArrayData = nullptr;
        ArrayCount = 0;
        ArrayMax = 0;
    }

    FString(ElementPointer other) {
        ArrayData = nullptr;
        ArrayCount = 0;
        ArrayMax = 0;

        ArrayMax = ArrayCount = *other ? (wcslen(other) + 1) : 0;

        if (ArrayCount > 0) {
            ArrayData = other;
        }
    }

    ~FString() {}

public:
    std::string ToString() const {
        if (!IsValid()) {
            std::wstring wideStr(ArrayData);
            std::string str(wideStr.begin(), wideStr.end());
            return str;
        }

        return std::string("null");
    }

    bool IsValid() const {
        return !ArrayData;
    }

    FString operator=(ElementPointer other)
    {
        if (ArrayData != other) {
            ArrayMax = ArrayCount = *other ? (wcslen(other) + 1) : 0;
            if (ArrayCount > 0) {
                ArrayData = other;
            }
        }

        return *this;
    }

    bool operator==(const FString& other) {
        return (!wcscmp(ArrayData, other.ArrayData));
    }
};

struct FUniqueNetId{
	unsigned char struct_size[0x48];
};

struct FChatMessage {
	void* PRI;
	void* Team;
	wchar_t* PlayerName;
	uint8_t PlayerNamePadding[0x8];
	wchar_t* Message;
	uint8_t MessagePadding[0x8];
	uint8_t ChatChannel;
	unsigned long bPreset : 1;
};

struct FChatMessage2 {
	class PriWrapper PRI; // 0x0000 (0x0008) [0x0000000000000000]               
	class ATeam_TA* Team; // 0x0008 (0x0008) [0x0000000000000000]               
	struct FString PlayerName; // 0x0010 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
	struct FString Message; // 0x0020 (0x0010) [0x0000000000400000] (CPF_NeedCtorLink)
	uint8_t ChatChannel; // 0x0030 (0x0001) [0x0000000000000000]               
	uint8_t UnknownData00[0x3]; // 0x0031 (0x0003) MISSED OFFSET
	unsigned long bPreset : 1; // 0x0034 (0x0004) [0x0000000000000000] [0x00000001] 
	struct FUniqueNetId Recipient; // 0x0038 (0x0048) [0x0000000000400000] (CPF_NeedCtorLink)
};

class bakkesmod_gpt : public BakkesMod::Plugin::BakkesModPlugin/*, public BakkesMod::Plugin::PluginSettingsWindow*//*, public BakkesMod::Plugin::PluginWindow*/
{

	//std::shared_ptr<bool> enabled;

	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();
	void appendLog(std::string message = "Default log");
	void onStatTickerMessage(void* params);
	void onStatEvent(void* params);
	void onPlayerMessage(void* params);

	// Inherited via PluginSettingsWindow
	/*
	void RenderSettings() override;
	std::string GetPluginName() override;
	void SetImGuiContext(uintptr_t ctx) override;
	*/

	// Inherited via PluginWindow
	/*

	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string menuTitle_ = "bakkesmod_gpt";

	virtual void Render() override;
	virtual std::string GetMenuName() override;
	virtual std::string GetMenuTitle() override;
	virtual void SetImGuiContext(uintptr_t ctx) override;
	virtual bool ShouldBlockInput() override;
	virtual bool IsActiveOverlay() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;

	*/
};
#include "pch.h"
#include "bakkesmod_gpt.h"

/* Plugin Settings Window code here
std::string bakkesmod_gpt::GetPluginName() {
	return "bakkesmod_gpt";
}

void bakkesmod_gpt::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Render the plugin settings here
// This will show up in bakkesmod when the plugin is loaded at
//  f2 -> plugins -> bakkesmod_gpt
void bakkesmod_gpt::RenderSettings() {
	ImGui::TextUnformatted("bakkesmod_gpt plugin settings");
}
*/

/*
// Do ImGui rendering here
void bakkesmod_gpt::Render()
{
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

// Name of the menu that is used to toggle the window.
std::string bakkesmod_gpt::GetMenuName()
{
	return "bakkesmod_gpt";
}

// Title to give the menu
std::string bakkesmod_gpt::GetMenuTitle()
{
	return menuTitle_;
}

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void bakkesmod_gpt::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool bakkesmod_gpt::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool bakkesmod_gpt::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void bakkesmod_gpt::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void bakkesmod_gpt::OnClose()
{
	isWindowOpen_ = false;
}
*/

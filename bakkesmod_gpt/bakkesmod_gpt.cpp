#include "pch.h"
#include "bakkesmod_gpt.h"
#include <fstream>

std::map<std::wstring, std::wstring> message_map = {
    {L"Group4Message1", L"$#@%!"},
    {L"Group1Message7", L"All yours."},
    {L"Group1Message11", L"Bumping!"},
    {L"Group3Message9", L"Calculated."},
    {L"Group1Message6", L"Centering!"},
    {L"Group3Message4", L"Close one!"},
    {L"Group1Message4", L"Defending..."},
    {L"Group1Message10", L"Faking."},
    {L"Group1Message5", L"Go for it!"},
    {L"Group2Message7", L"Great clear!"},
    {L"Group2Message2", L"Great pass!"},
    {L"Group3Message6", L"Holy cow!"},
    {L"Group1Message1", L"I got it!"},
    {L"Group1Message8", L"In position."},
    {L"Group1Message9", L"Incoming!"},
    {L"Group4Message5", L"My bad..."},
    {L"Group4Message7", L"My fault."},
    {L"Group1Message2", L"Need boost!"},
    {L"Group2Message8", L"Nice block!"},
    {L"Group2Message9", L"Nice bump!"},
    {L"Group2Message10", L"Nice demo!"},
    {L"Group2Message5", L"Nice one!"},
    {L"Group2Message1", L"Nice shot!"},
    {L"Group4Message2", L"No problem."},
    {L"Group3Message5", L"No way!"},
    {L"Group3Message2", L"Noooo!"},
    {L"Group3Message1", L"OMG!"},
    {L"Group3Message11", L"Okay."},
    {L"Group1Message12", L"On your left."},
    {L"Group1Message13", L"On your right."},
    {L"Group4Message6", L"Oops!"},
    {L"Group1Message14", L"Passing!"},
    {L"Group3Message10", L"Savage!"},
    {L"Group3Message8", L"Siiiick!"},
    {L"Group4Message4", L"Sorry!"},
    {L"Group1Message3", L"Take the shot!"},
    {L"Group2Message3", L"Thanks!"},
    {L"Group6Message4", L"This is Rocket League!"},
    {L"Group2Message6", L"What a play!"},
    {L"Group2Message4", L"What a save!"},
    {L"Group3Message7", L"Whew."},
    {L"Group4Message3", L"Whoops..."},
    {L"Group3Message3", L"Wow!"},
    {L"Group5Message1", L"gg"},
    {L"Group5Message2", L"Well played."},
    {L"Group5Message3", L"That was fun!"},
    {L"Group5Message4", L"Rematch!"},
    {L"Group5Message5", L"One. More. Game."},
    {L"Group5Message6", L"What a game!"},
    { L"Group5Message7", L"Nice moves." },
    { L"Group5Message8", L"Everybody dance!" },
    { L"Group6Message1", L"Good luck!" },
    { L"Group6Message2", L"Have fun!" },
    { L"Group6Message3", L"Get ready." },
    { L"Group6Message4", L"This is Rocket League!" },
    { L"Group6Message5", L"Let's do this!" },
    { L"Group6Message6", L"Here. We. Go." },
    { L"Group6Message7", L"Nice cars!" },
    { L"Group6Message8", L"I'll do my best." }
};


BAKKESMOD_PLUGIN(bakkesmod_gpt, "Gets the gamestate of rocket league for GPT", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void bakkesmod_gpt::onLoad()
{
    // This line is required for LOG to work and must be before any use of LOG()
    _globalCvarManager = cvarManager;
    // do something when it loads
    LOG("Bakkesmod_GPT has been loaded");

    // Reset the log file:
    deleteLog();

    cvarManager->registerNotifier("WriteToLog", [this](std::vector<std::string> args) {
        appendLog();
        }, "", PERMISSION_ALL);

    cvarManager->registerNotifier("GameState", [this](std::vector<std::string> args) {
        gameState();
        }, "", PERMISSION_ALL);

    // We need the params so we hook with caller, but there is no wrapper for the HUD
    gameWrapper->HookEventWithCallerPost<ServerWrapper>("Function TAGame.GFxHUD_TA.HandleStatTickerMessage",
        [this](ServerWrapper caller, void* params, std::string eventname) {
            if (params == nullptr) {
                LOG("NULL params passed to HandleStatTickerMessage");
                return;
            }
            onStatTickerMessage(params);
        });

    // Hooked whenever the primary player earns a stat
    gameWrapper->HookEventWithCallerPost<ServerWrapper>("Function TAGame.GFxHUD_TA.HandleStatEvent",
        [this](ServerWrapper caller, void* params, std::string eventname) {
            if (params == nullptr) {
                LOG("NULL params passed to HandleStatEvent");
                return;
            }
            onStatEvent(params);
        });

    gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.HUDBase_TA.OnChatMessage",
        [this](ActorWrapper caller, void* params, ...) {
            if (params == nullptr) {
                LOG("NULL params passed to OnChatMessage");
                return;
            }
            onPlayerMessage(params);
        });
}

void bakkesmod_gpt::onUnload() {
    LOG("Bakkesmod_GPT has been unloaded");
}

void bakkesmod_gpt::gameState() {
    ServerWrapper server = gameWrapper->GetCurrentGameState();
    if (!server.IsNull()) {
        int time_left = server.GetSecondsRemaining();

        if (server.GetTeams().Count() != 2) { return; }

        // Get the primary player's team.
        PlayerControllerWrapper playerController = gameWrapper->GetPlayerController();
        if (!playerController) { LOG("Null controller"); return; }
        PriWrapper playerPRI = playerController.GetPRI();
        if (!playerPRI) { LOG("Null player PRI"); return; }
        TeamInfoWrapper playerTeam = playerPRI.GetTeam();

        if (!playerTeam.IsNull()) {
            int playerTeamId = playerTeam.GetTeamIndex();
            int opposionTeamId = 1 - playerTeamId;
            int playerTeamScore = playerTeam.GetScore();

            TeamWrapper oppositionTeam = server.GetTeams().Get(opposionTeamId);
            if (!oppositionTeam.IsNull()) {
                int oppositionTeamScore = oppositionTeam.GetScore();

                // Open the file to overwrite.
                std::fstream logFile("E:/Documents/My Games/Rocket League/output.txt", std::ios_base::in | std::ios_base::out);
                if (!logFile.is_open()) {
                    LOG("Failed to open log file");
                    return;
                }

                std::string score = "";
                if (playerTeamScore > oppositionTeamScore) {
                    score = "[GAME] YOU are winning " + std::to_string(playerTeamScore) + " - " + std::to_string(oppositionTeamScore);
                }
                else if (playerTeamScore < oppositionTeamScore) {
                    score = "[GAME] YOU are losing " + std::to_string(playerTeamScore) + " - " + std::to_string(oppositionTeamScore);
				}
                else {
                    score = "[GAME] YOU are drawing " + std::to_string(playerTeamScore) + " - " + std::to_string(oppositionTeamScore);
				}

                std::string seconds = "[GAME] There are " + std::to_string(time_left) + " seconds remaining";

                // Write to the beginning of the file with the current time [HH:MM:SS] and the message.
                logFile.seekp(0);
                logFile << seconds << std::endl;
                logFile << score << std::endl;


                // Close the file.
                logFile.close();
			}
            else {
				return;
			}
		}
		else {
			return;
		}

    }
}



void bakkesmod_gpt::onStatEvent(void* params) {
    StatEventParams* pStruct = (StatEventParams*)params;
    if (pStruct == nullptr) {
        LOG("NULL StatEventParams");
        return;
    }
    PriWrapper playerPRI = PriWrapper(pStruct->PRI);
    StatEventWrapper statEvent = StatEventWrapper(pStruct->StatEvent);
}

void bakkesmod_gpt::deleteLog() {
    // Open the file to truncate it (recreate as a blank file).
    std::ofstream logFile("E:/Documents/My Games/Rocket League/output.txt", std::ios_base::trunc);
    if (!logFile.is_open()) {
        LOG("Failed to open log file");
        return;
    }

    // Close the file.
    logFile.close();
}

void bakkesmod_gpt::appendLog(std::string message) {

    // Update the GameState in the file.
    gameState();

    // Get the current time:
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    if (timeinfo == nullptr) {
        LOG("Cannot get current time");
        return;
    }
    char timeBuffer[9]; // Buffer to hold the formatted time
    std::strftime(timeBuffer, sizeof(timeBuffer), "%T", timeinfo);

    // Open the file to append to.
    std::ofstream logFile("E:/Documents/My Games/Rocket League/output.txt", std::ios_base::app);
    if (!logFile.is_open()) {
        LOG("Failed to open log file");
        return;
    }

    // Log to the file with the current time [HH:MM:SS] and the message.
    logFile << "[" << timeBuffer << "] " << message << std::endl;

    // Close the file.
    logFile.close();

};

void bakkesmod_gpt::onPlayerMessage(void* params) {

    FChatMessage* chatMessage = static_cast<FChatMessage*>(params);
    if (chatMessage == nullptr) {
        LOG("NULL FChatMessage");
        return;
    }

    std::wstring playerName(chatMessage->PlayerName);
    std::wstring message(chatMessage->Message);

    // Replace message if it's in the map
    auto map_iter = message_map.find(message);
    if (map_iter != message_map.end()) {
        message = map_iter->second;
    }

    std::string line = "[CHAT] " + std::string(playerName.begin(), playerName.end()) + ": " + std::string(message.begin(), message.end());
    LOG(line);
    appendLog(line);
};


// defined elsewhere in .cpp
void bakkesmod_gpt::onStatTickerMessage(void* params) {

    if (!params) { LOG("Null params pointer"); return; }

    struct StatTickerParams {
        uintptr_t Receiver;
        uintptr_t Victim;
        uintptr_t StatEvent;
    };

    StatTickerParams* pStruct = (StatTickerParams*)params;
    PriWrapper receiver = PriWrapper(pStruct->Receiver);
    PriWrapper victim = PriWrapper(pStruct->Victim);
    StatEventWrapper statEvent = StatEventWrapper(pStruct->StatEvent);

    // Verify that the StatEventWrapper object has a valid GetEventName()
    if (statEvent.GetEventName().empty()) { LOG("Invalid event name"); return; }

    // Log the event name
    LOG("StatEvent:" + statEvent.GetEventName());

    // Get the primary player's PRI.
    PlayerControllerWrapper playerController = gameWrapper->GetPlayerController();
    if (!playerController) { LOG("Null controller"); return; }
    PriWrapper playerPRI = playerController.GetPRI();
    if (!playerPRI) { LOG("Null player PRI"); return; }

    // Check if the receiver is null.
    if (!receiver) { LOG("Null reciever PRI"); return; }

    // Check if the receiver is the primary player
    std::string receiverDescription = "";
    if (playerPRI.memory_address == receiver.memory_address) {
        receiverDescription = "[YOU]";
	}
	else {
        TeamInfoWrapper receiverTeam = receiver.GetTeam();
        TeamInfoWrapper playerTeam = playerPRI.GetTeam();

        if (playerTeam.IsNull() == false && receiverTeam.IsNull() == false) {
            if (receiverTeam.GetTeamIndex() == playerTeam.GetTeamIndex()) {
                receiverDescription = receiver.GetPlayerName().ToString() + " [TEAMMATE]";
			}
			else {
                receiverDescription = receiver.GetPlayerName().ToString() + " [OPPONENT]";
			}
        }
	}

    if (statEvent.GetEventName() == "Demolish") {
        // Return if either the victim is null.
        std::string victimDescription = "";
        std::string line = "";
        if (!victim) { LOG("Null victim PRI"); return; }
        if (playerPRI.memory_address == victim.memory_address) {
            line = receiverDescription + " demolished " + " [YOU]";
        }
        else {
            line = receiverDescription + " demolished " + victim.GetPlayerName().ToString();
        }
        
        LOG(line);
        appendLog(line);
    }
    else if (statEvent.GetEventName() == "Goal") {
        std::string line = receiverDescription + " scored a goal";
        LOG(line);
        appendLog(line);
    }
    else if (statEvent.GetEventName() == "OvertimeGoal") {
        std::string line = receiverDescription + " scored an overtime goal";
        LOG(line);
        appendLog(line);
    }
    else if (statEvent.GetEventName() == "Assist") {
        std::string line = receiverDescription + " made an assist";
        LOG(line);
        appendLog(line);
    }
    else if (statEvent.GetEventName() == "Save") {
        std::string line = receiverDescription + " made a save";
        LOG(line);
        appendLog(line);
    }
    else if (statEvent.GetEventName() == "EpicSave") {
        std::string line = receiverDescription + " made an epic save";
        LOG(line);
        appendLog(line);
    }
    else if (statEvent.GetEventName() == "OwnGoal") {
        std::string line = receiverDescription + " scored an own goal";
        LOG(line);
        appendLog(line);
    }
    else if (statEvent.GetEventName() == "BreakoutDamage") {
        std::string line = receiverDescription + " scored dropshot damage";
        LOG(line);
        appendLog(line);
    }
    else if (statEvent.GetEventName() == "BreakoutDamageLarge") {
        std::string line = receiverDescription + " scored large dropshot damage";
        LOG(line);
        appendLog(line);
    }
}


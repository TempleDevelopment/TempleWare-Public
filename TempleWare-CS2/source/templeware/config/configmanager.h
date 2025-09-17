#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cstdlib>

#include "config.h"
#include "../../../external/json/json.hpp"

namespace internal_config
{
    class ConfigManager
    {
    private:

        static std::filesystem::path GetConfigFolder()
        {
            char* userProfile = nullptr;
            size_t len = 0;
            errno_t err = _dupenv_s(&userProfile, &len, "USERPROFILE");

            std::filesystem::path folder;
            if (err != 0 || userProfile == nullptr || len == 0)
            {
                folder = ".templeware";
            }
            else
            {
                folder = userProfile;
                free(userProfile);
                folder /= ".templeware";
            }
            folder /= "internal";

            std::error_code ec;
            std::filesystem::create_directories(folder, ec);

            return folder;
        }

        static std::filesystem::path GetConfigPath(const std::string& configName)
        {
            auto folder = GetConfigFolder();
            return folder / (configName + ".json");
        }

    public:

        static std::vector<std::string> ListConfigs()
        {
            std::vector<std::string> list;
            auto folder = GetConfigFolder();
            if (!std::filesystem::exists(folder))
                return list;

            for (const auto& entry : std::filesystem::directory_iterator(folder))
            {
                if (entry.is_regular_file())
                {
                    auto path = entry.path();
                    if (path.extension() == ".json")
                    {
                        list.push_back(path.stem().string());
                    }
                }
            }
            return list;
        }

        static void Save(const std::string& configName)
        {
            nlohmann::json j;
            // aimbot
            j["aimbot"] = Config::aimbot;
            j["aimbot_fov"] = Config::aimbot_fov;
            j["fov_circle"] = Config::fov_circle;
            j["rcs"] = Config::rcs;
            j["Trigger"] = Config::triggerBot;
            j["fovCircleColor"] = {
                Config::fovCircleColor.x,
                Config::fovCircleColor.y,
                Config::fovCircleColor.z,
                Config::fovCircleColor.w
            };


            // esp
            j["esp"] = Config::esp;
            j["showHealth"] = Config::showHealth;
            j["teamCheck"] = Config::teamCheck;
            j["espFill"] = Config::espFill;
            j["espThickness"] = Config::espThickness;
            j["espFillOpacity"] = Config::espFillOpacity;
            j["showNameTags"] = Config::showNameTags;
            j["espColor"] = {
                Config::espColor.x,
                Config::espColor.y,
                Config::espColor.z,
                Config::espColor.w
            };


            // chams
            j["chamsMaterial"] = Config::chamsMaterial;
            j["enemyChams"] = Config::enemyChams;
            j["enemyChamsInvisible"] = Config::enemyChamsInvisible;
            j["viewmodelChams"] = Config::viewmodelChams;
            j["armChams"] = Config::armChams;
            j["teamChams"] = Config::teamChams; // not used :/
            j["teamChamsInvisible"] = Config::teamChamsInvisible; // it to
            j["colVisualChams"] = {
                Config::colVisualChams.x,
                Config::colVisualChams.y,
                Config::colVisualChams.z,
                Config::colVisualChams.w
            };
            j["colVisualChamsIgnoreZ"] = {
                Config::colVisualChamsIgnoreZ.x,
                Config::colVisualChamsIgnoreZ.y,
                Config::colVisualChamsIgnoreZ.z,
                Config::colVisualChamsIgnoreZ.w
            };
            j["viewmodelChams_color"] = {
                Config::colViewmodelChams.x,
                Config::colViewmodelChams.y,
                Config::colViewmodelChams.z,
                Config::colViewmodelChams.w
            };
            j["armChams_color"] = {
                Config::colArmChams.x,
                Config::colArmChams.y,
                Config::colArmChams.z,
                Config::colArmChams.w
            };
            j["teamcolVisualChams"] = {
                Config::teamcolVisualChams.x,
                Config::teamcolVisualChams.y,
                Config::teamcolVisualChams.z,
                Config::teamcolVisualChams.w
            };
            j["teamcolVisualChamsIgnoreZ"] = {
                Config::teamcolVisualChamsIgnoreZ.x,
                Config::teamcolVisualChamsIgnoreZ.y,
                Config::teamcolVisualChamsIgnoreZ.z,
                Config::teamcolVisualChamsIgnoreZ.w
            };


            // removals
            j["antiflash"] = Config::antiflash;
            j["fovEnabled"] = Config::fovEnabled;
            j["fov"] = Config::fov;
            j["SmokeRemove"] = Config::SmokeRemove;
            j["ScopeRemove"] = Config::ScopeRemove;
            j["LegsRemove"] = Config::removelegs;


            // misc
            j["SkyBox"] = Config::skybox;
            j["SkyBoxColor"] = {
                Config::skyboxcolor.x,
                Config::skyboxcolor.y,
                Config::skyboxcolor.z,
                Config::skyboxcolor.w
            };
            j["Fog"] = Config::fog;
            j["Night"] = Config::Night;
            j["NightColor"] = {
                Config::NightColor.x,
                Config::NightColor.y,
                Config::NightColor.z,
                Config::NightColor.w
            };
            j["Light"] = Config::light;
            j["LightColor"] = {
                Config::DrawLight.x,
                Config::DrawLight.y,
                Config::DrawLight.z,
                Config::DrawLight.w
            };
            j["Viewmodel"] = Config::viewmodell;
            j["Viewx"] = Config::viewx;
            j["Viewy"] = Config::viewy;
            j["Viewz"] = Config::viewz;
            j["Ifov"] = Config::ifov;

            auto filePath = GetConfigPath(configName);
            std::ofstream ofs(filePath);
            if (ofs.is_open())
            {
                ofs << j.dump(4);
                ofs.close();
            }
        }

        static void Load(const std::string& configName)
        {
            auto filePath = GetConfigPath(configName);
            if (!std::filesystem::exists(filePath))
                return;

            std::ifstream ifs(filePath);
            if (!ifs.is_open())
                return;

            nlohmann::json j;
            ifs >> j;

            // aimbot
            Config::aimbot = j.value("aimbot", false);
            Config::fov_circle = j.value("fov_circle", false);
            Config::aimbot_fov = j.value("aimbot_fov", 0.f);
            Config::rcs = j.value("rcs", false);
            Config::triggerBot = j.value("Trigger", false);
            if (j.contains("fovCircleColor") && j["fovCircleColor"].is_array() && j["fovCircleColor"].size() == 4) {
                auto arr = j["fovCircleColor"];
                Config::fovCircleColor.x = arr[0].get<float>();
                Config::fovCircleColor.y = arr[1].get<float>();
                Config::fovCircleColor.z = arr[2].get<float>();
                Config::fovCircleColor.w = arr[3].get<float>();
            }


            // esp
            Config::esp = j.value("esp", false);
            Config::espFill = j.value("espFill", false);
            Config::espFillOpacity = j.value("espFillOpacity", 0.5f);
            Config::espThickness = j.value("espThickness", 1.0f);
            Config::teamCheck = j.value("teamCheck", false);
            Config::showHealth = j.value("showHealth", false);
            Config::showNameTags = j.value("showNameTags", false);
            if (j.contains("espColor") && j["espColor"].is_array() && j["espColor"].size() == 4)
            {
                auto arr = j["espColor"];
                Config::espColor.x = arr[0].get<float>();
                Config::espColor.y = arr[1].get<float>();
                Config::espColor.z = arr[2].get<float>();
                Config::espColor.w = arr[3].get<float>();
            }


            // chams
            Config::enemyChamsInvisible = j.value("enemyChamsInvisible", false);
            Config::enemyChams = j.value("enemyChams", false);
            Config::teamChams = j.value("teamChams", false);
            Config::teamChamsInvisible = j.value("teamChamsInvisible", false);
            Config::chamsMaterial = j.value("chamsMaterial", 0);
            Config::armChams = j.value("armChams", false);
            Config::viewmodelChams = j.value("viewmodelChams", false);

            if (j.contains("armChams_color") && j["armChams_color"].is_array() && j["armChams_color"].size() == 4)
            {
                auto arr = j["armChams_color"];
                Config::colArmChams.x = arr[0].get<float>();
                Config::colArmChams.y = arr[1].get<float>();
                Config::colArmChams.z = arr[2].get<float>();
                Config::colArmChams.w = arr[3].get<float>();
            }

            if (j.contains("viewmodelChams_color") && j["viewmodelChams_color"].is_array() && j["viewmodelChams_color"].size() == 4)
            {
                auto arr = j["viewmodelChams_color"];
                Config::colViewmodelChams.x = arr[0].get<float>();
                Config::colViewmodelChams.y = arr[1].get<float>();
                Config::colViewmodelChams.z = arr[2].get<float>();
                Config::colViewmodelChams.w = arr[3].get<float>();
            }

            if (j.contains("colVisualChams") && j["colVisualChams"].is_array() && j["colVisualChams"].size() == 4)
            {
                auto arr = j["colVisualChams"];
                Config::colVisualChams.x = arr[0].get<float>();
                Config::colVisualChams.y = arr[1].get<float>();
                Config::colVisualChams.z = arr[2].get<float>();
                Config::colVisualChams.w = arr[3].get<float>();
            }

            if (j.contains("colVisualChamsIgnoreZ") && j["colVisualChamsIgnoreZ"].is_array() && j["colVisualChamsIgnoreZ"].size() == 4)
            {
                auto arr = j["colVisualChamsIgnoreZ"];
                Config::colVisualChamsIgnoreZ.x = arr[0].get<float>();
                Config::colVisualChamsIgnoreZ.y = arr[1].get<float>();
                Config::colVisualChamsIgnoreZ.z = arr[2].get<float>();
                Config::colVisualChamsIgnoreZ.w = arr[3].get<float>();
            }

            if (j.contains("teamcolVisualChamsIgnoreZ") && j["teamcolVisualChamsIgnoreZ"].is_array() && j["teamcolVisualChamsIgnoreZ"].size() == 4)
            {
                auto arr = j["teamcolVisualChamsIgnoreZ"];
                Config::teamcolVisualChamsIgnoreZ.x = arr[0].get<float>();
                Config::teamcolVisualChamsIgnoreZ.y = arr[1].get<float>();
                Config::teamcolVisualChamsIgnoreZ.z = arr[2].get<float>();
                Config::teamcolVisualChamsIgnoreZ.w = arr[3].get<float>();
            }

            if (j.contains("teamcolVisualChams") && j["teamcolVisualChams"].is_array() && j["teamcolVisualChams"].size() == 4)
            {
                auto arr = j["teamcolVisualChams"];
                Config::teamcolVisualChams.x = arr[0].get<float>();
                Config::teamcolVisualChams.y = arr[1].get<float>();
                Config::teamcolVisualChams.z = arr[2].get<float>();
                Config::teamcolVisualChams.w = arr[3].get<float>();
            }

            // removals
            Config::antiflash = j.value("antiflash", false);
            Config::SmokeRemove = j.value("SmokeRemove", false);
            Config::ScopeRemove = j.value("ScopeRemove", false);
            Config::removelegs = j.value("LegsRemove", false);


            // misc
            Config::skybox = j.value("SkyBox", false);
            if (j.contains("SkyBoxColor") && j["SkyBoxColor"].is_array() && j["SkyBoxColor"].size() == 4)
            {
                auto arr = j["SkyBoxColor"];
                Config::skyboxcolor.x = arr[0].get<float>();
                Config::skyboxcolor.y = arr[1].get<float>();
                Config::skyboxcolor.z = arr[2].get<float>();
                Config::skyboxcolor.w = arr[3].get<float>();
            }
            Config::fog = j.value("Fog", false);
            Config::Night = j.value("Night", false);
            if (j.contains("NightColor") && j["NightColor"].is_array() && j["NightColor"].size() == 4)
            {
                auto arr = j["NightColor"];
                Config::NightColor.x = arr[0].get<float>();
                Config::NightColor.y = arr[1].get<float>();
                Config::NightColor.z = arr[2].get<float>();
                Config::NightColor.w = arr[3].get<float>();
            }
            Config::fovEnabled = j.value("fovEnabled", false);
            Config::fov = j.value("fov", 90.0f);
            Config::light = j.value("Light", false);
            if (j.contains("LightColor") && j["LightColor"].is_array() && j["LightColor"].size() == 4)
            {
                auto arr = j["LightColor"];
                Config::DrawLight.x = arr[0].get<float>();
                Config::DrawLight.y = arr[1].get<float>();
                Config::DrawLight.z = arr[2].get<float>();
                Config::DrawLight.w = arr[3].get<float>();
            }
            Config::viewmodell = j.value("Viewmodel", false);
            Config::viewx = j.value("Viewx", 0.0f);
            Config::viewy = j.value("Viewy", 0.0f);
            Config::viewz = j.value("Viewz", 0.0f);
            Config::ifov = j.value("Ifov", 100.0f);

            ifs.close();
        }

        static void Remove(const std::string& configName)
        {
            auto filePath = GetConfigPath(configName);
            if (std::filesystem::exists(filePath))
            {
                std::error_code ec;
                std::filesystem::remove(filePath, ec);
            }
        }
    };
}

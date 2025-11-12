#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdint>

#include "json.hpp"

typedef int32_t i32;
typedef uint32_t ui32;

#define ASSERT(expr, msg) do { if (!(expr)) throw std::runtime_error(msg); } while (0)

class Config
{
private:
    Config() { ASSERT(load("config.json"), "Failed to load config.json"); }
    ~Config() = default;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    int display_index = 0;
    int display_height = 0;
    int display_width = 0;
    int window_height = 0;
    int window_width = 0;
    int fps = 0;
    float target_frame_delta = 0.0f;

    float fov = 0.0f;
    float far_plane = 0.0f;
    float near_plane = 0.0f;
    float aspect_ratio = 0.0f;

    std::vector<int> window_flags;
    std::vector<int> renderer_flags;
    std::vector<int> texture_flags;

    // New runtime display & rendering controls
    float depth_scale = 1.0f;          // Controls perspective shrink in layered scene
    float default_parallax = 1.0f;      // Base parallax multiplier if layer not explicit
    bool debug_overlay = false;         // Toggle for on-screen debug

public:
    static Config& get_instance()
    {
        static Config instance;
        return instance;
    }

    bool load(const std::string& filename)
    {
        std::ifstream in(filename);
        if (!in) return false;
        nlohmann::json j;
        in >> j;

        if (j.contains("display_index")) display_index = j["display_index"]; else return false;
        if (j.contains("display_width")) display_width = j["display_width"]; else return false;
        if (j.contains("display_height")) display_height = j["display_height"]; else return false;
        if (j.contains("window_width")) window_width = j["window_width"]; else return false;
        if (j.contains("window_height")) window_height = j["window_height"]; else return false;
        if (j.contains("fps")) fps = j["fps"]; else return false;
        if (j.contains("fov")) fov = j["fov"]; else return false;
        if (j.contains("far_plane")) far_plane = j["far_plane"]; else return false;
        if (j.contains("near_plane")) near_plane = j["near_plane"]; else return false;

        if (j.contains("window_flags")) window_flags = j["window_flags"].get<std::vector<int>>(); else return false;
        if (j.contains("renderer_flags")) renderer_flags = j["renderer_flags"].get<std::vector<int>>(); else return false;
        if (j.contains("texture_flags")) texture_flags = j["texture_flags"].get<std::vector<int>>(); else return false;

        if (j.contains("depth_scale")) depth_scale = j["depth_scale"].get<float>();
        if (j.contains("default_parallax")) default_parallax = j["default_parallax"].get<float>();
        if (j.contains("debug_overlay")) debug_overlay = j["debug_overlay"].get<bool>();

            target_frame_delta = (1000.0f / static_cast<float>(fps));
            ASSERT(target_frame_delta > 0.0f, "Invalid target frame delta");

            aspect_ratio = static_cast<float>(window_width) / static_cast<float>(window_height);

            return true;
        }

    int get_display_index() const { return display_index; }
    void set_display_index(int v) { display_index = v; }

    int get_display_height() const { return display_height; }
    void set_display_height(int v) { display_height = v; }

    int get_display_width() const { return display_width; }
    void set_display_width(int v) { display_width = v; }

    int get_window_height() const { return window_height; }
    void set_window_height(int v) { window_height = v; }

    int get_window_width() const { return window_width; }
    void set_window_width(int v) { window_width = v; }

    int get_fps() const { return fps; }
    void set_fps(int v) { fps = v; }

    float get_target_frame_delta() const { return target_frame_delta; }
    void set_target_frame_delta(float v) { target_frame_delta = v; }

    float get_fov() const { return fov; }
    void set_fov(float v) { fov = v; }

    float get_far_plane() const { return far_plane; }
    void set_far_plane(float v) { far_plane = v; }

    float get_near_plane() const { return near_plane; }
    void set_near_plane(float v) { near_plane = v; }

    float get_aspect_ratio() const { return aspect_ratio; }
    void set_aspect_ratio(float v) { aspect_ratio = v; }

    uint32_t get_window_flags() const
    {
        uint32_t result = 0;
        for (int flag : window_flags) result |= flag;
        return result;
    }

    uint32_t get_renderer_flags() const
    {
        uint32_t result = 0;
        for (int flag : renderer_flags) result |= flag;
        return result;
    }

    uint32_t get_texture_flags() const
    {
        uint32_t result = 0;
        for (int flag : texture_flags) result |= flag;
        return result;
    }

    float get_depth_scale() const { return depth_scale; }
    float get_default_parallax() const { return default_parallax; }
    bool is_debug_overlay() const { return debug_overlay; }
};

#endif
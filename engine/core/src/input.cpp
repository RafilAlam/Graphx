#include <engine/core/include/input.hpp>

namespace Input {
    void RegisterWindow(GLFWwindow* window) {
        glfwSetKeyCallback(window, Input::Dispatch);
    }
    CallbackHandle Subscribe(std::function<void(Key, KeyAction, KeyModifier)> callback) {
        callbacks.emplace_back(callback);
        return {static_cast<uint32_t>(callbacks.size() - 1)};
    }
    void Unsubscribe(CallbackHandle handle) {
        callbacks[handle.id].active = false;
    }
    void Dispatch(
        GLFWwindow* window,
        int key,
        int scancode,
        int action,
        int mods
    ) {
        for (const auto& callback : callbacks) {
            if (callback.active)
                callback.func(Key(key), KeyAction(action), KeyModifier(mods));
        }
    }
}
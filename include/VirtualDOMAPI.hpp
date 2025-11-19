#pragma once
#include <Geode/Geode.hpp>
#include <Geode/loader/Dispatch.hpp>

#define MY_MOD_ID "camila314.uidesigner"
using namespace geode::prelude;

class VirtualNode;
using VirtualCreator = VirtualNode*(*)();

class VirtualDOMManager final {
    std::unordered_map<std::string, VirtualCreator> m_creators;
    std::vector<char const*> m_creatorNames;

    VirtualDOMManager();
public:
    static VirtualDOMManager* get() GEODE_EVENT_EXPORT_NORES(&VirtualDOMManager::get, ());

    virtual void initialize(CCLayer*);
    virtual void registerType(std::string_view name, VirtualCreator ctor);
    virtual VirtualNode* createFromJSON(matjson::Value);

    template <std::derived_from<VirtualNode> T>
    static int registerCreate(char const name[]) {
        VirtualDOMManager::get()->registerType(name, +[]() -> VirtualNode* { return new T(); });
        return 0;
    }
};

template <typename T, string::ConstexprString Name>
class RegisterDOM {
    static inline int registration = VirtualDOMManager::registerCreate<T>(Name.data());
    static inline auto registerRef = &registration;
};

inline std::string fmtFloat(matjson::Value thing) {
    auto val = thing.asDouble().unwrapOr(0.f);
    auto formatted = fmt::format("{:.4f}", val);

    if (auto last = formatted.find_last_not_of('0'); last != std::string::npos) {
        formatted = formatted.substr(0, last + 1);
    }

    return formatted;
};

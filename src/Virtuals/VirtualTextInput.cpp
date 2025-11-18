#include "VirtualNode.hpp"
#include <geode.devtools/include/API.hpp>
#include <Geode/ui/TextInput.hpp>

constexpr std::array<char const*, 11> filters = {
	"Uint",
	"Int",
	"Float",
	"ID",
	"Name",
	"Any",
	"Hex",
	"Base64Normal",
	"Base64URL",
	"Alphanumeric",
	"Alphabetic"
};

class VirtualTextInput : public VirtualNode, RegisterDOM<VirtualTextInput, "Text Input"> {
	std::string m_placeholder = "Enter text...";

	std::string m_font = "bigFont.fnt";
	bool m_fontDirty;

	std::string m_label = "";

	CommonFilter m_commonFilter = CommonFilter::Any;
	std::string m_filter;
	bool m_customFilter = false;

	unsigned int m_maxLength = 0;
	bool m_isPassword = false;
	TextInputAlign m_align;

	std::string m_value = "";
	bool m_valueDirty = false;
 public:
 	VirtualTextInput() : VirtualNode() {
 		setContentWidth(200);
		m_tether = TextInput::create(200, m_placeholder.c_str(), m_font.c_str());
		setAnchorPoint({0.5, 0.5});
	}

	VirtualTextInput(VirtualTextInput& src) : VirtualNode(src) {
		m_placeholder = src.m_placeholder;
		m_font = src.m_font;
		m_label = src.m_label;
		m_filter = src.m_filter;
		m_maxLength = src.m_maxLength;
		m_isPassword = src.m_isPassword;
		m_align = src.m_align;
		m_value = src.m_value;

		replaceTether(TextInput::create(200, m_font.c_str()));
	}

	void settings() override {
		VirtualNode::settings();

		devtools::newLine();
		devtools::separator();
		devtools::newLine();

		m_valueDirty = devtools::property("Default Value", m_value);
		devtools::property("Placeholder", m_placeholder);
		m_fontDirty = devtools::property("Font", m_font);
		devtools::property("Label", m_label);

		devtools::property("Custom Filter", m_customFilter);
		devtools::nextItemWidth(220);
		if (m_customFilter) {
			devtools::sameLine();
			devtools::property("", m_filter);
		} else {
			devtools::sameLine();
			devtools::combo("", m_commonFilter, filters);
		}

		devtools::nextItemWidth(100);
		devtools::property("Max Length (0 = infinite)", m_maxLength);
		devtools::property("Password", m_isPassword);

		devtools::combo("Alignment", m_align, {"Center", "Left"});
	}

	matjson::Value exportJSON() override {
		auto obj = VirtualNode::exportJSON();

		auto setValIf = [&obj](char const* key, auto value, auto def) {
			if (value != def)
				obj[key] = value;
		};

		setValIf("placeholder", m_placeholder, "Enter text...");
		setValIf("font", m_font, "bigFont.fnt");
		setValIf("label", m_label, "");
		if (m_customFilter) {
			setValIf("filter", m_filter, "");
		} else {
			setValIf("filter", static_cast<int>(m_commonFilter), static_cast<int>(CommonFilter::Any));
		}
		setValIf("maxLength", m_maxLength, 0);
		setValIf("isPassword", m_isPassword, false);
		setValIf("align", static_cast<int>(m_align), static_cast<int>(TextInputAlign::Center));
		setValIf("value", m_value, "");

		return obj;
	}

	void importJSON(matjson::Value value) override {
		VirtualNode::importJSON(value);

		m_placeholder = value["placeholder"].asString().unwrapOr("Enter text...");
		m_font = value["font"].asString().unwrapOr("bigFont.fnt");
		m_label = value["label"].asString().unwrapOr("");

		if (value["filter"].isString()) {
			m_filter = value["filter"].asString().unwrapOr("");
			m_customFilter = true;
		} else {
			m_commonFilter = static_cast<CommonFilter>(value["filter"].asInt().unwrapOr(static_cast<int>(CommonFilter::Any)));
			m_customFilter = false;
		}

		m_maxLength = static_cast<unsigned int>(value["maxLength"].asInt().unwrapOr(0));
		m_isPassword = value["isPassword"].asBool().unwrapOr(false);
		m_align = static_cast<TextInputAlign>(value["align"].asInt().unwrapOr(static_cast<int>(TextInputAlign::Center)));
		m_value = value["value"].asString().unwrapOr("");
	}

	std::string emitCode(int indent = 0) override {
		std::string ind(indent, ' ');
		std::string out = fmt::format("{}Build<TextInput>::create({}f, \"{}\", \"{}\")\n", ind, fmtFloat(getContentWidth()), m_placeholder, m_font);
	
		auto emitIf = [&](auto field, auto def, std::string_view code) {
			if (field != def) {
				out += fmt::format("{}    .{}\n", ind, code);
			}
		};

		emitIf(m_label, "", fmt::format("label(\"{}\")", m_label));
		if (m_customFilter) {
			emitIf(m_filter, "", fmt::format("filter(\"{}\")", m_filter));
		} else {
			emitIf(m_commonFilter, CommonFilter::Any, fmt::format("commonFilter(CommonFilter::{})", filters[static_cast<int>(m_commonFilter)]));
		}
		emitIf(m_maxLength, 0, fmt::format("maxCharCount({})", m_maxLength));
		emitIf(m_isPassword, false, "passwordMode(true)");
		emitIf(m_align, TextInputAlign::Center, fmt::format("textAlign(TextInputAlign::{})", m_align == TextInputAlign::Center ? "Center" : "Left"));
		emitIf(m_value, "", fmt::format("string(\"{}\")", m_value));
		emitIf(getContentWidth(), 200.f, fmt::format("width({}f)", fmtFloat(getContentWidth())));

		auto json = exportJSON();
		json.erase("size");
		out += VirtualNode::emitAttributes(exportJSON(), indent + 4);

		return out;
	}

	void updateTether() override {
        if (m_fontDirty && FNTConfigLoadFile(m_font.c_str()))
            replaceTether(TextInput::create(getContentWidth(), m_placeholder.c_str(), m_font.c_str()));

		auto tether = reinterpret_cast<TextInput*>(m_tether.data());

		if (m_valueDirty || m_fontDirty)
			tether->setString(m_value.c_str());

		tether->setPlaceholder(m_placeholder.c_str());
		tether->setLabel(m_label.c_str());
		tether->setMaxCharCount(m_maxLength);
		tether->setPasswordMode(m_isPassword);
		tether->setTextAlign(m_align);

		if (static_cast<int>(m_commonFilter) == -1) {
			tether->setFilter(m_filter.c_str());
		} else {
			tether->setCommonFilter(m_commonFilter);
		}

		tether->setWidth(getContentWidth());
		setContentSize(tether->getContentSize());

		VirtualNode::updateTether();
	}

	VirtualNode* duplicate() override {
		return new VirtualTextInput(*this);
	}
};

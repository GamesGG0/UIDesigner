# UIDesigner
## An Interface Builder for Geometry Dash

<img src="images/logo.svg" alt="Logo"/>

This mod combined with [Geode DevTools](https://github.com/geode-sdk/devtools) provides an interface you can use to design and export UI menu designs, similar to Xcode's Interface Builder. Any generated UI can be turned into usable [UIBuilder](https://github.com/camila314/UIBuilder) c++ code.

<img src="images/image1.png" alt="Showcase"/>

## Built-in elements

- Node/Menu
- Label
- Sprite
- Row Node
- Column Node
- Color Box
- Markdown Text Area
- Scale9Sprite
- Sprite Button
- Scale9Sprite Button
- Text Button
- Text Input
- Toggle

## API

You can add your own support for UIDesigner by including the `VirtualNodeAPI.hpp` file like this:

```cpp
#include <camila314.uidesigner/include/VirtualNodeAPI.hpp>
#include <geode.devtools/include/API.hpp>

using namespace uidesigner;

class VirtualCustom : VirtualNode, RegisterDOM<"Custom Node", VirtualCustom> {
	int m_setting;
 public:
	VirtualCustom() : VirtualNode(), m_setting(5) {
		m_tether = CustomNode::create();
	}

	void updateTether() override {
		// if a setting changes, display that change on the tethered node
		static_cast<CustomNode*>(m_tether)->setSomeSetting(m_setting);
		VirtualNode::updateTether();
	}

	void settings() override {
		// ImGui settings here
		devtools::property("Setting Value", m_setting);
	}

	std::string emitCode(int indent = 0) override {
		// generate code that builds the node.
		// must use UIBuilder to cooperate with VirtualNode::emitAttributes
		std::string ind(indent, ' ');

		std::string out = ind + "Build<CustomNode>::create()\n";
		out += VirtualNode::emitAttributes(exportJSON(), indent + 4);

		return out;
	}

	matjson::Value exportJSON() override {
		auto obj = VirtualNode::exportJSON();

		obj["type"] = "Custom Node"; // must be the same as the registered name
		obj["mySetting"] = m_setting;

		return obj;
	}

	void importJSON(matjson::Value json) override {
		VirtualNode::importJSON(json);
		m_setting = json["mySetting"].asInt();
	}
};
```

#include "VirtualDOM.hpp"
#include <Virtuals/VirtualNode.hpp>
#include <geode.devtools/include/API.hpp>
#include <geode.devtools/include/API.hpp>
#include <Geode/utils/coro.hpp>
#include <Geode/utils/file.hpp>

/// Virtual DOM Manager

VirtualDOMManager* VirtualDOMManager::get() {
	static VirtualDOMManager instance;
	return &instance;
}

VirtualDOMManager::VirtualDOMManager() {
	devtools::waitForDevTools(+[] {
		devtools::registerNode<VirtualNode>(+[](VirtualNode* self) {
			auto manager = VirtualDOMManager::get();

			if (devtools::button((char const*)u8"\ue91e" " Code")) {
				auto out = self->emitCode();
				if (out.back() == '\n')
				    out.pop_back();
				clipboard::write(out + ";");
			}

			devtools::sameLine();

			if (devtools::button((char const*)u8"\ue965" " Delete")) {
				self->removeFromParent();
				return;
			}

			devtools::sameLine();

			auto options = file::FilePickOptions { .filters = {{
				.description = "JSON File", .files = { "*.json"}
			}}};

			if (devtools::button((char const*)u8"\ue92a" " Import")) {
				$async(manager, self, options) {
					auto json = co_await file::pick(file::PickMode::OpenFile, options);

					if (json.isOk()) {
						auto data = file::readJson(json.unwrap());
						if (data.isOk())
							self->CCNode::addChild(manager->createFromJSON(data.unwrap()));
					}
				};
			}

			devtools::sameLine();

			if (devtools::button((char const*)u8"\ue967" " Export")) {
				$async(manager, self, options) {
					auto file = co_await file::pick(file::PickMode::SaveFile, options);

					if (file.isOk()) {
						if (file::writeToJson(file.unwrap(), self->exportJSON()).isErr()) {
							log::warn("Failed to export to {}", file.unwrap());
						}
					}
				};
			}

			if (devtools::button((char const*)u8"\ue91e" " Clone")) {
				self->duplicate();
			}

			devtools::sameLine();

			if (devtools::button((char const*)u8"\ue94f" " Child")) {
				self->CCNode::addChild(manager->m_creators[manager->m_creatorNames[self->m_nodeSelection]]());
			}
			devtools::sameLine();
			devtools::nextItemWidth(120.0f); 
			devtools::combo("##customnode", self->m_nodeSelection, manager->m_creatorNames);

			devtools::label(fmt::format("Tether Address: {}", (void*)self->m_tether.data()).c_str());
			self->settings();
		});
	});
}

void VirtualDOMManager::initialize(CCLayer* layer) {
	auto vnode = new VirtualNode;
	vnode->setAnchorPoint({0.5, 0.5});
	vnode->setID("Root Node");

	layer->addChildAtPosition(vnode, Anchor::Center);
	layer->addChild(vnode->m_tether);
}

void VirtualDOMManager::registerType(std::string_view name, VirtualCreator ctor) {
	auto [it, inserted] = m_creators.insert({ name.data(), ctor });
	if (inserted) {
		m_creatorNames.push_back(it->first.c_str());
	}
}

VirtualNode* VirtualDOMManager::createFromJSON(matjson::Value obj) {
	auto type = obj["type"].asString().unwrapOr("Node");
	auto it = m_creators.find(type);

	if (it != m_creators.end()) {
		auto node = it->second();
		node->importJSON(obj);
		return node;
	} else {
		auto node = m_creators["Node"]();
		node->importJSON(obj);
		return node;
	}
}

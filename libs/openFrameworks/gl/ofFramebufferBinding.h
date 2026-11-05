#pragma once

#include <deque>

// Tracks which framebuffer ofFbo::begin()/end() should return to.
// startRender() pushes the window id and that entry stays until finishRender().
// A matching begin/end inside the frame pushes and pops above it. finishRender()
// warns when an ofFbo is still bound, then binds the window again so a later
// end() does not report that as an unbalanced pair.

enum class ofFramebufferBindWarning {
	None,
	NoMatchingBegin,
	LeftBound,
	BoundOntoSelf
};

template <class Id>
struct ofFramebufferBindStep {
	Id framebufferId{};
	bool bind = false;
	ofFramebufferBindWarning warning = ofFramebufferBindWarning::None;
};

template <class Id>
ofFramebufferBindStep<Id> ofFramebufferStartRender(std::deque<Id> & stack, Id & current, Id defaultId) {
	current = defaultId;
	stack.push_back(defaultId);
	return {defaultId, true, ofFramebufferBindWarning::None};
}

template <class Id>
ofFramebufferBindStep<Id> ofFramebufferFinishRender(std::deque<Id> & stack, Id & current, Id defaultId) {
	const ofFramebufferBindWarning warning = (stack.size() != 1 || current != defaultId)
		? ofFramebufferBindWarning::LeftBound
		: ofFramebufferBindWarning::None;
	stack.clear();
	if (current != defaultId) {
		current = defaultId;
		return {defaultId, true, warning};
	}
	return {defaultId, false, warning};
}

template <class Id>
ofFramebufferBindStep<Id> ofFramebufferPush(std::deque<Id> & stack, Id & current, Id fboId) {
	const ofFramebufferBindWarning warning = (current == fboId)
		? ofFramebufferBindWarning::BoundOntoSelf
		: ofFramebufferBindWarning::None;
	stack.push_back(current);
	current = fboId;
	return {fboId, true, warning};
}

template <class Id>
ofFramebufferBindStep<Id> ofFramebufferPop(std::deque<Id> & stack, Id & current, Id defaultId) {
	if (stack.empty()) {
		const ofFramebufferBindWarning warning = (current != defaultId)
			? ofFramebufferBindWarning::NoMatchingBegin
			: ofFramebufferBindWarning::None;
		current = defaultId;
		return {defaultId, true, warning};
	}
	if (stack.size() == 1 && current == stack.back()) {
		return {current, true, ofFramebufferBindWarning::NoMatchingBegin};
	}
	current = stack.back();
	stack.pop_back();
	return {current, true, ofFramebufferBindWarning::None};
}

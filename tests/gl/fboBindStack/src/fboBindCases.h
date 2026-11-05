#pragma once

#include "ofFramebufferBinding.h"
#include <deque>
#include <string>

// Runs the framebuffer stack the same way the GL renderers do, with no GL context.
// Returns the number of failed checks. details receives one line per failure.
inline int runFboBindCases(std::string & details) {
	using Id = std::uint32_t;
	const Id window = 0;
	const Id fboA = 7;
	const Id fboB = 8;
	int failed = 0;

	auto check = [&](bool ok, const char * name) {
		if (!ok) {
			details += name;
			details += "\n";
			++failed;
		}
	};

	{
		std::deque<Id> stack;
		Id current = window;
		const auto start = ofFramebufferStartRender(stack, current, window);
		const auto began = ofFramebufferPush(stack, current, fboA);
		const auto ended = ofFramebufferPop(stack, current, window);
		check(start.warning == ofFramebufferBindWarning::None && start.bind && start.framebufferId == window, "start binds the window");
		check(began.warning == ofFramebufferBindWarning::None && ended.warning == ofFramebufferBindWarning::None, "balanced end is quiet");
		check(ended.framebufferId == window && stack.size() == 1 && current == window, "balanced end returns to the window");
		const auto finish = ofFramebufferFinishRender(stack, current, window);
		check(finish.warning == ofFramebufferBindWarning::None && !finish.bind && stack.empty() && current == window, "balanced frame does not warn");
	}

	{
		std::deque<Id> stack;
		Id current = window;
		ofFramebufferStartRender(stack, current, window);
		ofFramebufferPush(stack, current, fboA);
		const Id left = current;
		const auto finish = ofFramebufferFinishRender(stack, current, window);
		const auto late = ofFramebufferPop(stack, current, window);
		check(finish.warning == ofFramebufferBindWarning::LeftBound && finish.bind && left == fboA, "missing end is reported when the frame ends");
		check(current == window && stack.empty(), "missing end rebinds the window");
		check(late.warning == ofFramebufferBindWarning::None && late.framebufferId == window, "end after the frame recovered stays quiet");
	}

	{
		std::deque<Id> stack;
		Id current = window;
		ofFramebufferStartRender(stack, current, window);
		ofFramebufferPush(stack, current, fboA);
		ofFramebufferPop(stack, current, window);
		const auto extra = ofFramebufferPop(stack, current, window);
		check(extra.warning == ofFramebufferBindWarning::NoMatchingBegin && stack.size() == 1 && current == window, "extra end warns and keeps the window entry");
		const auto finish = ofFramebufferFinishRender(stack, current, window);
		check(finish.warning == ofFramebufferBindWarning::None, "extra end does not warn again at the end of the frame");
	}

	{
		std::deque<Id> stack;
		Id current = window;
		ofFramebufferFinishRender(stack, current, window);
		const auto began = ofFramebufferPush(stack, current, fboA);
		const auto ended = ofFramebufferPop(stack, current, window);
		check(began.warning == ofFramebufferBindWarning::None && ended.warning == ofFramebufferBindWarning::None, "begin/end outside a frame is quiet");
		check(stack.empty() && current == window, "begin/end outside a frame restores the window");
	}

	{
		std::deque<Id> stack;
		Id current = window;
		ofFramebufferStartRender(stack, current, window);
		ofFramebufferPush(stack, current, fboA);
		const auto nested = ofFramebufferPush(stack, current, fboA);
		const auto inner = ofFramebufferPop(stack, current, window);
		const auto outer = ofFramebufferPop(stack, current, window);
		const auto finish = ofFramebufferFinishRender(stack, current, window);
		check(nested.warning == ofFramebufferBindWarning::BoundOntoSelf && inner.framebufferId == fboA && inner.warning == ofFramebufferBindWarning::None, "binding an fbo onto itself still has a matching end");
		check(outer.framebufferId == window && finish.warning == ofFramebufferBindWarning::None, "self-bind end returns to the window");
	}

	{
		std::deque<Id> stack;
		Id current = window;
		ofFramebufferStartRender(stack, current, window);
		ofFramebufferPush(stack, current, fboA);
		ofFramebufferPush(stack, current, fboB);
		const auto inner = ofFramebufferPop(stack, current, window);
		const auto outer = ofFramebufferPop(stack, current, window);
		check(inner.framebufferId == fboA && outer.framebufferId == window, "nested fbos pop in order");
	}

	{
		std::deque<Id> stack;
		Id current = fboA;
		const auto late = ofFramebufferPop(stack, current, window);
		check(late.warning == ofFramebufferBindWarning::NoMatchingBegin && current == window, "end while another fbo is current and the stack is empty warns");
	}

	return failed;
}

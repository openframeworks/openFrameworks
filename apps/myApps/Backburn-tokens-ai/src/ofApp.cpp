// AI Token Backburn - what your AI coding tools have used, and what's left.
// Copyright (c) 2026 Daniel Rosser. MIT License.

#include "ofApp.h"

#include "PlatformWindow.h"
#include "ProviderApi.h"
#include "Strings.h"
#include "Secrets.h"
#include "../release/include/Obfuscate.h"   // OBFSTR: external links never sit in the binary in plaintext

#include "ofxImAnim.h"

#include <GLFW/glfw3.h>

#include <algorithm>
#include <cstdarg>
#include <cmath>
#include <chrono>
#include <ctime>
#include <fstream>
#include <random>
#include <unordered_map>

using namespace backburn;

namespace {

// two looks: Kingmaker's near-black blues and cyan light, and neon - cyan type on black with magenta for what
// matters. Amber and red stay for limits running out. Set by applyTheme.
ImVec4 GREEN(0.30f, 0.82f, 0.52f, 1.0f);
const ImVec4 AMBER(0.95f, 0.72f, 0.20f, 1.0f);
const ImVec4 RED(0.88f, 0.11f, 0.11f, 1.0f);
ImVec4 GREY, ACCENT, STEEL, HOVER, DIM, BRIGHT;
const ImVec4 SAVE(0.13f, 0.62f, 0.38f, 1);
const ImVec4 DANGER(0.55f, 0.10f, 0.10f, 1);
bool neon = false;
float alertLevel = 0;      // 0 calm blue, 1 red alert; eased in draw()
ImVec4 RAIN(0.30f, 0.80f, 0.74f, 1);   // the drifting digits

// each theme's faces; headingFont and labelFont point at the current theme's
ImFont * headingFont = nullptr;
ImFont * labelFont = nullptr;
struct Faces {
	ImFont * body = nullptr;   // null: the default font
	ImFont * heading = nullptr;
	ImFont * label = nullptr;
};
Faces classicFaces, neonFaces;
ImFont * rainFont = nullptr;

bool animate = true;

ImGuiID key(const std::string & name) { return ImHashStr(name.c_str()); }

float tween(ImGuiID id, float target, float seconds, int ease = iam_ease_out_cubic, float from = 0.0f) {
	if (!animate) return target;
	return iam_tween_float(id, 0, target, seconds, iam_ease_preset(ease), iam_policy_crossfade, ImGui::GetIO().DeltaTime, from);
}

ImVec4 tweenColour(ImGuiID id, const ImVec4 & target, float seconds) {
	if (!animate) return target;
	return iam_tween_color(id, 0, target, seconds, iam_ease_preset(iam_ease_out_cubic), iam_policy_crossfade, iam_col_oklab,
	                       ImGui::GetIO().DeltaTime, target);
}

float pulse(ImGuiID id, float hz) {
	if (!animate) return 1.0f;
	return 0.5f + iam_oscillate(id, 0.5f, hz, iam_wave_sine, 0.0f, ImGui::GetIO().DeltaTime);
}

std::string upper(const std::string & s) { return ofToUpper(s); }

// a section header; translated (lang/<code>.json), so a heading reads in the interface's language
void section(const char * text) {
	ImGui::PushFont(headingFont, ImGui::GetFontSize() * 1.2f);
	ImGui::TextColored(neon ? BRIGHT : ACCENT, "%s", upper(i18n::tr(text)).c_str());
	if (neon) {
		ImGui::SameLine(0, 10);
		ImGui::TextColored(ACCENT, "%s", "\u25B8");
	}
	ImGui::PopFont();
}

// Hints and descriptions (Settings > General > Advanced): off, the explanations go and only the numbers stay
bool hintsShown = false;

// grey explanation under a setting
void hint(const std::string & text) {
	if (!hintsShown) return;
	ImGui::PushTextWrapPos(0);
	ImGui::TextColored(DIM, "%s", text.c_str());
	ImGui::PopTextWrapPos();
}

// a tooltip that explains, rather than shows a number: only with hints on
IM_FMTARGS(1) void explain(const char * format, ...) {
	if (!hintsShown) return;
	va_list args;
	va_start(args, format);
	ImGui::SetTooltipV(format, args);
	va_end(args);
}

// A major button. Its words are translated; its id stays the English (or what follows ###), so a button keeps its
// state whatever the language.
bool button(const char * label, const ImVec2 & size = ImVec2(0, 0), const ImVec4 * colour = nullptr) {
	static std::unordered_map<ImGuiID, bool> hovered;
	const std::string full = label;
	const size_t hash = full.find("###");
	const std::string words = full.substr(0, hash), stable = hash == std::string::npos ? full : full.substr(hash + 3);
	std::string text = upper(i18n::tr(words)) + "###" + stable;
	ImGuiID id = ImGui::GetID(text.c_str());
	ImVec4 c = tweenColour(id, hovered[id] ? HOVER : colour ? *colour : STEEL, 0.18f);
	ImGui::PushStyleColor(ImGuiCol_Button, c);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, c);
	ImGui::PushFont(labelFont, 0.0f);
	bool pressed = ImGui::Button(text.c_str(), size);
	ImGui::PopFont();
	ImGui::PopStyleColor(2);
	hovered[id] = ImGui::IsItemHovered();
	return pressed;
}

// a panel's own fold button, at its top-right corner: - folds it, + (drawn in the folded strip) reopens it.
// Drawn first inside the panel, so it sits on the panel's own corner — each main section folds on its own.
bool foldCorner(const char * id, bool folded) {
	const float x = ImGui::GetWindowContentRegionMax().x - 34.0f;
	if (ImGui::GetCursorPosX() < x) ImGui::SameLine(x);
	else ImGui::SetCursorPosX(x);
	const bool pressed = button(folded ? ("+###fold" + std::string(id)).c_str() : ("-###fold" + std::string(id)).c_str(), ImVec2(34, 0));
	if (ImGui::IsItemHovered()) explain(folded ? "Open this panel again" : "Fold this panel away");
	return pressed;
}

void glowText(const std::string & text, float size) {
	ImGui::PushFont(headingFont, size);
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList * dl = ImGui::GetWindowDrawList();
	float strength = 0.08f + 0.04f * pulse(key("title glow"), 0.2f);
	ImU32 glow = ImGui::GetColorU32(ImVec4(ACCENT.x, ACCENT.y, ACCENT.z, strength));
	for (float r : { 3.0f, 2.0f, 1.0f })
		for (float dx : { -r, 0.0f, r })
			for (float dy : { -r, 0.0f, r })
				if (dx != 0 || dy != 0) dl->AddText(ImVec2(p.x + dx, p.y + dy), glow, text.c_str());
	ImGui::TextColored(BRIGHT, "%s", text.c_str());
	ImGui::PopFont();
}

// The title burning: each letter's colour moves through embers, orange and yellow with noise, an orange glow
// breathes behind it, and sparks drift up off it.
void fireText(const std::string & text, float size) {
	ImGui::PushFont(headingFont, size);
	const ImVec2 start = ImGui::GetCursorScreenPos();
	const ImVec2 extent = ImGui::CalcTextSize(text.c_str());
	ImDrawList * dl = ImGui::GetWindowDrawList();
	const float t = animate ? float(ImGui::GetTime()) : 0;
	auto fire = [](float heat, float alpha) {
		// 0: deep red, 0.5: orange, 1: yellow-white
		heat = ofClamp(heat, 0, 1);
		ImVec4 c = heat < 0.5f ? ImVec4(0.75f + heat * 0.5f, 0.10f + heat * 0.8f, 0.02f, alpha)
		                       : ImVec4(1.0f, 0.50f + (heat - 0.5f) * 0.9f, 0.05f + (heat - 0.5f) * 1.0f, alpha);
		return ImGui::GetColorU32(c);
	};
	// glow: soft orange halos, flickering
	for (float r : { 6.0f, 4.0f, 2.0f }) {
		float flicker = 0.6f + 0.4f * ofNoise(t * 3.0f, r);
		for (float dx : { -r, 0.0f, r })
			for (float dy : { -r, 0.0f, r })
				if (dx != 0 || dy != 0) dl->AddText(ImVec2(start.x + dx, start.y + dy), fire(0.35f, 0.045f * flicker), text.c_str());
	}
	// letters, each its own flame
	ImVec2 pos = start;
	for (size_t i = 0; i < text.size(); i++) {
		const std::string ch(1, text[i]);
		float heat = ofNoise(i * 0.45f, t * 2.2f) * 0.8f + 0.25f;
		// licks upward: a lighter copy a pixel or two above
		float lift = 1.0f + 2.0f * ofNoise(i * 0.7f, t * 4.0f);
		dl->AddText(ImVec2(pos.x, pos.y - lift), fire(heat + 0.25f, 0.45f), ch.c_str());
		dl->AddText(pos, fire(heat, 1.0f), ch.c_str());
		pos.x += ImGui::CalcTextSize(ch.c_str()).x;
	}
	// sparks: each rises from a place along the word, fades, and starts again
	if (animate)
		for (int i = 0; i < 14; i++) {
			float cycle = 1.6f + (i % 5) * 0.35f;
			float phase = std::fmod(t + i * 0.37f, cycle) / cycle;
			float x = start.x + extent.x * std::fmod(i * 0.618f, 1.0f) + 6 * std::sin(t * 2 + i);
			float y = start.y + extent.y * 0.6f - phase * extent.y * 1.4f;
			dl->AddCircleFilled(ImVec2(x, y), 1.4f * (1 - phase) + 0.4f, fire(0.9f - phase * 0.6f, (1 - phase) * 0.9f));
		}
	ImGui::Dummy(extent);
	ImGui::PopFont();
}

void hudFrame() {
	ImVec2 a = ImGui::GetItemRectMin(), b = ImGui::GetItemRectMax();
	ImDrawList * dl = ImGui::GetWindowDrawList();
	ImU32 c = ImGui::GetColorU32(ACCENT);
	const float l = 16, t = 2;
	dl->AddLine(a, ImVec2(a.x + l, a.y), c, t); dl->AddLine(a, ImVec2(a.x, a.y + l), c, t);
	dl->AddLine(ImVec2(b.x, a.y), ImVec2(b.x - l, a.y), c, t); dl->AddLine(ImVec2(b.x, a.y), ImVec2(b.x, a.y + l), c, t);
	dl->AddLine(ImVec2(a.x, b.y), ImVec2(a.x + l, b.y), c, t); dl->AddLine(ImVec2(a.x, b.y), ImVec2(a.x, b.y - l), c, t);
	dl->AddLine(b, ImVec2(b.x - l, b.y), c, t); dl->AddLine(b, ImVec2(b.x, b.y - l), c, t);
}

void tableHeaders() {
	ImGui::PushFont(labelFont, 0.0f);
	ImGui::TableHeadersRow();
	ImGui::PopFont();
}

bool contains(const std::string & haystack, const char * needle) {
	return !*needle || ofIsStringInString(ofToLower(haystack), ofToLower(needle));
}

bool sameQuery(const Query & a, const Query & b) {
	return a.name == b.name && a.providers == b.providers && a.project == b.project && a.projectExact == b.projectExact
	       && a.model == b.model && a.lastDays == b.lastDays && a.groupBy == b.groupBy && a.format == b.format
	       && a.autoExport == b.autoExport;
}

ImVec4 limitColour(float percent, int warnPercent) {
	if (percent < 0) return GREY;
	if (percent >= 100) return RED;
	if (percent >= warnPercent) return AMBER;
	return GREEN;
}

// a limit as a bar that fills to its percent; the colour turns amber then red as it runs out
void meter(const std::string & id, const Limit & l, int warnPercent, float height = 10) {
	// a window that has reset since it was last seen counts as empty; its old number is kept in the label
	const bool past = l.resetsAt && l.resetsAt < now();
	float pct = l.percent < 0 || past ? 0 : std::min(100.0f, l.percent);
	float shown = tween(key("meter " + id), pct, 0.9f, iam_ease_out_expo);
	ImVec4 c = tweenColour(key("meter colour " + id), past ? GREY : limitColour(l.percent, warnPercent), 0.5f);
	ImGui::PushFont(labelFont, 0.0f);
	ImGui::TextColored(BRIGHT, "%s", upper(l.name).c_str());
	ImGui::PopFont();
	std::string right = l.percent < 0 ? "?" : past ? "was " + ofToString(int(l.percent)) + "%"
	                  : (l.estimate ? "\u2248" : "") + ofToString(int(std::round(shown))) + "%";
	ImGui::SameLine(ImGui::GetContentRegionAvail().x + ImGui::GetCursorPosX() - ImGui::CalcTextSize(right.c_str()).x
	                - ImGui::GetStyle().ItemSpacing.x - (ImGui::GetCursorPosX() - ImGui::GetCursorStartPos().x));
	ImGui::TextColored(c, "%s", right.c_str());
	ImVec2 p = ImGui::GetCursorScreenPos();
	float w = ImGui::GetContentRegionAvail().x;
	ImDrawList * dl = ImGui::GetWindowDrawList();
	dl->AddRectFilled(p, ImVec2(p.x + w, p.y + height), ImGui::GetColorU32(ImVec4(0.06f, 0.10f, 0.13f, 1)));
	ImVec4 fill = c;
	if (l.active && animate) fill.w = 0.75f + 0.25f * pulse(key("meter pulse " + id), 0.6f);
	dl->AddRectFilled(p, ImVec2(p.x + w * shown / 100.0f, p.y + height), ImGui::GetColorU32(fill));
	ImGui::Dummy(ImVec2(w, height));
	if (l.resetsAt) ImGui::TextColored(DIM, "%s", ((past ? "reset " : "resets ") + relativeTime(l.resetsAt)).c_str());
	else if (l.estimate) ImGui::TextColored(DIM, "%s", "opens with your next message");
	if (l.estimate && ImGui::IsItemHovered())
		explain("Estimated: %.0f%% when last saved, plus your use since.\nRun /usage in Claude Code for the real number.", l.measured);
}

void card(const std::string & caption, const std::string & value, const std::string & detail = "") {
	ImGui::BeginGroup();
	ImGui::PushFont(headingFont, ImGui::GetFontSize() * 2.4f);
	ImGui::TextColored(ACCENT, "%s", value.c_str());
	ImGui::PopFont();
	ImGui::PushFont(labelFont, 0.0f);
	ImGui::TextColored(BRIGHT, "%s", upper(caption).c_str());
	ImGui::PopFont();
	if (!detail.empty()) ImGui::TextColored(DIM, "%s", detail.c_str());
	ImGui::EndGroup();
}

// a number that counts up to its value
enum class Shown { Money, Tokens, Percent };
std::string counted(const std::string & id, double value, Shown as) {
	if (value < 0) return "-";
	double v = tween(key("count " + id), float(value), 0.8f, iam_ease_out_expo);
	return as == Shown::Money ? money(v) : as == Shown::Percent ? percent(v) : tokens(int64_t(v));
}

// a provider's use as it's shown: % of the weekly limit on a subscription, else dollars
std::string countedAmount(const std::string & id, const Provider & p, const Totals & t) {
	return p.showsPercent() ? counted(id + " pct", p.percentOf(t), Shown::Percent) : counted(id, t.cost, Shown::Money);
}

void address(const char * name, const char * caption, std::string value) {
	static std::unordered_map<std::string, double> copiedAt;
	ImGui::PushID(name);
	if (*caption) ImGui::TextColored(DIM, "%s", caption);
	const std::string id = std::string(name) + " " + value;
	bool copied = copiedAt.count(id) && ImGui::GetTime() - copiedAt[id] < 1.5;
	ImGui::PushFont(labelFont, 0.0f);
	float copyWidth = std::max(ImGui::CalcTextSize("COPY").x, ImGui::CalcTextSize("COPIED").x) + ImGui::GetStyle().FramePadding.x * 2;
	ImGui::PopFont();
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - copyWidth - ImGui::GetStyle().ItemSpacing.x);
	ImGui::InputText("##address", value.data(), value.size() + 1, ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
	ImGui::SameLine();
	if (button((std::string(copied ? "Copied" : "Copy") + "###copy").c_str(), ImVec2(copyWidth, 0), copied ? &SAVE : nullptr)) {
		ImGui::SetClipboardText(value.c_str());
		copiedAt[id] = ImGui::GetTime();
	}
	ImGui::PopID();
}

void findingRow(const Finding & f) {
	ImVec4 c = f.level == Finding::Level::Problem ? RED : f.level == Finding::Level::Warning ? AMBER : ACCENT;
	ImVec2 p = ImGui::GetCursorScreenPos();
	float h = ImGui::GetTextLineHeight();
	ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + 3, p.y + h), ImGui::GetColorU32(c));
	ImGui::Indent(12);
	ImGui::PushTextWrapPos(0);
	ImGui::TextColored(BRIGHT, "%s", f.title.c_str());
	if (!f.detail.empty() && hintsShown) ImGui::TextColored(DIM, "%s", f.detail.c_str());
	ImGui::PopTextWrapPos();
	ImGui::Unindent(12);
	ImGui::Spacing();
}

// the last days as bars: cost when there is one, else output tokens. Days with no use are gaps. Returns the
// day clicked, if one was; selected is drawn lit.
std::string dayChart(const std::string & id, const std::vector<DayUsage> & days, int count, const std::string & selected = "",
                     const Provider * as = nullptr, float height = 110) {
	if (days.empty()) {
		ImGui::TextColored(DIM, "%s", "No usage on record.");
		return "";
	}
	const bool byPercent = as && as->showsPercent();
	bool byCost = byPercent;
	for (const auto & d : days) byCost |= d.cost > 0;
	auto value = [&](const DayUsage & d) { return byPercent ? as->percentOf(d) : byCost ? d.cost : double(d.output); };
	auto shown = [&](double v) { return byPercent ? percent(v) : byCost ? money(v) : tokens(int64_t(v)); };
	// the last count calendar days, ending today
	std::vector<std::string> slots;
	for (int i = count - 1; i >= 0; i--) slots.push_back(localDay(now() - int64_t(i) * 86400));
	std::map<std::string, const DayUsage *> byDay;
	for (const auto & d : days) byDay[d.day] = &d;
	double top = 0;
	for (const auto & s : slots)
		if (byDay.count(s)) top = std::max(top, value(*byDay[s]));
	ImVec2 o = ImGui::GetCursorScreenPos();
	float w = ImGui::GetContentRegionAvail().x;
	float slot = w / float(count);
	ImDrawList * dl = ImGui::GetWindowDrawList();
	const DayUsage * hovered = nullptr;
	std::string hoveredDay;
	ImVec2 mouse = ImGui::GetIO().MousePos;
	for (int i = 0; i < count; i++) {
		float x = o.x + i * slot;
		bool over = mouse.x >= x && mouse.x < x + slot && mouse.y >= o.y && mouse.y < o.y + height && ImGui::IsWindowHovered();
		if (over) hoveredDay = slots[i];
		auto it = byDay.find(slots[i]);
		if (it == byDay.end() || top <= 0) continue;
		if (over) hovered = it->second;
		float v = float(value(*it->second) / top);
		v = tween(key("bar " + id + slots[i]), v, 0.6f, iam_ease_out_cubic);
		ImVec4 colour = over || slots[i] == selected ? ACCENT : STEEL;
		dl->AddRectFilled(ImVec2(x + 2, o.y + height * (1 - v)), ImVec2(x + slot - 2, o.y + height), ImGui::GetColorU32(colour));
		if (slots[i] == selected) dl->AddRect(ImVec2(x + 1, o.y), ImVec2(x + slot - 1, o.y + height), ImGui::GetColorU32(ACCENT));
	}
	dl->AddLine(ImVec2(o.x, o.y + height), ImVec2(o.x + w, o.y + height), ImGui::GetColorU32(ImGuiCol_Border));
	ImGui::Dummy(ImVec2(w, height));
	std::string highest = shown(top);
	ImGui::TextColored(DIM, "%s, last %d days to today; highest day %s.%s",
	                   byPercent ? "Share of the weekly limit" : byCost ? "Cost" : "Output tokens", count, highest.c_str(),
	                   hintsShown ? " Click a day for what ran." : "");
	if (hovered)
		ImGui::SetTooltip("%s\n%s\n%s output tokens\n%lld calls\nclick: the tasks that day", hovered->day.c_str(), shown(value(*hovered)).c_str(),
		                  tokens(hovered->output).c_str(), static_cast<long long>(hovered->calls));
	else if (!hoveredDay.empty())
		ImGui::SetTooltip("%s\nno use", hoveredDay.c_str());
	if (hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) return hovered->day;
	return "";
}

void totalsTable(const char * id, const std::vector<std::pair<std::string, Totals>> & rows, const char * first, const Provider & p) {
	if (!ImGui::BeginTable(id, 8, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp)) return;
	for (const char * h : { first, "Calls", "Input", "Output", "Thinking", "Cache read", "Cache write", amountName(p) }) ImGui::TableSetupColumn(h);
	tableHeaders();
	for (const auto & [name, t] : rows) {
		ImGui::TableNextRow();
		ImGui::TableNextColumn(); ImGui::TextUnformatted(name.empty() ? "-" : name.c_str());
		ImGui::TableNextColumn(); ImGui::Text("%lld", static_cast<long long>(t.calls));
		ImGui::TableNextColumn(); ImGui::TextUnformatted(tokens(t.input).c_str());
		ImGui::TableNextColumn(); ImGui::TextUnformatted(tokens(t.output).c_str());
		ImGui::TableNextColumn(); ImGui::TextUnformatted(tokens(t.reasoning).c_str());
		ImGui::TableNextColumn(); ImGui::TextUnformatted(tokens(t.cacheRead).c_str());
		ImGui::TableNextColumn(); ImGui::TextUnformatted(tokens(t.cacheWrite5m + t.cacheWrite1h).c_str());
		if (ImGui::IsItemHovered() && t.cacheWrite1h)
			ImGui::SetTooltip("5-minute: %s\n1-hour: %s", tokens(t.cacheWrite5m).c_str(), tokens(t.cacheWrite1h).c_str());
		ImGui::TableNextColumn(); ImGui::TextColored(ACCENT, "%s", amount(p, t).c_str());
		if (ImGui::IsItemHovered() && p.showsPercent() && t.cost >= 0) ImGui::SetTooltip("%s at API prices", money(t.cost).c_str());
	}
	ImGui::EndTable();
}

// a status light per provider in the header: green, amber or red by its tightest limit; grey if not found
void statusLight(const Provider & p, int warnPercent, float baseline, int index) {
	float worst = -1;
	for (const auto & l : p.limits)
		if (!l.resetsAt || l.resetsAt > now()) worst = std::max(worst, l.percent);
	bool problem = false;
	for (const auto & f : p.findings) problem |= f.level == Finding::Level::Problem;
	ImVec4 colour = !p.found ? GREY : problem ? RED : worst < 0 ? ACCENT : limitColour(worst, warnPercent);
	ImGui::PushFont(labelFont, 0.0f);
	const float h = ImGui::GetTextLineHeight();
	ImGui::SetCursorPosY(baseline);
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 centre(pos.x + h * 0.4f, pos.y + h * 0.5f);
	ImVec4 c = tweenColour(key("dot " + p.id), colour, 0.4f);
	ImDrawList * dl = ImGui::GetWindowDrawList();
	if (p.found && animate) {
		float phase = 0.5f + iam_oscillate(key("dot ping " + p.id), 0.5f, 0.4f, iam_wave_sawtooth, index * 0.18f, ImGui::GetIO().DeltaTime);
		ImVec4 ring = c;
		ring.w = 0.55f * (1.0f - phase);
		dl->AddCircle(centre, h * 0.3f * (1.0f + phase * 1.3f), ImGui::GetColorU32(ring), 0, 1.5f);
	}
	dl->AddCircleFilled(centre, h * 0.3f, ImGui::GetColorU32(c));
	ImGui::Dummy(ImVec2(h * 0.8f, h));
	ImGui::SameLine();
	ImGui::SetCursorPosY(baseline);
	std::string label = upper(p.name);
	if (worst >= 0) label += " " + ofToString(int(worst)) + "%";
	ImGui::TextUnformatted(label.c_str());
	ImGui::PopFont();
}

// a combo over a list of strings; true when the choice changed
bool choose(const char * label, std::string & value, const std::vector<std::string> & options, float width = 160) {
	bool changed = false;
	ImGui::SetNextItemWidth(width);
	if (ImGui::BeginCombo(label, value.c_str())) {
		for (const auto & o : options)
			if (ImGui::Selectable(o.c_str(), o == value)) {
				value = o;
				changed = true;
			}
		ImGui::EndCombo();
	}
	return changed;
}

void reveal(const std::string & path) {
	// NSWorkspace, which works in the sandbox too
	access::reveal(path);
}

// The glowing themes are one palette each; red alert is another, and the screen blends from the theme's towards
// it as things go bad. line is the colour of borders, rules and table lines; background clears the window.
struct Palette {
	ImVec4 green, grey, accent, steel, hover, dim, bright, rain, text, child, popup, line, frame, frameHover, frameActive,
		buttonActive, tab, tabHover, header, headerHover, tableHeader, scroll;
	ofColor background;
};

// aqua type on dark teal, brighter aqua for numbers and what's selected
const Palette NEON = {
	{ 0.35f, 1.0f, 0.70f, 1 }, { 0.16f, 0.32f, 0.34f, 1 }, { 0.62f, 1.0f, 1.0f, 1 }, { 0.0f, 0.24f, 0.29f, 0.70f },
	{ 0.0f, 0.46f, 0.54f, 0.95f }, { 0.24f, 0.58f, 0.62f, 1 }, { 0.80f, 1.0f, 1.0f, 1 }, { 0.30f, 0.80f, 0.74f, 1 },
	{ 0.42f, 0.90f, 0.93f, 1 }, { 0.0f, 0.035f, 0.045f, 0.80f }, { 0.0f, 0.05f, 0.06f, 0.96f }, { 0.30f, 0.85f, 0.90f, 1 },
	{ 0.0f, 0.08f, 0.10f, 0.75f }, { 0.0f, 0.16f, 0.19f, 0.85f }, { 0.0f, 0.24f, 0.28f, 0.9f }, { 0.20f, 0.70f, 0.78f, 1 },
	{ 0.0f, 0.05f, 0.06f, 0.6f }, { 0.0f, 0.38f, 0.45f, 0.85f }, { 0.0f, 0.32f, 0.38f, 0.45f }, { 0.0f, 0.46f, 0.54f, 0.6f },
	{ 0.0f, 0.13f, 0.16f, 0.85f }, { 0.0f, 0.46f, 0.54f, 0.7f }, ofColor(0, 7, 9) };
// phosphor green on black, the brightest green almost white
const Palette MATRIX = {
	{ 0.55f, 1.0f, 0.55f, 1 }, { 0.12f, 0.30f, 0.14f, 1 }, { 0.80f, 1.0f, 0.80f, 1 }, { 0.0f, 0.24f, 0.06f, 0.70f },
	{ 0.0f, 0.46f, 0.12f, 0.95f }, { 0.18f, 0.60f, 0.26f, 1 }, { 0.85f, 1.0f, 0.85f, 1 }, { 0.15f, 0.85f, 0.30f, 1 },
	{ 0.30f, 0.95f, 0.42f, 1 }, { 0.0f, 0.035f, 0.01f, 0.80f }, { 0.0f, 0.05f, 0.015f, 0.96f }, { 0.25f, 0.90f, 0.40f, 1 },
	{ 0.0f, 0.08f, 0.02f, 0.75f }, { 0.0f, 0.16f, 0.04f, 0.85f }, { 0.0f, 0.24f, 0.06f, 0.9f }, { 0.20f, 0.75f, 0.30f, 1 },
	{ 0.0f, 0.05f, 0.012f, 0.6f }, { 0.0f, 0.38f, 0.10f, 0.85f }, { 0.0f, 0.32f, 0.08f, 0.45f }, { 0.0f, 0.46f, 0.12f, 0.6f },
	{ 0.0f, 0.13f, 0.03f, 0.85f }, { 0.0f, 0.46f, 0.12f, 0.7f }, ofColor(0, 6, 1) };
// an amber terminal: warm yellow on brown-black
const Palette AMBER_PALETTE = {
	{ 0.60f, 1.0f, 0.50f, 1 }, { 0.32f, 0.26f, 0.12f, 1 }, { 1.0f, 0.92f, 0.55f, 1 }, { 0.30f, 0.20f, 0.0f, 0.70f },
	{ 0.56f, 0.38f, 0.0f, 0.95f }, { 0.68f, 0.52f, 0.18f, 1 }, { 1.0f, 0.95f, 0.75f, 1 }, { 0.90f, 0.66f, 0.12f, 1 },
	{ 1.0f, 0.76f, 0.22f, 1 }, { 0.04f, 0.025f, 0.0f, 0.66f }, { 0.06f, 0.04f, 0.0f, 0.96f }, { 1.0f, 0.72f, 0.20f, 1 },
	{ 0.10f, 0.065f, 0.0f, 0.75f }, { 0.19f, 0.12f, 0.0f, 0.85f }, { 0.28f, 0.18f, 0.0f, 0.9f }, { 0.85f, 0.60f, 0.10f, 1 },
	{ 0.06f, 0.04f, 0.0f, 0.6f }, { 0.45f, 0.30f, 0.0f, 0.85f }, { 0.38f, 0.25f, 0.0f, 0.45f }, { 0.56f, 0.38f, 0.0f, 0.6f },
	{ 0.16f, 0.10f, 0.0f, 0.85f }, { 0.56f, 0.38f, 0.0f, 0.7f }, ofColor(8, 5, 0) };
// red alert: salmon type on maroon, electric blue for what matters
const Palette ALERT = {
	{ 0.35f, 1.0f, 0.70f, 1 }, { 0.36f, 0.16f, 0.16f, 1 }, { 0.42f, 0.52f, 1.0f, 1 }, { 0.30f, 0.04f, 0.05f, 0.70f },
	{ 0.55f, 0.10f, 0.12f, 0.95f }, { 0.72f, 0.36f, 0.30f, 1 }, { 1.0f, 0.82f, 0.72f, 1 }, { 0.90f, 0.32f, 0.30f, 1 },
	{ 1.0f, 0.50f, 0.38f, 1 }, { 0.07f, 0.0f, 0.012f, 0.80f }, { 0.10f, 0.01f, 0.02f, 0.96f }, { 1.0f, 0.40f, 0.30f, 1 },
	{ 0.13f, 0.01f, 0.02f, 0.75f }, { 0.22f, 0.02f, 0.04f, 0.85f }, { 0.32f, 0.03f, 0.05f, 0.9f }, { 0.80f, 0.25f, 0.20f, 1 },
	{ 0.07f, 0.0f, 0.01f, 0.6f }, { 0.45f, 0.08f, 0.10f, 0.85f }, { 0.40f, 0.05f, 0.07f, 0.45f }, { 0.55f, 0.10f, 0.12f, 0.6f },
	{ 0.17f, 0.01f, 0.03f, 0.85f }, { 0.55f, 0.10f, 0.12f, 0.7f }, ofColor(16, 1, 4) };

const Palette * calmPalette = &NEON;   // the glowing theme in use
// what drifts behind the panels: digit blocks (neon), code falling in streams (matrix), smoke and embers (amber)
enum class Backdrop { Drift, Streams, Smoke };
Backdrop backdrop = Backdrop::Drift;
ofColor clearColour;

// The current glowing theme at a level of alarm: 0 is the theme, 1 is red alert, in between blends.
void neonColours(float red) {
	const Palette & a = *calmPalette;
	const Palette & b = ALERT;
	auto mix = [red](const ImVec4 & calm, const ImVec4 & alarm) {
		return ImVec4(calm.x + (alarm.x - calm.x) * red, calm.y + (alarm.y - calm.y) * red, calm.z + (alarm.z - calm.z) * red,
		              calm.w + (alarm.w - calm.w) * red);
	};
	GREEN = mix(a.green, b.green);
	GREY = mix(a.grey, b.grey);
	ACCENT = mix(a.accent, b.accent);
	STEEL = mix(a.steel, b.steel);
	HOVER = mix(a.hover, b.hover);
	DIM = mix(a.dim, b.dim);
	BRIGHT = mix(a.bright, b.bright);
	RAIN = mix(a.rain, b.rain);
	clearColour = a.background.getLerped(b.background, red);
	ImVec4 * c = ImGui::GetStyle().Colors;
	c[ImGuiCol_Text] = mix(a.text, b.text);
	c[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 0);
	c[ImGuiCol_ChildBg] = mix(a.child, b.child);
	c[ImGuiCol_PopupBg] = mix(a.popup, b.popup);
	const ImVec4 line = mix(a.line, b.line);
	c[ImGuiCol_Border] = ImVec4(line.x, line.y, line.z, 0.45f);
	c[ImGuiCol_Separator] = ImVec4(line.x, line.y, line.z, 0.35f);
	c[ImGuiCol_TableBorderStrong] = ImVec4(line.x, line.y, line.z, 0.35f);
	c[ImGuiCol_TableBorderLight] = ImVec4(line.x, line.y, line.z, 0.15f);
	c[ImGuiCol_TableRowBgAlt] = ImVec4(line.x, line.y, line.z, 0.035f);
	c[ImGuiCol_FrameBg] = mix(a.frame, b.frame);
	c[ImGuiCol_FrameBgHovered] = mix(a.frameHover, b.frameHover);
	c[ImGuiCol_FrameBgActive] = mix(a.frameActive, b.frameActive);
	c[ImGuiCol_ButtonActive] = mix(a.buttonActive, b.buttonActive);
	c[ImGuiCol_Tab] = mix(a.tab, b.tab);
	c[ImGuiCol_TabHovered] = mix(a.tabHover, b.tabHover);
	c[ImGuiCol_Header] = mix(a.header, b.header);
	c[ImGuiCol_HeaderHovered] = mix(a.headerHover, b.headerHover);
	c[ImGuiCol_TableHeaderBg] = mix(a.tableHeader, b.tableHeader);
	c[ImGuiCol_ScrollbarBg] = ImVec4(0, 0, 0, 0.3f);
	c[ImGuiCol_ScrollbarGrab] = mix(a.scroll, b.scroll);
	c[ImGuiCol_TextDisabled] = DIM;
	c[ImGuiCol_Button] = STEEL;
	c[ImGuiCol_ButtonHovered] = HOVER;
	c[ImGuiCol_CheckMark] = ACCENT;
	c[ImGuiCol_SliderGrab] = STEEL;
	c[ImGuiCol_SliderGrabActive] = ACCENT;
	c[ImGuiCol_TabSelected] = STEEL;
	c[ImGuiCol_TabSelectedOverline] = ACCENT;
}

void applyTheme(const std::string & theme) {
	neon = theme != "classic";
	calmPalette = theme == "matrix" ? &MATRIX : theme == "amber" ? &AMBER_PALETTE : &NEON;
	backdrop = theme == "matrix" ? Backdrop::Streams : theme == "amber" ? Backdrop::Smoke : Backdrop::Drift;
	ImGui::StyleColorsDark();
	ImGuiStyle & s = ImGui::GetStyle();
	s.WindowRounding = 0;
	s.ChildRounding = 0;
	s.FrameRounding = neon ? 0 : 1;
	s.TabRounding = neon ? 0 : 1;
	s.GrabRounding = neon ? 0 : 1;
	s.ChildBorderSize = 1;
	s.FrameBorderSize = neon ? 1 : 0;
	s.FramePadding = ImVec2(10, 7);
	s.ItemSpacing = ImVec2(10, 8);
	s.ItemInnerSpacing = ImVec2(10, 6);
	s.WindowPadding = ImVec2(16, 14);
	s.CellPadding = ImVec2(8, 5);
	ImVec4 * c = s.Colors;
	if (!neon) {
		GREEN = ImVec4(0.30f, 0.82f, 0.52f, 1.0f);
		GREY = ImVec4(0.30f, 0.38f, 0.43f, 1.0f);
		ACCENT = ImVec4(0.13f, 0.72f, 0.94f, 1.0f);
		STEEL = ImVec4(0.0f, 0.52f, 0.65f, 1.0f);
		HOVER = ImVec4(0.01f, 0.57f, 0.86f, 1);
		DIM = ImVec4(0.55f, 0.64f, 0.69f, 1.0f);
		BRIGHT = ImVec4(0.95f, 0.97f, 0.98f, 1.0f);
		c[ImGuiCol_Text] = ImVec4(0.88f, 0.92f, 0.94f, 1);
		c[ImGuiCol_WindowBg] = ImVec4(0.020f, 0.035f, 0.050f, 1);
		c[ImGuiCol_ChildBg] = ImVec4(0.040f, 0.063f, 0.086f, 1);
		c[ImGuiCol_PopupBg] = ImVec4(0.040f, 0.063f, 0.086f, 0.97f);
		c[ImGuiCol_Border] = ImVec4(0.16f, 0.27f, 0.33f, 1);
		c[ImGuiCol_Separator] = ImVec4(0.0f, 0.52f, 0.65f, 0.55f);
		c[ImGuiCol_FrameBg] = ImVec4(0.06f, 0.10f, 0.13f, 1);
		c[ImGuiCol_FrameBgHovered] = ImVec4(0.08f, 0.16f, 0.20f, 1);
		c[ImGuiCol_FrameBgActive] = ImVec4(0.10f, 0.20f, 0.25f, 1);
		c[ImGuiCol_ButtonActive] = ImVec4(0.0f, 0.62f, 0.78f, 1);
		c[ImGuiCol_Tab] = ImVec4(0.06f, 0.10f, 0.13f, 1);
		c[ImGuiCol_TabHovered] = ImVec4(0.01f, 0.57f, 0.86f, 0.85f);
		c[ImGuiCol_Header] = ImVec4(0.0f, 0.52f, 0.65f, 0.45f);
		c[ImGuiCol_HeaderHovered] = ImVec4(0.01f, 0.57f, 0.86f, 0.6f);
		c[ImGuiCol_TableHeaderBg] = ImVec4(0.0f, 0.20f, 0.25f, 1);
		c[ImGuiCol_TableBorderStrong] = ImVec4(0.16f, 0.27f, 0.33f, 1);
		c[ImGuiCol_TableBorderLight] = ImVec4(0.11f, 0.19f, 0.23f, 1);
		c[ImGuiCol_TableRowBgAlt] = ImVec4(0.0f, 0.52f, 0.65f, 0.05f);
		c[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.035f, 0.05f, 1);
		c[ImGuiCol_ScrollbarGrab] = ImVec4(0.0f, 0.52f, 0.65f, 0.6f);
	} else {
		neonColours(alertLevel);
		headingFont = neonFaces.heading;
		labelFont = neonFaces.label;
		return;
	}
	c[ImGuiCol_TextDisabled] = DIM;
	c[ImGuiCol_Button] = STEEL;
	c[ImGuiCol_ButtonHovered] = HOVER;
	c[ImGuiCol_CheckMark] = ACCENT;
	c[ImGuiCol_SliderGrab] = STEEL;
	c[ImGuiCol_SliderGrabActive] = ACCENT;
	c[ImGuiCol_TabSelected] = STEEL;
	c[ImGuiCol_TabSelectedOverline] = ACCENT;
	headingFont = neon ? neonFaces.heading : classicFaces.heading;
	labelFont = neon ? neonFaces.label : classicFaces.label;
}

// a gaussian blur, one direction a pass, for the glow; GLSL for both of oF's renderers, rectangle textures
const char * blurVertex120 = R"(#version 120
void main() { gl_TexCoord[0] = gl_MultiTexCoord0; gl_FrontColor = gl_Color; gl_Position = ftransform(); })";
const char * blurFragment120 = R"(#version 120
#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect tex0;
uniform vec2 dir;
void main() {
	vec2 c = gl_TexCoord[0].st;
	vec4 s = texture2DRect(tex0, c) * 0.227027;
	s += (texture2DRect(tex0, c + dir * 1.384615) + texture2DRect(tex0, c - dir * 1.384615)) * 0.316216;
	s += (texture2DRect(tex0, c + dir * 3.230769) + texture2DRect(tex0, c - dir * 3.230769)) * 0.070270;
	gl_FragColor = s;
})";
const char * blurVertex150 = R"(#version 150
uniform mat4 modelViewProjectionMatrix;
in vec4 position;
in vec2 texcoord;
out vec2 uv;
void main() { uv = texcoord; gl_Position = modelViewProjectionMatrix * position; })";
const char * blurFragment150 = R"(#version 150
uniform sampler2DRect tex0;
uniform vec2 dir;
in vec2 uv;
out vec4 outColor;
void main() {
	vec4 s = texture(tex0, uv) * 0.227027;
	s += (texture(tex0, uv + dir * 1.384615) + texture(tex0, uv - dir * 1.384615)) * 0.316216;
	s += (texture(tex0, uv + dir * 3.230769) + texture(tex0, uv - dir * 3.230769)) * 0.070270;
	outColor = s;
})";

} // namespace

void ofApp::setup() {
	ofSetWindowTitle("Backburn Tokens");
	ofBackground(5, 9, 13);
	log.setFile(Paths::logFile().string());
	settings = Settings::load();
	animate = settings.animations;
	ofSetFrameRate(settings.frameRate);
	ofSetVerticalSync(settings.vsync);
	i18n::load(settings.language);
	// main.cpp made the window from these; changing it later needs a restart
	seeThroughNow = settings.seeThrough;
	// --tab=donate: the overview with the Donate panel open
	if (startTab == "donate") {
#if !BACKBURN_APP_STORE
		donateOpen = !access::sandboxed();
#endif
		startTab = "overview";
	}
	if (startTab.empty()) startTab = settings.startTab;
	if (!startDay.empty()) openDay[startTab] = startDay;
	if (!startTask.empty()) openTask[startTab] = startTask;
	resumeAccess();
	// the first launch opens on the Sources page, after the dive
	sourcesOpen = !settings.welcomed;
	hintsShown = settings.hints;
#if !BACKBURN_APP_STORE
	// the donation codes: plain 2D textures (ImGui can't draw rectangle ones), sharp-edged when scaled
	const bool arb = ofGetUsingArbTex();
	ofDisableArbTex();
	for (auto [texture, file] : { std::pair<ofTexture *, const char *>{ &paypalQr, "donate/paypal-qr.png" }, { &pepeQr, "donate/pepe-qr.png" } })
		if (ofLoadImage(*texture, file)) texture->setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	if (arb) ofEnableArbTex();
#endif
	// --tab=settings/export opens a Settings page
	if (size_t slash = startTab.find('/'); slash != std::string::npos) {
		startSubTab = startTab.substr(slash + 1);
		startTab = startTab.substr(0, slash);
	}
	std::snprintf(remoteUrl, sizeof(remoteUrl), "%s", settings.remoteUrl.c_str());
	std::snprintf(remoteToken, sizeof(remoteToken), "%s", settings.remoteToken.c_str());
	std::snprintf(exportFolder, sizeof(exportFolder), "%s", settings.exportFolder.c_str());
	std::snprintf(fileName, sizeof(fileName), "%s", settings.fileName.c_str());
	selectQuery(0);
	lanIp = lanAddress();

	collector = std::make_unique<Collector>(log);
	// sandboxed (or simulating it): a tool is read only after its folder is granted
	if (access::sandboxed())
		collector->setAllowed([this](const std::string & id) {
			const std::string folder = settings.provider(id).path.empty() ? defaultFolder(id) : settings.provider(id).path;
			return access::readable(folder);
		});
	collector->start(settings);
	if (access::simulating()) log.add("backburn", "simulating the sandboxed first launch: nothing is readable until granted");
	// --simulate-grant: the picker's callback, without the picker (for testing)
	for (const auto & id : ofSplitString(simulatedGrants, ",", true, true))
		for (auto & ps : settings.providers)
			if (ps.id == id) granted(ps, ps.path.empty() ? defaultFolder(id) : ps.path, "");
	api = std::make_unique<Api>(log, *collector);
	// a phone only reads; a desktop serves its report to them
	if (settings.serveApi && !collector->remote()) api->start(settings);
	log.add("backburn", collector->remote() ? "reading " + settings.remoteUrl : "reading this computer's logs");

	gui.setup(nullptr, false);
	auto spaced = [](float tracking, bool merge = false) {
		ImFontConfig config;
		config.GlyphExtraAdvanceX = tracking;
		config.MergeMode = merge;
		return config;
	};
	// font families come from Fonts.h (per platform); the first file on this machine wins
	const AppFonts & families = appFonts();
	const std::string unicodeFont = firstFontFile(families.unicode);
	const bool unicode = !unicodeFont.empty();
	auto face = [&](const std::vector<std::string> & candidates, float size, float tracking) -> ImFont * {
		const std::string path = firstFontFile(candidates);
		if (path.empty()) return nullptr;
		ImFontConfig config = spaced(tracking), fallback = spaced(tracking, true);
		ImFont * f = gui.addFont(path, size, &config);
		if (unicode) gui.addFont(unicodeFont, size, &fallback);
		return f;
	};
	ImFontConfig body = spaced(0.3f);
	if (unicode) gui.addFont(unicodeFont, 15.0f, &body, nullptr, true);
	// classic: condensed headings and labels
	classicFaces.heading = face(families.condensed, 15.0f, 1.5f);
	classicFaces.label = face(families.label, 14.0f, 1.0f);
	// neon: a wide heavy face for headings and labels, monospace for the data
	neonFaces.heading = face(families.wide, 15.0f, 1.2f);
	neonFaces.label = face(families.wide, 12.0f, 0.8f);
	neonFaces.body = face(families.mono, 14.0f, 0.2f);
	rainFont = face(families.mono, 13.0f, 0.0f);
	if (!neonFaces.heading) neonFaces = classicFaces;
	setTheme();

	ImGui::GetStyle().FontScaleMain = settings.uiScale;
	// where the window was last time
	if (auto glfw = dynamic_cast<ofAppGLFWWindow *>(ofGetWindowPtr()); glfw && settings.rememberWindow && settings.windowW > 200) {
		glfwSetWindowSize(glfw->getGLFWWindow(), settings.windowW, settings.windowH);
		glfwSetWindowPos(glfw->getGLFWWindow(), settings.windowX, settings.windowY);
	}
	if (settings.fullscreen) ofSetFullscreen(true);
	city.setup();
	cityColours();
	if (settings.intro) {
		city.startIntro(5.5f);
		uiFade = 0;
	}

	// quitting plays CONNECTION TERMINATED first: Esc and the window's close button start it, and it exits after
	ofSetEscapeQuitsApp(false);
	if (auto glfw = dynamic_cast<ofAppGLFWWindow *>(ofGetWindowPtr())) {
		static ofApp * self = this;
		glfwSetWindowCloseCallback(glfw->getGLFWWindow(), [](GLFWwindow * w) {
			glfwSetWindowShouldClose(w, GLFW_FALSE);
			self->terminate(true);
		});
	}
}

void ofApp::keyPressed(int key) {
	if (!city.introDone()) {
		city.skipIntro();
		return;
	}
	if (key == OF_KEY_ESC) terminate(true);
}

// a click during the dive skips it
void ofApp::mousePressed(int, int, int) {
	if (!city.introDone()) city.skipIntro();
}

// Folders picked on the Sources page are kept as bookmarks; in the sandbox they have to be resumed each launch.
void ofApp::resumeAccess() {
	bool changed = false;
	for (auto & ps : settings.providers) {
		std::string path;
		if (access::resume(ps.bookmark, path) && path != ps.path && path != defaultFolder(ps.id)) {
			ps.path = path;
			changed = true;
		}
		std::string file;
		access::resume(ps.fileBookmark, file);
	}
	if (changed) settings.save();
}

bool ofApp::cityShown() const { return settings.city || !city.introDone(); }

// The towers carry each source's lines: its plan, limits, models, recent tasks (titles only if they're shown),
// and Backburn's own log in terminal style. Never prompts.
void ofApp::feedCity() {
	std::map<std::string, std::vector<std::string>> lines;
	const auto logTail = log.tail(40);
	for (const auto & p : report.providers) {
		auto & l = lines[p.id];
		l.push_back("$ backburn --source " + p.id);
		l.push_back(p.found ? "[ OK ] " + p.source.substr(0, 36) : "[FAIL] " + p.error.substr(0, 36));
		if (!p.plan.empty()) l.push_back("plan      " + p.plan);
		for (const auto & lim : p.limits)
			l.push_back(ofToUpper(lim.name).substr(0, 14) + std::string(std::max<int>(1, 16 - int(lim.name.size())), ' ')
			            + (lim.estimate ? "~" : "") + ofToString(int(lim.percent)) + "%");
		l.push_back("used      " + amount(p, p.total));
		for (const auto & m : p.models) l.push_back(m.model.substr(0, 20) + "  " + tokens(m.output));
		int n = 0;
		for (const auto & t : p.tasks) {
			if (n++ >= 10) break;
			l.push_back("> " + (settings.showTitles ? p.title(t.session) : "session " + t.session.substr(0, 8)).substr(0, 28));
			l.push_back("    " + t.lastDay.substr(5) + "  " + amount(p, t));
		}
		for (const auto & line : logTail)
			if (line.rfind("[" + p.id + "]", 0) == 0 || line.rfind("[backburn]", 0) == 0) l.push_back(line.substr(0, 34));
		l.push_back("0x" + ofToHex(int(p.total.output & 0xffffff)) + "  " + tokens(p.total.allInput()) + " in");
	}
	city.setReport(report, lines);
}

// the city in the theme's colours: towers and text in its light, the board's traces in a cooler second colour
void ofApp::cityColours() {
	City::Colours c;
	auto f = [](const ImVec4 & v, float boost = 1) { return ofFloatColor(std::min(1.0f, v.x * boost), std::min(1.0f, v.y * boost), std::min(1.0f, v.z * boost)); };
	c.tower = f(ACCENT);
	c.edge = f(BRIGHT);
	c.text = f(ImGui::GetStyle().Colors[ImGuiCol_Text], 1.1f);
	const std::string theme = startTheme.empty() ? settings.theme : startTheme;
	// neon's board is violet under aqua glass, as in the film; the others keep to their own colour
	c.trace = theme == "neon" || theme == "classic" ? ofFloatColor(0.52f, 0.30f, 1.0f).getLerped(f(RAIN), alertLevel) : f(RAIN);
	c.pad = c.trace.getLerped(ofFloatColor(1, 1, 1), 0.3f);
	city.setColours(c);
}

void ofApp::drawCity() {
	const int w = ofGetWidth(), h = ofGetHeight();
	if (!cityFbo.isAllocated() || cityFbo.getWidth() != w || cityFbo.getHeight() != h || cityAntialias != settings.cityAntialias) {
		cityAntialias = settings.cityAntialias;
		ofFboSettings fs;
		fs.width = w;
		fs.height = h;
		fs.internalformat = GL_RGBA;
		fs.useDepth = true;
		fs.numSamples = cityAntialias;
		cityFbo.allocate(fs);
	}
	// the towers' faces are buffers of their own: drawn first, not inside the city's
	city.drawFaces();
	cityFbo.begin();
	ofClear(0, 0, 0, 0);
	city.draw(w, h);
	cityFbo.end();
	bloomCity();
	// multisampled: resolved here, in the context that owns the framebuffers, before the city window shows it
	if (cityWindow) cityFbo.getTexture();
}

// The city's bloom, with the same gaussian shader as the interface glow: the city frame, quarter-size,
// blurred both ways, kept to be added back over the city wherever it is composited (behind the panels,
// in the glow's scene, in the folded-away view). Without this the towers and board energy stay flat —
// the shader pass is what makes their light spill. Strength follows Settings > Graphics > Glow.
void ofApp::bloomCity() {
	if (!neon || settings.glow <= 0 || !blur.isLoaded() || !cityFbo.isAllocated()) return;
	const int w = std::max(1, int(cityFbo.getWidth()) / 4), h = std::max(1, int(cityFbo.getHeight()) / 4);
	if (!cityBloom.isAllocated() || cityBloom.getWidth() != w || cityBloom.getHeight() != h) cityBloom.allocate(w, h, GL_RGBA);
	for (ofFbo * f : { &glowA, &glowB })
		if (!f->isAllocated() || f->getWidth() != w || f->getHeight() != h) f->allocate(w, h, GL_RGBA);
	ofPushStyle();
	glowA.begin();
	ofClear(0, 0, 0, 0);
	ofSetColor(255);
	cityFbo.draw(0, 0, w, h);
	glowA.end();
	for (int pass = 0; pass < std::clamp(settings.glowPasses, 1, 4); pass++) {
		glowB.begin();
		ofClear(0, 0, 0, 0);
		blur.begin();
		blur.setUniform2f("dir", 1.5f, 0);
		glowA.draw(0, 0);
		blur.end();
		glowB.end();
		glowA.begin();
		ofClear(0, 0, 0, 0);
		blur.begin();
		blur.setUniform2f("dir", 0, 1.5f);
		glowB.draw(0, 0);
		blur.end();
		glowA.end();
	}
	cityBloom.begin();
	ofClear(0, 0, 0, 0);
	ofSetColor(255);
	glowA.draw(0, 0);
	cityBloom.end();
	ofPopStyle();
}

void ofApp::attachCityWindow(std::shared_ptr<ofAppBaseWindow> window, bool native) {
	cityWindow = window;
	cityNative = native;
	ofAddListener(window->events().draw, this, &ofApp::drawCityWindow);
	ofAddListener(window->events().exit, this, &ofApp::cityWindowClosed);
	log.add("backburn", std::string("city window open, ") + (native ? "drawn in its own context" : "the main window's frame"));
}

void ofApp::cityWindowClosed(ofEventArgs &) {
	ofRemoveListener(cityWindow->events().draw, this, &ofApp::drawCityWindow);
	cityWindow.reset();
	log.add("backburn", "city window closed");
}

// The city window's frame. Its context shares textures and buffers with the main one, not framebuffers or vertex
// arrays: texture mode shows the main window's city frame (resolved there), native draws the scene again here.
void ofApp::drawCityWindow(ofEventArgs &) {
	const int w = ofGetWidth(), h = ofGetHeight();
	ofBackground(0);
	if (cityNative) city.draw(w, h);
	else if (cityFbo.isAllocated()) {
		// fit the main window's frame, keeping its shape
		const float k = std::min(w / cityFbo.getWidth(), h / cityFbo.getHeight());
		const float fw = cityFbo.getWidth() * k, fh = cityFbo.getHeight() * k;
		ofSetColor(255);
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		cityFbo.getTexture().draw((w - fw) / 2, (h - fh) / 2, fw, fh);
	}
	ofDrawBitmapStringHighlight(std::string(cityNative ? "native: drawn in this context" : "texture: the main window's frame") + "  "
	                                + ofToString(w) + "x" + ofToString(h) + "  " + ofToString(ofGetFrameRate(), 0) + " fps",
	                            10, 20);
}

void ofApp::terminate(bool quit) {
	quitAfter = quitAfter || quit;
	if (terminatedAt < 0) terminatedAt = ofGetElapsedTimef();
	if (!animate && quit) ofExit();
}

// The film-style full-window notice: a violet wash, scan lines, huge dark condensed letters that flicker out
// one by one, then it fades. About two seconds.
void ofApp::drawTerminated() {
	// quitting runs longer: after the words the screen goes to black, then the app closes
	const double t = ofGetElapsedTimef() - terminatedAt, length = quitAfter ? 2.6 : 2.2;
	if (t > length) {
		terminatedAt = -1;
		if (quitAfter) ofExit();
		return;
	}
	const float in = float(std::min(1.0, t / 0.18)), out = quitAfter ? 1.0f : float(std::min(1.0, (length - t) / 0.35));
	const float a = std::min(in, out);
	ImDrawList * dl = ImGui::GetForegroundDrawList();
	const ImVec2 size = ImGui::GetIO().DisplaySize;
	dl->AddRectFilled(ImVec2(0, 0), size, ImGui::GetColorU32(ImVec4(0.42f, 0.38f, 0.58f, 0.62f * a)));
	for (float y = 0; y < size.y; y += 3) dl->AddLine(ImVec2(0, y), ImVec2(size.x, y), ImGui::GetColorU32(ImVec4(0, 0, 0, 0.10f * a)));
	// condensed, like the film's: the classic heading face
	ImFont * font = classicFaces.heading ? classicFaces.heading : ImGui::GetFont();
	const std::string text = "CONNECTION TERMINATED";
	float fontSize = size.x / 11.0f;
	ImVec2 extent = font->CalcTextSizeA(fontSize, FLT_MAX, 0, text.c_str());
	if (extent.x > size.x * 0.9f) {
		fontSize *= size.x * 0.9f / extent.x;
		extent = font->CalcTextSizeA(fontSize, FLT_MAX, 0, text.c_str());
	}
	ImVec2 pos((size.x - extent.x) / 2, (size.y - extent.y) / 2);
	// a letter drops out for a moment now and then, as if the signal's breaking up
	std::mt19937 rng(unsigned(t * 14));
	for (size_t i = 0; i < text.size(); i++) {
		const std::string ch(1, text[i]);
		const float w = font->CalcTextSizeA(fontSize, FLT_MAX, 0, ch.c_str()).x;
		const bool flicker = t > 0.35 && std::uniform_int_distribution<int>(0, 9)(rng) == 0;
		const float shade = flicker ? 0.35f : 1.0f;
		dl->AddText(font, fontSize, pos, ImGui::GetColorU32(ImVec4(0.05f, 0.03f, 0.10f, 0.92f * a * shade)), ch.c_str());
		pos.x += w;
	}
	// to black, over the words, holding a moment before the window closes
	if (quitAfter) {
		const float black = float(ofClamp((t - 1.5) / 0.8, 0, 1));
		dl->AddRectFilled(ImVec2(0, 0), size, ImGui::GetColorU32(ImVec4(0, 0, 0, black)));
	}
}

// The window's look: what the background clears to (see-through when the window was made transparent), its
// opacity and whether it floats on top. All of these apply live.
void ofApp::applyWindow() {
	ofColor clear = neon ? clearColour : ofColor(5, 9, 13);
	if (seeThroughNow) clear.a = (unsigned char)(255 * ofClamp(settings.backgroundAlpha, 0, 1));
	ofBackground(clear);
	// the classic theme paints its own window; let the desktop through there too
	if (!neon && seeThroughNow) ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = ofClamp(settings.backgroundAlpha, 0, 1);
	if (auto glfw = dynamic_cast<ofAppGLFWWindow *>(ofGetWindowPtr())) {
		glfwSetWindowOpacity(glfw->getGLFWWindow(), ofClamp(settings.windowOpacity, 0.3f, 1.0f));
		glfwSetWindowAttrib(glfw->getGLFWWindow(), GLFW_FLOATING, settings.alwaysOnTop ? GLFW_TRUE : GLFW_FALSE);
		glfwSetWindowAttrib(glfw->getGLFWWindow(), GLFW_RESIZABLE, settings.resizable ? GLFW_TRUE : GLFW_FALSE);
		glfwSetWindowAttrib(glfw->getGLFWWindow(), GLFW_DECORATED, settings.titleBar ? GLFW_TRUE : GLFW_FALSE);
	}
}

void ofApp::setTheme() {
	applyTheme(startTheme.empty() ? settings.theme : startTheme);
	applyWindow();
	if (!neon) return;
	if (!blur.isLoaded()) {
		bool gl3 = ofIsGLProgrammableRenderer();
		blur.setupShaderFromSource(GL_VERTEX_SHADER, gl3 ? blurVertex150 : blurVertex120);
		blur.setupShaderFromSource(GL_FRAGMENT_SHADER, gl3 ? blurFragment150 : blurFragment120);
		if (gl3) blur.bindDefaults();
		blur.linkProgram();
	}
}

// Smoke (amber): soft puffs rise from the bottom, swell, drift on noise and thin out, with embers floating up
// through them. Each puff is a few stacked circles, faint at the edge and denser in the middle.
void ofApp::drawSmoke() {
	ImDrawList * dl = ImGui::GetBackgroundDrawList();
	const ImVec2 size = ImGui::GetIO().DisplaySize;
	static std::mt19937 rng(31);
	std::uniform_real_distribution<float> unit(0, 1);
	const float dt = animate ? ImGui::GetIO().DeltaTime : 0;
	auto spawn = [&](Puff & p, bool anywhere) {
		p.x = unit(rng) * size.x;
		p.life = 9 + unit(rng) * 9;
		p.age = anywhere ? unit(rng) * p.life : 0;
		p.y = size.y + 60 - (anywhere ? p.age * 28 : 0);
		p.rise = 18 + unit(rng) * 22;
		p.seed = unit(rng) * 100;
		p.ember = unit(rng) < 0.45f;
		if (p.ember) p.life = 4 + unit(rng) * 6;
	};
	if (puffs.empty()) {
		puffs.resize(130);
		for (Puff & p : puffs) spawn(p, true);
	}
	const float t = float(ImGui::GetTime());
	for (Puff & p : puffs) {
		p.age += dt;
		if (p.age > p.life || p.y < -200) spawn(p, false);
		const float k = p.age / p.life;
		// currents: sideways on noise, a little faster as it rises and cools
		p.y -= p.rise * dt * (p.ember ? 1.6f : 1.0f);
		p.x += (ofNoise(p.seed, t * 0.15f) - 0.5f) * 40 * dt + (p.ember ? std::sin(t * 3 + p.seed) * 12 * dt : 0);
		const float alpha = std::sin(k * float(PI));   // in, then thin out
		if (p.ember) {
			// yellow-hot at first, cooling through orange to red as it rises; it flickers, with a small halo
			const float flicker = 0.65f + 0.35f * ofNoise(p.seed * 3.1f, t * 9.0f);
			ImVec4 c(1.0f, 0.85f - 0.65f * k, 0.35f - 0.30f * k, alpha * flicker);
			const float r = (1.4f + 1.4f * ofNoise(p.seed)) * (1.0f - 0.5f * k);
			dl->AddCircleFilled(ImVec2(p.x, p.y), r * 3.2f, ImGui::GetColorU32(ImVec4(c.x, c.y * 0.7f, c.z, 0.10f * c.w)));
			dl->AddCircleFilled(ImVec2(p.x, p.y), r, ImGui::GetColorU32(c));
			continue;
		}
		const float r = 30 + k * 130;
		// warm near the fire, greying as it rises
		ImVec4 base(RAIN.x * (1 - k * 0.5f) + 0.45f * k * 0.5f, RAIN.y * (1 - k * 0.5f) + 0.40f * k * 0.5f, RAIN.z * (1 - k * 0.5f) + 0.35f * k * 0.5f, 1);
		for (float layer : { 1.0f, 0.72f, 0.45f }) {
			ImVec4 c(base.x, base.y, base.z, 0.09f * alpha * (1.4f - layer));
			dl->AddCircleFilled(ImVec2(p.x, p.y), r * layer, ImGui::GetColorU32(c), 32);
		}
	}
}

// Code falling in streams (matrix): each column a run of characters with a bright head, fading behind
// it, that falls past the bottom and starts again from the top.
void ofApp::drawStreams() {
	ImDrawList * dl = ImGui::GetBackgroundDrawList();
	const ImVec2 size = ImGui::GetIO().DisplaySize;
	static std::mt19937 rng(23);
	static const char glyphs[] = "0123456789ABCDEF<>=+*:.$#@%&";
	auto glyph = [&] { return std::string(1, glyphs[std::uniform_int_distribution<int>(0, int(sizeof(glyphs)) - 2)(rng)]); };
	const float cell = 15;
	if (falls.empty() || fallsSize.x != size.x || fallsSize.y != size.y) {
		falls.clear();
		fallsSize = size;
		std::uniform_real_distribution<float> unit(0, 1);
		const int rows = int(size.y / cell) + 2;
		for (float x = 4; x < size.x; x += cell * 1.1f) {
			Fall f;
			f.x = x;
			f.head = unit(rng) * rows * 2 - rows;
			f.speed = 6 + unit(rng) * 14;   // rows a second
			f.length = 6 + int(unit(rng) * 22);
			for (int i = 0; i < rows; i++) f.chars.push_back(glyph());
			falls.push_back(f);
		}
	}
	const float dt = animate ? ImGui::GetIO().DeltaTime : 0;
	const int rows = falls.empty() ? 0 : int(falls.front().chars.size());
	for (Fall & f : falls) {
		f.head += f.speed * dt;
		if (f.head - f.length > rows) {
			f.head = -float(std::uniform_int_distribution<int>(0, rows)(rng));
			f.speed = 6 + std::uniform_real_distribution<float>(0, 14)(rng);
		}
		// the head changes its character as it goes
		if (animate && std::uniform_int_distribution<int>(0, 6)(rng) == 0)
			f.chars[std::uniform_int_distribution<int>(0, rows - 1)(rng)] = glyph();
		const int head = int(f.head);
		for (int i = std::max(0, head - f.length); i <= std::min(rows - 1, head); i++) {
			float fade = 1.0f - float(head - i) / float(f.length);
			bool lead = i == head;
			ImVec4 c = lead ? ImVec4(BRIGHT.x, BRIGHT.y, BRIGHT.z, 0.85f) : ImVec4(RAIN.x, RAIN.y, RAIN.z, 0.55f * fade * fade);
			dl->AddText(rainFont, 14, ImVec2(f.x, i * cell), ImGui::GetColorU32(c), f.chars[i].c_str());
		}
	}
}

// Columns of digits drifting down behind the panels, nearer ones bigger and brighter. Drawn into ImGui's
// background, so the glow takes them too.
void ofApp::drawDataRain() {
	if (!rainFont) return;
	if (backdrop == Backdrop::Streams) {
		drawStreams();
		return;
	}
	if (backdrop == Backdrop::Smoke) {
		drawSmoke();
		return;
	}
	ImDrawList * dl = ImGui::GetBackgroundDrawList();
	const ImVec2 size = ImGui::GetIO().DisplaySize;
	if (rain.empty() || rainSize.x != size.x || rainSize.y != size.y) {
		rain.clear();
		rainSize = size;
		std::mt19937 rng(7);
		std::uniform_real_distribution<float> unit(0, 1);
		for (float x = -10; x < size.x;) {
			Column col;
			col.x = x;
			col.depth = unit(rng);
			col.speed = 6 + col.depth * 22;
			col.offset = unit(rng) * size.y;
			col.size = 9 + col.depth * 9;
			int lines = int(size.y / col.size) + 2;
			for (int i = 0; i < lines; i++) col.lines.push_back(rainLine(rng));
			x += col.size * (4.5f + unit(rng) * 5);
			rain.push_back(col);
		}
	}
	const float dt = animate ? ImGui::GetIO().DeltaTime : 0;
	static std::mt19937 rng(11);
	for (Column & col : rain) {
		col.offset = std::fmod(col.offset + col.speed * dt, col.size * col.lines.size());
		// now and then a line changes
		if (animate && std::uniform_int_distribution<int>(0, 40)(rng) == 0)
			col.lines[std::uniform_int_distribution<size_t>(0, col.lines.size() - 1)(rng)] = rainLine(rng);
		ImU32 colour = ImGui::GetColorU32(ImVec4(RAIN.x, RAIN.y, RAIN.z, 0.05f + col.depth * 0.13f));
		for (size_t i = 0; i < col.lines.size(); i++) {
			float y = std::fmod(i * col.size + col.offset, col.size * col.lines.size()) - col.size;
			dl->AddText(rainFont, col.size, ImVec2(col.x, y), colour, col.lines[i].c_str());
		}
	}
}

std::string ofApp::rainLine(std::mt19937 & rng) {
	static const char * shapes[] = { "0000000", "00000", "0000", "000000000", "00", "000", "000000" };
	std::string line = shapes[std::uniform_int_distribution<int>(0, 6)(rng)];
	for (char & ch : line)
		if (std::uniform_int_distribution<int>(0, 2)(rng)) ch = char('0' + std::uniform_int_distribution<int>(0, 9)(rng));
	return line;
}

// The glow: the frame is drawn into scene, a quarter-size copy is blurred, and added back over it.
void ofApp::drawGlow(bool withCity) {
	const int w = ofGetWidth(), h = ofGetHeight();
	if (!scene.isAllocated() || scene.getWidth() != w || scene.getHeight() != h) {
		scene.allocate(w, h, GL_RGBA);
		for (ofFbo * f : { &glowA, &glowB }) f->allocate(std::max(1, w / 4), std::max(1, h / 4), GL_RGBA);
	}
	scene.begin();
	ofClear(0, 0, 0, 0);
	// the city under the interface; drawn flipped here because the whole scene is flipped back below
	if (withCity) {
		ofSetColor(255);
		cityFbo.draw(0, h, w, -h);
		if (cityBloom.isAllocated() && settings.glow > 0) {
			ofEnableBlendMode(OF_BLENDMODE_ADD);
			ofSetColor(255 * std::min(1.0f, settings.glow));
			cityBloom.draw(0, h, w, -h);
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			ofSetColor(255);
		}
	}
	gui.draw();
	scene.end();

	ofPushStyle();
	ofSetColor(255);
	// ImGui's colours are already multiplied by their alpha
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	// ImGui draws into the buffer with GL's y up; drawn back flipped, so the frame stands the right way
	scene.draw(0, h, w, -h);

	glowA.begin();
	ofClear(0, 0, 0, 0);
	ofDisableBlendMode();
	scene.draw(0, 0, glowA.getWidth(), glowA.getHeight());
	glowA.end();
	for (int pass = 0; pass < std::clamp(settings.glowPasses, 1, 4); pass++) {
		glowB.begin();
		ofClear(0, 0, 0, 0);
		blur.begin();
		blur.setUniform2f("dir", 1.5f, 0);
		glowA.draw(0, 0);
		blur.end();
		glowB.end();
		glowA.begin();
		ofClear(0, 0, 0, 0);
		blur.begin();
		blur.setUniform2f("dir", 0, 1.5f);
		glowB.draw(0, 0);
		blur.end();
		glowA.end();
	}
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	float k = ofClamp(settings.glow, 0, 2);
	ofSetColor(255 * std::min(1.0f, k));
	glowA.draw(0, h, w, -h);
	if (k > 1) {
		ofSetColor(255 * (k - 1));
		glowA.draw(0, h, w, -h);
	}
	ofPopStyle();
}

// Each frame: is the window in front, can it be seen. In front it runs at the most Settings allows; behind another
// app it slows (or freezes); hidden it stops drawing, and with vsync off, since macOS stalls a hidden window at its
// next buffer swap and update() would stop with it.
void ofApp::pace() {
	auto glfw = dynamic_cast<ofAppGLFWWindow *>(ofGetWindowPtr());
	if (!glfw) return;
	GLFWwindow * w = glfw->getGLFWWindow();
	focused = glfwGetWindowAttrib(w, GLFW_FOCUSED);
	// the city window counts: looking at it, the city keeps moving
	if (auto city = std::dynamic_pointer_cast<ofAppGLFWWindow>(cityWindow))
		if (city->getGLFWWindow()) focused = focused || glfwGetWindowAttrib(city->getGLFWWindow(), GLFW_FOCUSED);
	visible = !glfwGetWindowAttrib(w, GLFW_ICONIFIED) && platform::windowVisible(glfw->getCocoaWindow());
	// a screenshot run must draw whatever its window is doing
	const bool hidden = !visible && settings.stopHidden && screenshotPath.empty();
	drawing = !hidden;
	int rate = settings.frameRate;
	bool still = false;
	if (hidden) rate = 4;
	else if (!focused && settings.unfocused != "full") {
		rate = settings.unfocusedRate;
		still = settings.unfocused == "freeze";
	}
	animate = settings.animations && !still;
	const int vsync = settings.vsync && !hidden ? 1 : 0;
	if (rate != pacedRate) {
		ofSetFrameRate(rate);
		pacedRate = rate;
	}
	if (vsync != pacedVsync) {
		ofSetVerticalSync(vsync);
		pacedVsync = vsync;
	}
	collector->setPaused(!visible && !settings.readHidden);
}

void ofApp::saveWindow() {
	auto glfw = dynamic_cast<ofAppGLFWWindow *>(ofGetWindowPtr());
	if (!glfw || !settings.rememberWindow || settings.fullscreen) return;
	glfwGetWindowPos(glfw->getGLFWWindow(), &settings.windowX, &settings.windowY);
	glfwGetWindowSize(glfw->getGLFWWindow(), &settings.windowW, &settings.windowH);
	settings.save();
}

void ofApp::update() {
	pace();
	if (collector->lastRefresh() == seenReport) return;
	seenReport = collector->lastRefresh();
	report = collector->snapshot();
	feedCity();
	// toast when a limit crosses the warning level or a new problem shows; the first report only records them
	std::vector<std::string> over;
	for (const auto & p : report.providers) {
		for (const auto & l : p.limits)
			if (l.percent >= settings.warnPercent && (!l.resetsAt || l.resetsAt > now())) {
				std::string id = p.name + " " + l.name;
				over.push_back(id);
				if (warnPrimed && settings.toastLimits && std::find(warned.begin(), warned.end(), id) == warned.end())
					toast(id + ": " + ofToString(int(l.percent)) + "% used, resets " + relativeTime(l.resetsAt), l.percent >= 100 ? RED : AMBER);
			}
		for (const auto & f : p.findings)
			if (f.level == Finding::Level::Problem && problemsSeen.insert(p.id + f.title).second && warnPrimed && settings.toastProblems)
				toast(p.name + ": " + f.title, RED);
	}
	warned = over;
	warnPrimed = !report.providers.empty();
	// bad enough for red alert: a limit at the warning level, or reading or serving has failed
	alarm = !over.empty() || !collector->lastError().empty() || (settings.serveApi && !collector->remote() && !api->running());
	autoExport();
}

// saved queries marked for it export once a day, after the first report of the day is in
void ofApp::autoExport() {
	if (report.providers.empty()) return;
	bool changed = false;
	for (size_t i = 0; i < settings.queries.size(); i++) {
		Query & q = settings.queries[i];
		if (!q.autoExport || (q.lastExport && localDay(q.lastExport) == localDay(now()))) continue;
		fs::path written;
		std::string error;
		if (exportQuery(q, report, settings, written, error)) {
			q.lastExport = now();
			if (int(i) == selectedQuery) editing.lastExport = q.lastExport;
			log.add("export", q.name + " (daily): " + written.string());
			changed = true;
		} else {
			log.add("export", q.name + " (daily) failed: " + error);
		}
	}
	if (changed) applySettings();
}

void ofApp::draw() {
	// hidden (and Settings says so): nothing to see, nothing drawn
	if (!drawing) return;
	gui.begin();
	ofxImAnim::beginFrame();
	ofxImAnim::update(ImGui::GetIO().DeltaTime);
	// red alert eases in and out over about a second
	if (neon) {
		float target = (settings.redAlert && alarm) || forceAlert ? 1.0f : 0.0f;
		float level = animate ? tween(key("red alert"), target, 1.1f, iam_ease_in_out_cubic) : target;
		if (std::abs(level - alertLevel) > 0.001f || (level == target && alertLevel != target)) {
			alertLevel = level;
			neonColours(alertLevel);
			applyWindow();
		}
	}
	// the city: the dive at launch, then flights to a tool's tower as its tab opens
	const float dt = ImGui::GetIO().DeltaTime;
	if (neon && alertLevel > 0.001f) cityColours();
	city.update(dt, animate);
	const bool withCity = cityShown();
	if (withCity || cityWindow) drawCity();
	// Fly-through sequence: fade the UI fully off, fly to the tower, then fade the UI back on.
	// Switching tabs mid-sequence restarts it (stage 1 with the new destination queued).
	if (flyStage != 0 && (!settings.flyTabs || !withCity)) flyStage = 0;   // feature off or city gone: just fade back
	if (flyStage == 1 && uiFade <= 0.03f) {
		city.flyTo(pendingFlight);
		flyStage = 2;
	} else if (flyStage == 2 && !city.flying()) {
		flyStage = 3;
	} else if (flyStage == 3 && uiFade >= 0.99f) {
		flyStage = 0;
	}
	const float target = !city.introDone() ? std::max(0.0f, (city.introProgress() - 0.72f) / 0.28f)
	                     : (flyStage == 1 || flyStage == 2) ? 0.0f : 1.0f;
	uiFade += (target - uiFade) * std::min(1.0f, dt * (city.introDone() ? 6.0f : 20.0f));
	if (flyStage == 3 && uiFade > 0.99f) uiFade = 1.0f;
	ImGui::GetStyle().Alpha = std::max(0.02f, uiFade);

	const Faces & faces = neon ? neonFaces : classicFaces;
	if (faces.body) ImGui::PushFont(faces.body, 0.0f);
	if (neon && settings.dataRain && !withCity) drawDataRain();
	// the first launch: only the Sources screen, nothing else of the app until it's done
	const bool firstRun = sourcesOpen && !settings.welcomed;
	if (firstRun && city.introDone()) {
		drawSources(true);
		drawToasts();
		if (terminatedAt >= 0) drawTerminated();
		if (faces.body) ImGui::PopFont();
		gui.end();
		if (neon && settings.glow > 0 && blur.isLoaded()) drawGlow(withCity);
		else gui.draw();
		if (!screenshotPath.empty() && ofGetElapsedTimef() > screenshotAt) {
			ofSaveScreen(screenshotPath);
			screenshotPath.clear();
		}
		return;
	}
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::Begin("Backburn Tokens", nullptr,
	             ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings
	                 | ImGuiWindowFlags_NoBringToFrontOnFocus);
	// while the UI is off for a flight it takes no input either
	ImGui::BeginDisabled(flyStage == 1 || flyStage == 2);

	drawHeader();
	ImGui::Separator();

	if (sidebarFolded) {
		ImGui::BeginChild("sidebar", ImVec2(52, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysUseWindowPadding);
		if (foldCorner("sidebar", true)) sidebarFolded = false;
		ImGui::EndChild();
	} else {
		ImGui::BeginChild("sidebar", ImVec2(300, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysUseWindowPadding);
		if (foldCorner("sidebar", false)) sidebarFolded = true;
		drawSidebar();
		ImGui::EndChild();
	}
	hudFrame();
	ImGui::SameLine();

	if (mainFolded) {
		ImGui::BeginChild("main", ImVec2(52, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysUseWindowPadding);
		if (foldCorner("main", true)) mainFolded = false;
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::Dummy(ImVec2(0, 0));   // the rest of the row stays empty: the city shows there
	} else {
		ImGui::BeginChild("main", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysUseWindowPadding);
		if (foldCorner("main", false)) mainFolded = true;
	if (ImGui::BeginTabBar("tabs")) {
		auto tab = [&](const std::string & name, const std::string & label) {
			ImGuiTabItemFlags flags = ofToLower(startTab) == name ? ImGuiTabItemFlags_SetSelected : 0;
			ImGui::PushFont(labelFont, 0.0f);
			bool open = ImGui::BeginTabItem((upper(i18n::tr(label)) + "###" + name).c_str(), nullptr, flags);
			ImGui::PopFont();
			return open;
		};
		auto content = [&](const std::string & name, const std::function<void()> & drawTab) {
			if (shownTab != name) {
				shownTab = name;
				tabSwitches++;
				// a tool's tab flies to its tower; the rest to the whole row
				const bool source = report.find(name) != nullptr;
				const std::string to = source ? name : "";
				if (settings.flyTabs && withCity && city.introDone() && to != flewTo) {
					// queue it: the UI fades fully off first, then the flight starts (see draw())
					pendingFlight = to;
					flyStage = 1;
				}
				flewTo = to;
			}
			float shown = tween(key("tab " + name + ofToString(tabSwitches)), 1.0f, 0.25f, iam_ease_out_cubic, 0.0f);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (1.0f - shown) * 8.0f);
			// times the interface's own fade (the intro, flights between towers), not instead of it
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, shown * ImGui::GetStyle().Alpha);
			ImGui::BeginChild(("tab " + name).c_str());
			drawTab();
			ImGui::EndChild();
			ImGui::PopStyleVar();
			ImGui::EndTabItem();
		};
		if (tab("overview", "Overview")) content("overview", [&] { drawOverview(); });
		// one tab per provider, in the order from Settings (Claude first)
		for (const auto & p : report.providers)
			if (tab(p.id, p.name)) content(p.id, [&] { drawProvider(p); });
		if (tab("queries", "Queries")) content("queries", [&] { drawQueries(); });
		if (tab("settings", "Settings")) content("settings", [&] { drawSettings(); });
		if (tab("log", "Log")) content("log", [&] { drawLog(); });
		if (tab("how", "How it works")) content("how", [&] { drawHow(); });
		ImGui::EndTabBar();
		// a provider's tab only exists once the first report is in; wait for it
		bool providerTab = std::any_of(settings.providers.begin(), settings.providers.end(),
		                               [&](const ProviderSettings & ps) { return ps.id == startTab; });
		if (!report.providers.empty() || !providerTab) startTab.clear();
	}
	ImGui::EndChild();
	hudFrame();
	}

	ImGui::EndDisabled();
	ImGui::End();
	if (sourcesOpen && city.introDone()) drawSources(false);
#if !BACKBURN_APP_STORE
	if (donateOpen) drawDonate();
#endif
	drawToasts();
	if (terminatedAt >= 0) drawTerminated();
	if (faces.body) ImGui::PopFont();
	gui.end();
	if (neon && settings.glow > 0 && blur.isLoaded()) drawGlow(withCity);
	else {
		if (withCity) {
			ofSetColor(255);
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			cityFbo.draw(0, 0);
			if (cityBloom.isAllocated() && settings.glow > 0) {
				ofEnableBlendMode(OF_BLENDMODE_ADD);
				ofSetColor(255 * std::min(1.0f, settings.glow));
				cityBloom.draw(0, 0, ofGetWidth(), ofGetHeight());
				ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			}
		}
		gui.draw();
	}

	if (!screenshotPath.empty() && seenReport && ofGetElapsedTimef() > screenshotAt) {
		ofSaveScreen(screenshotPath);
		log.add("backburn", "saved " + screenshotPath);
		screenshotPath.clear();
	}
}

void ofApp::drawHeader() {
	fireText("BACKBURN TOKENS", ImGui::GetFontSize() * 2.1f);
	ImGui::SameLine(0, 26);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() * 0.55f);
	ImGui::TextColored(DIM, "%s", "AI usage and limits");
	ImGui::SameLine(0, 40);
	const float baseline = ImGui::GetCursorPosY() + ImGui::GetFontSize() * 0.55f;
	int i = 0;
	for (const auto & p : report.providers) {
		statusLight(p, settings.warnPercent, baseline, i++);
		ImGui::SameLine(0, 22);
	}
	// top right corner: FPS (if on). Folding lives on each panel's own corner, not here.
	if (settings.showFps) {
		const std::string fps = ofToString(int(std::round(ofGetFrameRate()))) + " FPS";
		ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - ImGui::CalcTextSize(fps.c_str()).x);
		ImGui::TextColored(ACCENT, "%s", fps.c_str());
		if (ImGui::IsItemHovered()) explain("%.1f ms a frame", 1000.0f / std::max(1.0f, float(ofGetFrameRate())));
	}
	ImGui::NewLine();
}

void ofApp::drawSidebar() {
	float w = ImGui::GetContentRegionAvail().x;
	// Claude first, at the top, biggest
	const Provider * claude = report.find("claude");
	section("Claude Code");
	if (!claude) {
		ImGui::TextColored(DIM, "%s", collector->busy() ? "Reading..." : settings.enabled("claude") ? "Not found" : "Off in Settings");
	} else {
		if (!claude->plan.empty()) ImGui::TextColored(DIM, "%s plan", claude->plan.c_str());
		for (const auto & l : claude->limits) {
			meter("side claude " + l.name, l, settings.warnPercent, 14);
			ImGui::Spacing();
		}
		if (claude->limitsAt) {
			bool stale = now() - claude->limitsAt > settings.staleHours * 3600;
			ImGui::TextColored(stale ? AMBER : DIM, "as of %s", relativeTime(claude->limitsAt).c_str());
			if (ImGui::IsItemHovered())
				explain("Claude Code saves these when it checks its limits.\nRun /usage in Claude Code to refresh them.");
		}
		const Credits & c = claude->credits;
		if (c.known) {
			ImGui::Spacing();
			ImGui::PushFont(labelFont, 0.0f);
			ImGui::TextColored(BRIGHT, "%s", "USAGE CREDITS");
			ImGui::PopFont();
			ImGui::SameLine();
			ImGui::TextColored(c.enabled ? GREEN : DIM, "%s", c.enabled ? "on" : "off");
			if (c.used >= 0) ImGui::TextColored(DIM, "%s used%s", money(c.used).c_str(), c.limit > 0 ? (" of " + money(c.limit)).c_str() : "");
		}
	}

	ImGui::Spacing();
	ImGui::Separator();
	for (const auto & p : report.providers) {
		if (p.id == "claude") continue;
		ImGui::Spacing();
		section(p.name.c_str());
		if (!p.found) {
			ImGui::TextColored(DIM, "%s", p.error.empty() ? "Not found" : p.error.c_str());
			continue;
		}
		if (!p.plan.empty()) ImGui::TextColored(DIM, "%s", p.plan.c_str());
		for (const auto & l : p.limits) meter("side " + p.id + l.name, l, settings.warnPercent, 8);
		if (p.limits.empty())
			ImGui::TextColored(DIM, "%s output tokens%s", tokens(p.total.output).c_str(), p.total.cost >= 0 || p.showsPercent() ? (", " + amount(p, p.total)).c_str() : "");
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	if (button(collector->busy() ? "Reading...###refresh" : "Refresh now###refresh", ImVec2(w, 0))) collector->refreshNow();
	if (button("Sources...###sources", ImVec2(w, 0))) sourcesOpen = true;
	// Apple allows donations to a person only through in-app purchase: the App Store build has none (BACKBURN_APP_STORE),
	// and a sandboxed run of any other build hides it too
#if !BACKBURN_APP_STORE
	if (!access::sandboxed() && button("Donate...###donate", ImVec2(w, 0))) donateOpen = true;
#endif
	if (button("Export...###quick export", ImVec2(w, 0))) ImGui::OpenPopup("quick export");
	if (ImGui::BeginPopup("quick export")) {
		for (const auto & q : settings.queries)
			if (ImGui::MenuItem(q.name.c_str(), q.format.c_str())) runExport(q);
		ImGui::Separator();
		if (ImGui::MenuItem("Everything, every row", "json")) {
			Query all;
			all.name = "Backburn all rows";
			all.groupBy = "row";
			all.format = "json";
			runExport(all);
		}
		ImGui::EndPopup();
	}
	if (!collector->lastError().empty()) {
		ImGui::PushTextWrapPos(0);
		ImGui::TextColored(RED, "%s", collector->lastError().c_str());
		ImGui::PopTextWrapPos();
	}
	ImGui::TextColored(DIM, "updated %s", relativeTime(collector->lastRefresh()).c_str());
	if (collector->remote()) ImGui::TextColored(DIM, "from %s", report.host.empty() ? settings.remoteUrl.c_str() : report.host.c_str());
	else if (!api->running()) {
		ImGui::Spacing();
		if (button("Start server", ImVec2(w, 0))) {
			settings.serveApi = true;
			applySettings(true);
			toast("Server running on port " + ofToString(settings.apiPort) + ".", GREEN);
		}
	} else if (api->running()) {
		ImGui::Spacing();
		if (button("Stop server", ImVec2(w, 0))) {
			settings.serveApi = false;
			applySettings(true);
			terminate(false);
		}
		if (button("Open in browser", ImVec2(w, 0)))
			ofLaunchBrowser("http://127.0.0.1:" + ofToString(settings.apiPort) + "/" + (settings.trustLocal ? "" : "?token=" + settings.apiToken));
		if (ImGui::IsItemHovered()) explain("This computer's page: totals, limits, the week's tasks, API links");
	}
}

void ofApp::drawOverview() {
	if (report.providers.empty()) {
		float a = 0.5f + 0.5f * pulse(key("loading"), 0.8f);
		ImGui::TextColored(ImVec4(DIM.x, DIM.y, DIM.z, a), "%s", "Reading the logs. The first read of a large Claude history takes a few seconds.");
		return;
	}
	for (const auto & p : report.providers) {
		ImGui::PushID(p.id.c_str());
		section(p.name.c_str());
		if (!p.found) {
			ImGui::TextColored(DIM, "%s", p.error.c_str());
			ImGui::PopID();
			ImGui::Spacing();
			continue;
		}
		float col = ImGui::GetContentRegionAvail().x / 4;
		card(p.showsPercent() ? "Of a weekly limit" : p.costReported ? "Cost reported" : p.id == "claude" ? "At API prices" : "Cost",
		     countedAmount(p.id + " cost", p, p.total),
		     // one short line: the next card starts a quarter across
		     p.showsPercent() && p.total.cost >= 0 ? money(p.total.cost) + " at API prices" : ofToString(p.projects.size()) + " projects");
		ImGui::SameLine(col);
		card("Output tokens", counted(p.id + " out", double(p.total.output), Shown::Tokens), ofToString(p.days.size()) + " days used");
		ImGui::SameLine(col * 2);
		int64_t input = p.total.allInput();
		std::string hit = input > 0 ? ofToString(int(std::round(p.total.cacheRead * 100.0 / input))) + "% from cache" : "";
		card("Input tokens", counted(p.id + " in", double(input), Shown::Tokens), hit);
		ImGui::SameLine(col * 3);
		ImGui::BeginGroup();
		for (const auto & l : p.limits) meter("over " + p.id + l.name, l, settings.warnPercent, 8);
		if (p.limits.empty()) ImGui::TextColored(DIM, "%s", "no limits reported");
		ImGui::EndGroup();
		for (const auto & f : p.findings)
			if (f.level != Finding::Level::Info) findingRow(f);
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::PopID();
	}
}

void ofApp::drawProvider(const Provider & p) {
	ImGui::PushID(p.id.c_str());
	if (!p.found) {
		ImGui::TextColored(DIM, "%s", p.error.c_str());
		ImGui::PopID();
		return;
	}
	// a task opened from a table: its prompts
	auto task = openTask.find(p.id);
	if (task != openTask.end()) {
		for (const auto & t : p.tasks)
			if (t.session == task->second) {
				drawTask(p, t);
				ImGui::PopID();
				return;
			}
		openTask.erase(task);
	}
	// a project opened from the table below; the one with no folder is kept as " "
	auto open = openProject.find(p.id);
	if (open != openProject.end()) {
		if (const ProjectUsage * project = p.project(open->second == " " ? "" : open->second)) {
			drawProject(p, *project);
			ImGui::PopID();
			return;
		}
		openProject.erase(open);
	}

	ImGui::TextColored(DIM, "From %s%s", p.source.c_str(), p.oldestLog ? (", oldest log " + localDay(p.oldestLog)).c_str() : "");

	ImGui::Spacing();
	section("Days");
	std::string & day = openDay[p.id];
	std::string clicked = dayChart(p.id, p.days, 30, day, &p);
	if (!clicked.empty()) day = clicked == day ? "" : clicked;
	if (!day.empty()) drawDayTasks(p, day, nullptr);

	if (!p.findings.empty()) {
		ImGui::Spacing();
		section("Audit");
		for (const auto & f : p.findings) findingRow(f);
	}

	if (!p.limits.empty()) {
		ImGui::Spacing();
		section("Limits");
		float half = ImGui::GetContentRegionAvail().x / 2 - 10;
		int i = 0;
		for (const auto & l : p.limits) {
			if (i % 2) ImGui::SameLine(half + 20);
			ImGui::BeginChild(("limit " + l.name).c_str(), ImVec2(half, 76));
			meter("tab " + p.id + l.name, l, settings.warnPercent, 12);
			ImGui::EndChild();
			i++;
		}
	}


	ImGui::Spacing();
	section("Tasks");
	drawTasks(p, nullptr);

	ImGui::Spacing();
	section("Models");
	std::vector<std::pair<std::string, Totals>> models;
	for (const auto & m : p.models) models.push_back({ m.model, m });
	totalsTable("models", models, "Model", p);

	ImGui::Spacing();
	section("Projects");
	ImGui::SetNextItemWidth(260);
	ImGui::InputTextWithHint("##projectfilter", "filter", projectFilter, sizeof(projectFilter));
	ImGui::SameLine();
	if (button("Export projects")) {
		Query q;
		q.name = p.name + " projects";
		q.providers = { p.id };
		q.project = projectFilter;
		runExport(q);
	}
	if (ImGui::IsItemHovered()) explain("This table (with the filter) as CSV, to %s", settings.exports().string().c_str());
	hint("Click a project for its days, models and its own export.");
	double total = std::max(1e-9, p.total.cost);
	if (ImGui::BeginTable("projects", 6, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Folder", ImGuiTableColumnFlags_WidthStretch, 4);
		ImGui::TableSetupColumn("Days", ImGuiTableColumnFlags_WidthStretch, 1.4f);
		ImGui::TableSetupColumn("Calls", ImGuiTableColumnFlags_WidthStretch, 0.8f);
		ImGui::TableSetupColumn("Output", ImGuiTableColumnFlags_WidthStretch, 0.9f);
		ImGui::TableSetupColumn(amountName(p), ImGuiTableColumnFlags_WidthStretch, 0.9f);
		ImGui::TableSetupColumn("Share", ImGuiTableColumnFlags_WidthStretch, 1.6f);
		tableHeaders();
		for (const auto & n : p.projects) {
			if (!contains(n.name, projectFilter)) continue;
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable((n.name.empty() ? std::string("(no folder)") : n.name).c_str(), false, ImGuiSelectableFlags_SpanAllColumns))
				openProject[p.id] = n.name.empty() ? " " : n.name;
			ImGui::TableNextColumn();
			ImGui::TextColored(DIM, "%s", (n.firstDay == n.lastDay ? n.firstDay : n.firstDay.substr(5) + " - " + n.lastDay.substr(5)).c_str());
			ImGui::TableNextColumn(); ImGui::Text("%lld", static_cast<long long>(n.calls));
			ImGui::TableNextColumn(); ImGui::TextUnformatted(tokens(n.output).c_str());
			ImGui::TableNextColumn(); ImGui::TextColored(ACCENT, "%s", amount(p, n).c_str());
			ImGui::TableNextColumn();
			float share = n.cost > 0 ? float(n.cost / total) : float(n.output) / float(std::max<int64_t>(1, p.total.output));
			ImVec2 a = ImGui::GetCursorScreenPos();
			float w = ImGui::GetContentRegionAvail().x * tween(key("share " + p.id + n.name), share, 0.6f);
			float h = ImGui::GetTextLineHeight();
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(a.x, a.y + h * 0.3f), ImVec2(a.x + w, a.y + h * 0.7f), ImGui::GetColorU32(STEEL));
			ImGui::Dummy(ImVec2(1, h));
		}
		ImGui::EndTable();
	}
	ImGui::PopID();
}

void ofApp::drawProject(const Provider & p, const ProjectUsage & project) {
	const std::string shownName = project.name.empty() ? "(no folder)" : project.name;
	if (button("< All projects")) openProject.erase(p.id);
	ImGui::SameLine();
	glowText(shownName, ImGui::GetFontSize() * 1.5f);
	ImGui::TextColored(DIM, "%s, %s to %s, %s", p.name.c_str(), project.firstDay.c_str(), project.lastDay.c_str(),
	                   ofJoinString(project.models, ", ").c_str());

	ImGui::Spacing();
	float col = ImGui::GetContentRegionAvail().x / 4;
	card(p.showsPercent() ? "Of a weekly limit" : p.costReported ? "Cost reported" : "Cost", countedAmount(p.id + project.name + " cost", p, project),
	     p.showsPercent() && project.cost >= 0 ? money(project.cost) + " at API prices" : "");
	ImGui::SameLine(col);
	card("Output tokens", counted(p.id + project.name + " out", double(project.output), Shown::Tokens));
	ImGui::SameLine(col * 2);
	card("Calls", ofToString(project.calls));
	ImGui::SameLine(col * 3);
	double share = p.total.cost > 0 && project.cost >= 0 ? project.cost / p.total.cost
	                                                     : double(project.output) / double(std::max<int64_t>(1, p.total.output));
	card("Of " + p.name, ofToString(int(std::round(share * 100))) + "%");

	// this project's rows: by day and by model
	std::map<std::string, DayUsage> days;
	std::map<std::string, Totals> models;
	for (const auto & r : p.rows) {
		if (r.project != project.name) continue;
		days[r.day].day = r.day;
		days[r.day].add(r);
		models[r.model].add(r);
	}
	std::vector<DayUsage> dayList;
	for (const auto & [d, u] : days) dayList.push_back(u);

	ImGui::Spacing();
	section("Days");
	std::string & day = openDay[p.id + "|" + project.name];
	std::string clicked = dayChart(p.id + " " + project.name, dayList, 30, day, &p);
	if (!clicked.empty()) day = clicked == day ? "" : clicked;
	if (!day.empty()) drawDayTasks(p, day, &project.name);

	ImGui::Spacing();
	section("Tasks");
	drawTasks(p, &project.name);

	ImGui::Spacing();
	section("Models");
	std::vector<std::pair<std::string, Totals>> modelRows(models.begin(), models.end());
	totalsTable("project models", modelRows, "Model", p);

	ImGui::Spacing();
	section("Export this project");
	static std::string format = "csv", group = "day";
	Query q;
	q.name = (project.name.empty() ? std::string("no folder") : ofFilePath::getFileName(project.name)) + " " + p.id;
	q.providers = { p.id };
	q.project = project.name;
	q.projectExact = true;
	choose("Format##project", format, formats(), 100);
	ImGui::SameLine(0, 30);
	choose("Group by##project", group, groupings(), 120);
	q.format = format;
	q.groupBy = group;
	if (button("Export")) runExport(q);
	ImGui::SameLine();
	if (button("Save as...")) {
		auto result = ofSystemSaveDialog(q.name + "." + q.format, "Save " + shownName);
		if (result.bSuccess) runExport(q, result.getPath());
	}
	ImGui::SameLine();
	if (button("Save as a query")) {
		settings.queries.push_back(q);
		applySettings();
		selectQuery(int(settings.queries.size()) - 1);
		toast("Saved \"" + q.name + "\" in Queries.", GREEN);
	}
	hint(describe(q) + ". Every day of it, from the ledger, whatever the tool has kept.");

	ImGui::Spacing();
	section("Every day");
	std::vector<std::pair<std::string, Totals>> dayRows;
	for (auto it = dayList.rbegin(); it != dayList.rend(); ++it) dayRows.push_back({ it->day, *it });
	totalsTable("project days", dayRows, "Day", p);
}

std::string ofApp::taskName(const Provider & p, const std::string & session) const {
	if (settings.showTitles) return p.title(session);
	return session.empty() ? std::string("(no session)") : "session " + session.substr(0, 8);
}

// what ran on one day: each task's use that day, dearest first
void ofApp::drawDayTasks(const Provider & p, const std::string & day, const std::string * project) {
	struct Line {
		Totals t;
		std::string project;
		std::set<std::string> models;
	};
	std::map<std::string, Line> tasks;
	Totals total;
	for (const auto & r : p.rows) {
		if (r.day != day || (project && r.project != *project)) continue;
		Line & l = tasks[r.session];
		l.t.add(r);
		l.project = r.project;
		l.models.insert(r.model);
		total.add(r);
	}
	std::vector<std::pair<std::string, Line>> sorted(tasks.begin(), tasks.end());
	std::sort(sorted.begin(), sorted.end(), [](const auto & a, const auto & b) {
		return a.second.t.cost != b.second.t.cost ? a.second.t.cost > b.second.t.cost : a.second.t.output > b.second.t.output;
	});
	ImGui::BeginChild(("day " + p.id + day).c_str(), ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysUseWindowPadding);
	ImGui::PushFont(labelFont, 0.0f);
	ImGui::TextColored(BRIGHT, "%s", upper(day + ": " + ofToString(sorted.size()) + (sorted.size() == 1 ? " task" : " tasks")).c_str());
	ImGui::PopFont();
	ImGui::SameLine();
	ImGui::TextColored(DIM, "%s, %s output tokens, %lld calls", amount(p, total).c_str(), tokens(total.output).c_str(),
	                   static_cast<long long>(total.calls));
	ImGui::SameLine(ImGui::GetContentRegionAvail().x + ImGui::GetCursorPosX() - 260);
	if (button("Export this day")) {
		Query q;
		q.name = p.name + " " + day;
		q.providers = { p.id };
		q.groupBy = "day+task";
		q.lastDays = 0;
		if (project) {
			q.project = *project;
			q.projectExact = true;
		}
		// only this day: the query has no date range, so filter the rows to it
		Report one = report;
		for (auto & op : one.providers) {
			op.rows.erase(std::remove_if(op.rows.begin(), op.rows.end(), [&](const UsageRow & r) { return r.day != day; }), op.rows.end());
			op.summarize();
		}
		fs::path written;
		std::string error;
		if (exportQuery(q, one, settings, written, error)) {
			lastExport = written.string();
			toast("Exported " + day + " to " + tidyPath(lastExport), GREEN);
			if (settings.revealAfterExport) reveal(lastExport);
		} else {
			toast("Couldn't export: " + error, RED);
		}
	}
	ImGui::SameLine();
	if (button("Close")) openDay.erase(project ? p.id + "|" + *project : p.id);
	if (ImGui::BeginTable("day tasks", project ? 5 : 6, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Task", ImGuiTableColumnFlags_WidthStretch, 4);
		if (!project) ImGui::TableSetupColumn("Folder", ImGuiTableColumnFlags_WidthStretch, 2.5f);
		ImGui::TableSetupColumn("Models", ImGuiTableColumnFlags_WidthStretch, 1.8f);
		ImGui::TableSetupColumn("Calls", ImGuiTableColumnFlags_WidthStretch, 0.7f);
		ImGui::TableSetupColumn("Output", ImGuiTableColumnFlags_WidthStretch, 0.8f);
		ImGui::TableSetupColumn(amountName(p), ImGuiTableColumnFlags_WidthStretch, 0.8f);
		tableHeaders();
		for (const auto & [session, l] : sorted) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable((taskName(p, session) + "##day " + session).c_str(), false, ImGuiSelectableFlags_SpanAllColumns) && !session.empty())
				openTask[p.id] = session;
			if (ImGui::IsItemHovered() && !session.empty()) ImGui::SetTooltip("session %s\nclick: what each prompt in it used", session.c_str());
			if (!project) {
				ImGui::TableNextColumn(); ImGui::TextColored(DIM, "%s", l.project.empty() ? "-" : l.project.c_str());
			}
			ImGui::TableNextColumn(); ImGui::TextColored(DIM, "%s", ofJoinString(std::vector<std::string>(l.models.begin(), l.models.end()), ", ").c_str());
			ImGui::TableNextColumn(); ImGui::Text("%lld", static_cast<long long>(l.t.calls));
			ImGui::TableNextColumn(); ImGui::TextUnformatted(tokens(l.t.output).c_str());
			ImGui::TableNextColumn(); ImGui::TextColored(ACCENT, "%s", amount(p, l.t).c_str());
		}
		ImGui::EndTable();
	}
	ImGui::EndChild();
}

// every task, most recent first, with a filter
void ofApp::drawTasks(const Provider & p, const std::string * project) {
	ImGui::PushID(project ? project->c_str() : "all tasks");
	ImGui::SetNextItemWidth(260);
	ImGui::InputTextWithHint("##taskfilter", "filter tasks", taskFilter, sizeof(taskFilter));
	if (!settings.showTitles) {
		ImGui::SameLine();
		ImGui::TextColored(DIM, "%s", "titles hidden (Settings > Privacy)");
	}
	int shown = 0;
	if (ImGui::BeginTable("tasks", project ? 5 : 6, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp
	                                                | ImGuiTableFlags_ScrollY, ImVec2(0, 280))) {
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableSetupColumn("Task", ImGuiTableColumnFlags_WidthStretch, 4);
		if (!project) ImGui::TableSetupColumn("Folder", ImGuiTableColumnFlags_WidthStretch, 2.5f);
		ImGui::TableSetupColumn("Days", ImGuiTableColumnFlags_WidthStretch, 1.4f);
		ImGui::TableSetupColumn("Calls", ImGuiTableColumnFlags_WidthStretch, 0.7f);
		ImGui::TableSetupColumn("Output", ImGuiTableColumnFlags_WidthStretch, 0.8f);
		ImGui::TableSetupColumn(amountName(p), ImGuiTableColumnFlags_WidthStretch, 0.8f);
		tableHeaders();
		for (const auto & t : p.tasks) {
			if (project && t.project != *project) continue;
			const std::string name = taskName(p, t.session);
			if (!contains(name + " " + t.project, taskFilter)) continue;
			shown++;
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			// a click opens the task, broken down by prompt
			if (ImGui::Selectable((name + "##" + t.session).c_str(), false, ImGuiSelectableFlags_SpanAllColumns)) openTask[p.id] = t.session;
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("%s\n%s\nclick: what each prompt in it used", t.session.c_str(), ofJoinString(t.models, ", ").c_str());
			if (!project) {
				ImGui::TableNextColumn(); ImGui::TextColored(DIM, "%s", t.project.empty() ? "-" : t.project.c_str());
			}
			ImGui::TableNextColumn();
			ImGui::TextColored(DIM, "%s", (t.firstDay == t.lastDay ? t.firstDay : t.firstDay.substr(5) + " - " + t.lastDay.substr(5)).c_str());
			ImGui::TableNextColumn(); ImGui::Text("%lld", static_cast<long long>(t.calls));
			ImGui::TableNextColumn(); ImGui::TextUnformatted(tokens(t.output).c_str());
			ImGui::TableNextColumn(); ImGui::TextColored(ACCENT, "%s", amount(p, t).c_str());
		}
		ImGui::EndTable();
	}
	ImGui::TextColored(DIM, "%d of %zu tasks", shown, p.tasks.size());
	ImGui::PopID();
}

void ofApp::drawTask(const Provider & p, const TaskUsage & task) {
	const std::string loadKey = p.id + "|" + task.session + "|" + (settings.readPrompts ? "text" : "");
	if (button("< Back")) {
		openTask.erase(p.id);
		return;
	}
	ImGui::SameLine();
	glowText(taskName(p, task.session), ImGui::GetFontSize() * 1.4f);
	ImGui::TextColored(DIM, "%s, %s, %s to %s, %s", p.name.c_str(), task.project.empty() ? "no folder" : task.project.c_str(),
	                   task.firstDay.c_str(), task.lastDay.c_str(), ofJoinString(task.models, ", ").c_str());

	ImGui::Spacing();
	float col = ImGui::GetContentRegionAvail().x / 4;
	card(p.showsPercent() ? "Of a weekly limit" : "Cost", countedAmount(p.id + task.session, p, task),
	     p.showsPercent() && task.cost >= 0 ? money(task.cost) + " at API prices" : "");
	ImGui::SameLine(col);
	card("Output tokens", counted(p.id + task.session + " out", double(task.output), Shown::Tokens));
	ImGui::SameLine(col * 2);
	card("Replies", ofToString(task.calls));
	ImGui::SameLine(col * 3);
	card("Prompts", promptsKey == loadKey && !promptLoad.valid() ? ofToString(prompts.prompts.size()) : "-");

	ImGui::Spacing();
	section("Prompts");
	if (!canBreakDown(p.id)) {
		hint(p.name + " can't be broken down by prompt yet. Claude Code and Grok Build can.");
		return;
	}
	// read on a background thread when the task, or whether to read the words, changes
	if (promptsKey != loadKey && !promptLoad.valid()) {
		promptsKey = loadKey;
		openPrompt = -1;
		const std::string folder = settings.provider(p.id).path;
		const bool withText = settings.readPrompts;
		const std::string id = p.id, session = task.session;
		promptLoad = std::async(std::launch::async, [id, folder, session, withText] { return readPrompts(id, folder, session, withText); });
	}
	if (promptLoad.valid()) {
		if (promptLoad.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
			float a = 0.5f + 0.5f * pulse(key("prompts loading"), 0.9f);
			ImGui::TextColored(ImVec4(DIM.x, DIM.y, DIM.z, a), "%s", "Reading the transcript...");
			return;
		}
		prompts = promptLoad.get();
		// the task may have changed while it read
		if (promptsKey != loadKey) return;
	}
	if (!prompts.error.empty()) {
		hint(prompts.error);
		return;
	}
	if (!settings.readPrompts)
		hint("The prompts' words are off: Settings > Privacy > Show prompts. The numbers below don't need them.");

	// which prompts carried the task
	std::vector<const PromptUsage *> order;
	for (const auto & pr : prompts.prompts) order.push_back(&pr);
	std::vector<const PromptUsage *> dearest = order;
	std::sort(dearest.begin(), dearest.end(), [](const PromptUsage * a, const PromptUsage * b) {
		return a->cost != b->cost ? a->cost > b->cost : a->output > b->output;
	});
	if (dearest.size() > 3 && task.cost > 0) {
		double top = 0;
		for (int i = 0; i < 3; i++) top += std::max(0.0, dearest[i]->cost);
		ImGui::TextColored(BRIGHT, "The 3 dearest of %zu prompts are %d%% of the task.", prompts.prompts.size(), int(std::round(top / task.cost * 100)));
	}
	if (ImGui::RadioButton("Dearest first", promptsByCost)) promptsByCost = true;
	ImGui::SameLine();
	if (ImGui::RadioButton("In order", !promptsByCost)) promptsByCost = false;
	ImGui::SameLine(0, 30);
	choose("##promptformat", promptFormat, formats(), 90);
	ImGui::SameLine();
	if (button("Export prompts")) exportPrompts(p, promptFormat);
	if (ImGui::IsItemHovered())
		explain("%s", settings.exportPrompts && settings.readPrompts ? "With the prompts' words (Settings > Privacy)."
		                                                                       : "Numbers only: the words need Settings > Privacy > Put prompts in exports.");

	const std::vector<const PromptUsage *> & rows = promptsByCost ? dearest : order;
	double most = 0;
	for (const auto * pr : rows) most = std::max(most, pr->units());
	if (ImGui::BeginTable("prompts", 7, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp
	                                        | ImGuiTableFlags_ScrollY, ImVec2(0, 340))) {
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableSetupColumn("#", ImGuiTableColumnFlags_WidthStretch, 0.4f);
		ImGui::TableSetupColumn("Sent", ImGuiTableColumnFlags_WidthStretch, 1.6f);
		ImGui::TableSetupColumn("Prompt", ImGuiTableColumnFlags_WidthStretch, 4.5f);
		ImGui::TableSetupColumn("Replies", ImGuiTableColumnFlags_WidthStretch, 0.7f);
		ImGui::TableSetupColumn("Tools", ImGuiTableColumnFlags_WidthStretch, 2.0f);
		ImGui::TableSetupColumn("Output", ImGuiTableColumnFlags_WidthStretch, 0.8f);
		ImGui::TableSetupColumn(amountName(p), ImGuiTableColumnFlags_WidthStretch, 1.2f);
		tableHeaders();
		for (const auto * pr : rows) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::Text("%d", pr->index);
			ImGui::TableNextColumn();
			std::time_t tt = static_cast<std::time_t>(pr->at);
			char sent[32];
			std::strftime(sent, sizeof(sent), "%d %b %H:%M", std::localtime(&tt));
			ImGui::TextColored(DIM, "%s", sent);
			ImGui::TableNextColumn();
			std::string words = settings.readPrompts ? pr->text : "prompt " + ofToString(pr->index);
			std::replace(words.begin(), words.end(), '\n', ' ');
			if (words.size() > 90) words = words.substr(0, 90) + "...";
			if (ImGui::Selectable((words + "##prompt" + ofToString(pr->index)).c_str(), openPrompt == pr->index, ImGuiSelectableFlags_SpanAllColumns))
				openPrompt = openPrompt == pr->index ? -1 : pr->index;
			ImGui::TableNextColumn(); ImGui::Text("%d", pr->replies);
			ImGui::TableNextColumn();
			std::vector<std::pair<int, std::string>> tools;
			for (const auto & [name, n] : pr->tools) tools.push_back({ n, name });
			std::sort(tools.rbegin(), tools.rend());
			std::string top;
			for (size_t i = 0; i < tools.size() && i < 2; i++) top += (top.empty() ? "" : ", ") + tools[i].second + " " + ofToString(tools[i].first);
			ImGui::TextColored(DIM, "%s", top.c_str());
			ImGui::TableNextColumn(); ImGui::TextUnformatted(tokens(pr->output).c_str());
			ImGui::TableNextColumn();
			// a bar for its share of the dearest prompt
			ImVec2 a = ImGui::GetCursorScreenPos();
			float h = ImGui::GetTextLineHeight(), w = ImGui::GetContentRegionAvail().x;
			if (most > 0)
				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(a.x, a.y + h * 0.15f), ImVec2(a.x + w * float(pr->units() / most), a.y + h * 0.85f),
				                                          ImGui::GetColorU32(ImVec4(STEEL.x, STEEL.y, STEEL.z, 0.55f)));
			ImGui::TextColored(ACCENT, "%s", amount(p, *pr).c_str());
		}
		ImGui::EndTable();
	}

	// the open prompt: its words, and what it set off
	for (const auto & pr : prompts.prompts) {
		if (pr.index != openPrompt) continue;
		ImGui::Spacing();
		ImGui::BeginChild("prompt detail", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysUseWindowPadding);
		ImGui::PushFont(labelFont, 0.0f);
		ImGui::TextColored(BRIGHT, "PROMPT %d OF %zu", pr.index, prompts.prompts.size());
		ImGui::PopFont();
		const int minutes = int((pr.lastAt - pr.at) / 60);
		ImGui::TextColored(DIM, "%d replies, %d tool calls over %d min. Input %s, cache read %s, cache write %s, output %s, thinking %s. %s",
		                   pr.replies, pr.toolCalls, minutes, tokens(pr.input).c_str(), tokens(pr.cacheRead).c_str(),
		                   tokens(pr.cacheWrite5m + pr.cacheWrite1h).c_str(), tokens(pr.output).c_str(), tokens(pr.reasoning).c_str(),
		                   (amount(p, pr) + (p.showsPercent() && pr.cost >= 0 ? " (" + money(pr.cost) + ")" : "")).c_str());
		if (!pr.tools.empty()) {
			std::vector<std::pair<int, std::string>> tools;
			for (const auto & [name, n] : pr.tools) tools.push_back({ n, name });
			std::sort(tools.rbegin(), tools.rend());
			std::string all;
			for (const auto & [n, name] : tools) all += (all.empty() ? "" : ", ") + name + " " + ofToString(n);
			hint("Tools: " + all);
		}
		if (settings.readPrompts) {
			std::string words = pr.text;
			ImGui::InputTextMultiline("##words", words.data(), words.size() + 1, ImVec2(-1, std::min(220.0f, 40 + ImGui::GetTextLineHeight() * 8)),
			                          ImGuiInputTextFlags_ReadOnly);
		}
		ImGui::EndChild();
	}
}

void ofApp::exportPrompts(const Provider & p, const std::string & format) {
	std::string name = taskName(p, prompts.session) + " prompts " + localDay(now());
	for (char & c : name)
		if (!(std::isalnum(static_cast<unsigned char>(c)) || c == ' ' || c == '-' || c == '_')) c = '-';
	const fs::path file = settings.exports() / (name + "." + format);
	std::error_code ec;
	fs::create_directories(file.parent_path(), ec);
	std::ofstream out(file, std::ios::binary);
	if (!out) {
		toast("Couldn't write " + file.string(), RED);
		return;
	}
	out << renderPrompts(prompts, p, format, settings.exportPrompts);
	out.close();
	lastExport = file.string();
	log.add("export", "prompts of " + prompts.session + (settings.exportPrompts && settings.readPrompts ? ", with words: " : ": ") + lastExport);
	toast("Exported the prompts to " + tidyPath(lastExport), GREEN);
	if (settings.revealAfterExport) reveal(lastExport);
}

void ofApp::granted(ProviderSettings & ps, const std::string & path, const std::string & bookmark) {
	access::grant(path);
	ps.bookmark = bookmark;
	ps.path = path == defaultFolder(ps.id) ? "" : path;
	folderFields[ps.id][0] = 0;
	applySettings();
	collector->refreshNow();
	log.add(ps.id, "access granted: " + tidyPath(path));
	toast(std::string(providerName(ps.id)) + ": access granted, reading now.", GREEN);
}

// The tools' folders, each with its state and what can be done. firstRun: the whole window, before any of the rest
// of the app; afterwards a panel over it.
void ofApp::drawSources(bool firstRun) {
	const ImVec2 size = ImGui::GetIO().DisplaySize;
	const ImVec2 panel = firstRun ? size : ImVec2(std::min(860.0f, size.x - 80), std::min(620.0f, size.y - 120));
	ImGui::SetNextWindowPos(firstRun ? ImVec2(0, 0) : ImVec2((size.x - panel.x) / 2, (size.y - panel.y) / 2 + 20));
	ImGui::SetNextWindowSize(panel);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, firstRun ? ImVec4(0, 0, 0, 0) : ImVec4(0.0f, 0.02f, 0.03f, 0.92f));
	ImGui::Begin("##sources", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove);
	// on the first launch the cards sit in a column down the middle
	const float column = firstRun ? std::min(820.0f, size.x - 80) : ImGui::GetContentRegionAvail().x;
	if (firstRun) {
		ImGui::SetCursorPosX((size.x - column) / 2);
		ImGui::BeginGroup();
		ImGui::Dummy(ImVec2(column, 30));
		fireText("BACKBURN TOKENS", ImGui::GetFontSize() * 2.1f);
	}
	ImGui::PushItemWidth(column);
	glowText(upper(i18n::tr("Sources")), ImGui::GetFontSize() * 1.8f);
	const bool sandbox = access::sandboxed();
	int grantedCount = 0, total = 0;
	for (auto & ps : settings.providers) {
		total++;
		grantedCount += access::readable(ps.path.empty() ? defaultFolder(ps.id) : ps.path);
	}
	ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + column);
	if (sandbox)
		ImGui::TextColored(DIM, "%s", (std::string(access::simulating() ? "Simulating the App Store build's first launch. " : "")
		                               + "Backburn runs in the sandbox, so it can only read a tool's folder once you grant it. "
		                                 "Nothing is asked until you click Grant access; skip any you don't use, and grant "
		                                 "them later from Sources in the sidebar. " + ofToString(grantedCount) + " of "
		                               + ofToString(total) + " granted.").c_str());
	else
		ImGui::TextColored(DIM, "%s", "Each AI tool keeps its usage in its own folder. This copy can read them as they are; "
		                              "nothing is asked for. Pick a different folder if a tool keeps its files somewhere else.");
	ImGui::PopTextWrapPos();
	ImGui::Spacing();
	for (auto & ps : settings.providers) {
		ImGui::PushID(ps.id.c_str());
		ImGui::BeginChild("source", ImVec2(column, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysUseWindowPadding);
		const std::string folder = ps.path.empty() ? defaultFolder(ps.id) : ps.path;
		std::error_code ec;
		const bool readable = access::readable(folder);
		// inside the sandbox a folder that isn't granted can't even be seen, so don't claim it's missing
		const bool exists = sandbox ? true : fs::exists(folder, ec);
		const std::string claudeJson = (Paths::home() / ".claude.json").string();
		const bool needsFile = ps.id == "claude" && readable && sandbox && !access::readable(claudeJson);
		ImVec4 colour = !exists ? GREY : readable && !needsFile ? GREEN : AMBER;
		ImVec2 p = ImGui::GetCursorScreenPos();
		float h = ImGui::GetTextLineHeight();
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(p.x + h * 0.4f, p.y + h * 0.6f), h * 0.32f, ImGui::GetColorU32(colour));
		ImGui::Dummy(ImVec2(h, h));
		ImGui::SameLine();
		section(providerName(ps.id));
		ImGui::SameLine(column * 0.45f);
		ImGui::TextColored(colour, "%s", !exists ? "not found: is it installed?"
		                                         : readable && !needsFile ? (sandbox ? "granted, reading" : "found, readable")
		                                         : needsFile              ? "granted; its limits file still needs access"
		                                                                  : "no access yet");
		ImGui::TextColored(DIM, "%s", tidyPath(folder).c_str());
		if (sandbox && !readable) {
			if (button("Grant access")) {
				std::string path, mark;
				if (access::pick("Grant Backburn read access to " + std::string(providerName(ps.id)) + "'s folder", folder, true, path, mark))
					granted(ps, path, mark);
			}
			ImGui::SameLine();
		}
		if (needsFile) {
			if (button("Grant ~/.claude.json")) {
				std::string path, mark;
				if (access::pick("Grant Backburn read access to ~/.claude.json (Claude Code's limits)", claudeJson, false, path, mark)) {
					ps.fileBookmark = mark;
					applySettings();
					collector->refreshNow();
					toast("Claude Code: limits file granted.", GREEN);
				}
			}
			ImGui::SameLine();
		}
		if (!sandbox || readable) {
			if (button("Choose folder...")) {
				std::string path, mark;
				if (access::pick("The folder " + std::string(providerName(ps.id)) + " keeps its files in", folder, true, path, mark))
					granted(ps, path, mark);
			}
			if (exists && readable) {
				ImGui::SameLine();
				if (button("Show in Finder")) access::reveal(folder);
			}
		}
		ImGui::EndChild();
		ImGui::PopID();
		ImGui::Spacing();
	}
	ImGui::Spacing();
	const char * done = settings.welcomed ? "Close" : sandbox && grantedCount == 0 ? "Continue without access" : "Continue";
	if (button(done, ImVec2(column, 0))) {
		sourcesOpen = false;
		if (!settings.welcomed) {
			settings.welcomed = true;
			applySettings();
		}
	}
	ImGui::PopItemWidth();
	if (firstRun) ImGui::EndGroup();
	ImGui::End();
	ImGui::PopStyleColor();
}

#if !BACKBURN_APP_STORE
// The author's donation page, as on danoli3.com: PayPal, and Pepe to an address. Each code was checked to read as
// the link or address printed beside it.
void ofApp::drawDonate() {
	const ImVec2 size = ImGui::GetIO().DisplaySize;
	const ImVec2 panel(std::min(860.0f, size.x - 80), std::min(640.0f, size.y - 120));
	ImGui::SetNextWindowPos(ImVec2((size.x - panel.x) / 2, (size.y - panel.y) / 2 + 20));
	ImGui::SetNextWindowSize(panel);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.02f, 0.03f, 0.94f));
	ImGui::Begin("##donate", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove);
	glowText(upper(i18n::tr("Donate")), ImGui::GetFontSize() * 1.8f);
	ImGui::PushTextWrapPos(0);
	ImGui::TextColored(BRIGHT, "%s", "Help me recover & continue creating");
	ImGui::TextColored(DIM, "%s", "Backburn Tokens is free. Every contribution helps with ongoing health recovery from tick-borne "
	                              "illness and allows me to keep creating music, code, games and advocating for better awareness. "
	                              "All donations are greatly appreciated. - Dan Rosser, danoli3.com");
	ImGui::PopTextWrapPos();
	ImGui::Spacing();
	const float column = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) / 2;
	// the cards stop short of the Close button
	const float cardHeight = ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing() * 1.5f;
	const float qr = std::max(80.0f, std::min(column - 40, cardHeight - ImGui::GetFrameHeightWithSpacing() * 4.5f));
	auto code = [&](const char * id, const char * name, const ofTexture & texture, const std::function<void()> & below) {
		ImGui::BeginChild(id, ImVec2(column, cardHeight), ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysUseWindowPadding);
		section(name);
		if (texture.isAllocated()) {
			ImGui::SetCursorPosX((column - qr) / 2);
			// a quiet zone round the code, so phones read it on a dark screen
			const ImVec2 at = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(at.x - 8, at.y - 8), ImVec2(at.x + qr + 8, at.y + qr + 8), IM_COL32_WHITE);
			ImGui::Image(GetImTextureID(texture), ImVec2(qr, qr));
			ImGui::Dummy(ImVec2(0, 8));
		} else {
			ImGui::TextColored(DIM, "%s", "(the code image is missing from data/donate)");
		}
		below();
		ImGui::EndChild();
	};
	code("paypal", "PayPal", paypalQr, [&] {
		if (button("Donate with PayPal", ImVec2(-1, 0)))
			ofLaunchBrowser(OBFSTR("https://www.paypal.com/donate/?business=SUY4WDZ2TFM6Q&currency_code=AUD"));
		ImGui::PushTextWrapPos(0);
		ImGui::TextColored(DIM, "%s", "Scan, or open PayPal's donate page (AUD). Before paying, check the browser address is PayPal's own site.");
		ImGui::PopTextWrapPos();
	});
	ImGui::SameLine();
	code("pepe", "Pepe", pepeQr, [&] {
		address("pepe", "", "0x8b6d84626c3a5af6e8a728388116cd7a50ad17b4");
		ImGui::PushTextWrapPos(0);
		ImGui::TextColored(DIM, "%s", "Scan, or copy the address. Check it in your wallet before sending.");
		ImGui::PopTextWrapPos();
	});
	ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetFrameHeightWithSpacing() - ImGui::GetStyle().WindowPadding.y);
	if (button("Close###donateClose", ImVec2(-1, 0))) donateOpen = false;
	ImGui::End();
	ImGui::PopStyleColor();
}
#endif

void ofApp::selectQuery(int index) {
	if (settings.queries.empty()) settings.queries.push_back(Query());
	selectedQuery = std::clamp(index, 0, int(settings.queries.size()) - 1);
	editing = settings.queries[selectedQuery];
	std::snprintf(queryName, sizeof(queryName), "%s", editing.name.c_str());
	std::snprintf(queryProject, sizeof(queryProject), "%s", editing.project.c_str());
	std::snprintf(queryModel, sizeof(queryModel), "%s", editing.model.c_str());
}

void ofApp::drawQueries() {
	// saved queries on the left, the one being edited and its answer on the right
	ImGui::BeginChild("saved queries", ImVec2(250, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AlwaysUseWindowPadding);
	section("Saved");
	for (size_t i = 0; i < settings.queries.size(); i++) {
		const Query & q = settings.queries[i];
		ImGui::PushID(int(i));
		if (ImGui::Selectable(q.name.c_str(), int(i) == selectedQuery)) selectQuery(int(i));
		if (q.autoExport) {
			ImGui::SameLine();
			ImGui::TextColored(GREEN, "%s", "daily");
		}
		ImGui::PopID();
	}
	ImGui::Spacing();
	if (button("New", ImVec2(-1, 0))) {
		Query q;
		q.name = "New query";
		settings.queries.push_back(q);
		applySettings();
		selectQuery(int(settings.queries.size()) - 1);
	}
	if (button("Duplicate", ImVec2(-1, 0))) {
		Query q = editing;
		q.name += " copy";
		q.autoExport = false;
		q.lastExport = 0;
		settings.queries.push_back(q);
		applySettings();
		selectQuery(int(settings.queries.size()) - 1);
	}
	ImGui::BeginDisabled(settings.queries.size() <= 1);
	if (button("Delete", ImVec2(-1, 0), &DANGER)) {
		settings.queries.erase(settings.queries.begin() + selectedQuery);
		applySettings();
		selectQuery(selectedQuery);
	}
	ImGui::EndDisabled();
	ImGui::EndChild();
	ImGui::SameLine();

	ImGui::BeginChild("query editor", ImVec2(0, 0), ImGuiChildFlags_None);
	section("Query");
	Query & q = editing;
	ImGui::SetNextItemWidth(360);
	if (ImGui::InputText("Name", queryName, sizeof(queryName))) q.name = queryName;
	ImGui::TextColored(DIM, "%s", "Tools");
	ImGui::SameLine(80);
	bool all = q.providers.empty();
	if (ImGui::Checkbox("All", &all)) q.providers.clear();
	for (const auto & ps : settings.providers) {
		ImGui::SameLine();
		bool on = std::find(q.providers.begin(), q.providers.end(), ps.id) != q.providers.end();
		if (ImGui::Checkbox(providerName(ps.id), &on)) {
			if (on) q.providers.push_back(ps.id);
			else q.providers.erase(std::remove(q.providers.begin(), q.providers.end(), ps.id), q.providers.end());
		}
	}
	ImGui::SetNextItemWidth(360);
	if (ImGui::InputTextWithHint("Project", "part of the folder, e.g. openFrameworks", queryProject, sizeof(queryProject))) q.project = queryProject;
	ImGui::SameLine();
	if (button("Pick...")) ImGui::OpenPopup("pick project");
	if (ImGui::BeginPopup("pick project")) {
		std::set<std::string> names;
		for (const auto & p : report.providers)
			for (const auto & pr : p.projects) names.insert(pr.name);
		for (const auto & n : names)
			if (!n.empty() && ImGui::MenuItem(n.c_str())) {
				q.project = n;
				q.projectExact = true;
				std::snprintf(queryProject, sizeof(queryProject), "%s", n.c_str());
			}
		ImGui::EndPopup();
	}
	ImGui::SameLine();
	ImGui::Checkbox("Exact", &q.projectExact);
	ImGui::SetNextItemWidth(360);
	if (ImGui::InputTextWithHint("Model", "part of the name, e.g. opus", queryModel, sizeof(queryModel))) q.model = queryModel;
	ImGui::SetNextItemWidth(200);
	ImGui::SliderInt("Days", &q.lastDays, 0, 365, q.lastDays == 0 ? "every day" : "last %d");
	for (int d : { 1, 7, 30, 90 }) {
		ImGui::SameLine();
		if (button((ofToString(d) + "###days" + ofToString(d)).c_str())) q.lastDays = d;
	}
	ImGui::SameLine();
	if (button("Every day")) q.lastDays = 0;
	choose("Group by", q.groupBy, groupings(), 140);
	ImGui::SameLine(0, 30);
	choose("Format", q.format, formats(), 100);
	ImGui::SameLine(0, 30);
	ImGui::Checkbox("Export daily", &q.autoExport);
	if (ImGui::IsItemHovered())
		explain("Saved queries with this on are exported once a day while Backburn runs,\nto %s",
		                  settings.exports().string().c_str());
	hint(describe(q) + (q.lastExport ? ". Last exported " + relativeTime(q.lastExport) + "." : "."));

	ImGui::Spacing();
	bool dirty = selectedQuery < int(settings.queries.size()) && !sameQuery(settings.queries[selectedQuery], q);
	if (button(dirty ? "Save changes###save" : "Saved###save", ImVec2(0, 0), dirty ? &SAVE : nullptr)) {
		settings.queries[selectedQuery] = q;
		applySettings();
	}
	ImGui::SameLine();
	if (button("Export")) runExport(q);
	if (ImGui::IsItemHovered()) ImGui::SetTooltip("To %s", settings.exports().string().c_str());
	ImGui::SameLine();
	if (button("Save as...")) {
		auto result = ofSystemSaveDialog(q.name + "." + q.format, "Export " + q.name);
		if (result.bSuccess) runExport(q, result.getPath());
	}
	ImGui::SameLine();
	if (button("Copy")) {
		ImGui::SetClipboardText(render(runQuery(q, report, settings.exportTitles), q.format, settings.exportTotals).c_str());
		toast("Copied \"" + q.name + "\" as " + q.format + ".", GREEN);
	}
	if (!lastExport.empty()) {
		ImGui::SameLine();
		if (button("Show last export")) reveal(lastExport);
		if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s", lastExport.c_str());
	}

	ImGui::Spacing();
	drawQueryPreview(q);
	ImGui::EndChild();
}

void ofApp::drawQueryPreview(const Query & q) {
	QueryResult res = runQuery(q, report, settings.showTitles);
	section("Answer");
	if (res.lines.empty()) {
		hint("Nothing matches. Try every day, all tools, or a shorter project filter.");
		return;
	}
	ImGui::TextColored(DIM, "%zu lines, %s to %s. Total %s, %s output tokens.", res.lines.size(), res.firstDay.c_str(), res.lastDay.c_str(),
	                   (res.total.weeklyPercent >= 0 ? percent(res.total.weeklyPercent) + " of the weekly limit" : money(res.total.cost)).c_str(),
	                   tokens(res.total.output).c_str());
	// each line in its provider's terms
	std::map<std::string, const Provider *> byName;
	for (const auto & p : report.providers) byName[p.name] = &p;
	auto lineAmount = [&](const QueryLine & l) {
		auto it = byName.find(l.provider);
		return it != byName.end() ? amount(*it->second, l) : money(l.cost);
	};
	const int columns = 1 + int(res.keyColumns.size()) + 5;
	if (!ImGui::BeginTable("answer", columns, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp
	                                              | ImGuiTableFlags_ScrollY))
		return;
	ImGui::TableSetupScrollFreeze(0, 1);
	ImGui::TableSetupColumn("Tool", ImGuiTableColumnFlags_WidthStretch, 1.5f);
	for (const auto & k : res.keyColumns)
		ImGui::TableSetupColumn(k.c_str(), ImGuiTableColumnFlags_WidthStretch, k == "project" || k == "task" ? 3.0f : 1.4f);
	for (const char * h : { "Calls", "Input", "Output", "Cache read", "Cost or limit" }) ImGui::TableSetupColumn(h, ImGuiTableColumnFlags_WidthStretch, 1);
	tableHeaders();
	ImGuiListClipper clipper;
	clipper.Begin(int(res.lines.size()) + 1);
	while (clipper.Step())
		for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
			bool totalLine = i == int(res.lines.size());
			const Totals & t = totalLine ? res.total : static_cast<const Totals &>(res.lines[i]);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (totalLine) ImGui::TextColored(BRIGHT, "%s", "TOTAL");
			else ImGui::TextUnformatted(res.lines[i].provider.c_str());
			for (size_t k = 0; k < res.keyColumns.size(); k++) {
				ImGui::TableNextColumn();
				if (!totalLine) ImGui::TextUnformatted(res.lines[i].keys[k].empty() ? "-" : res.lines[i].keys[k].c_str());
			}
			ImGui::TableNextColumn(); ImGui::Text("%lld", static_cast<long long>(t.calls));
			ImGui::TableNextColumn(); ImGui::TextUnformatted(tokens(t.input).c_str());
			ImGui::TableNextColumn(); ImGui::TextUnformatted(tokens(t.output).c_str());
			ImGui::TableNextColumn(); ImGui::TextUnformatted(tokens(t.cacheRead).c_str());
			ImGui::TableNextColumn();
			ImGui::TextColored(ACCENT, "%s", (totalLine ? (res.total.weeklyPercent >= 0 ? percent(res.total.weeklyPercent) : money(t.cost)) : lineAmount(res.lines[i])).c_str());
		}
	ImGui::EndTable();
}

void ofApp::runExport(const Query & q, const std::string & path) {
	fs::path written;
	std::string error;
	if (exportQuery(q, report, settings, written, error, path)) {
		lastExport = written.string();
		log.add("export", q.name + ": " + lastExport);
		toast("Exported \"" + q.name + "\" to " + tidyPath(lastExport), GREEN);
		if (settings.revealAfterExport) reveal(lastExport);
	} else {
		log.add("export", q.name + " failed: " + error);
		toast("Couldn't export: " + error, RED);
	}
}

void ofApp::drawLog() {
	ImGui::SetNextItemWidth(260);
	ImGui::InputTextWithHint("##logfilter", "filter", logFilter, sizeof(logFilter));
	ImGui::SameLine();
	ImGui::Checkbox("Follow", &followLog);
	ImGui::BeginChild("logtext", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
	for (const auto & line : log.tail(400)) {
		if (!contains(line, logFilter)) continue;
		bool error = ofIsStringInString(line, "failed") || ofIsStringInString(line, "unavailable") || ofIsStringInString(line, "refused");
		if (error) ImGui::PushStyleColor(ImGuiCol_Text, RED);
		ImGui::TextUnformatted(line.c_str());
		if (error) ImGui::PopStyleColor();
	}
	if (followLog) ImGui::SetScrollHereY(1.0f);
	ImGui::EndChild();
}

void ofApp::applySettings(bool restartApi) {
	settings.save();
	animate = settings.animations;
	collector->setSettings(settings);
	if (restartApi) {
		api->stop();
		if (settings.serveApi && !collector->remote()) api->start(settings);
		lanIp = lanAddress();
	}
}

void ofApp::drawSettings() {
	if (!ImGui::BeginTabBar("settings tabs")) return;
	auto page = [&](const char * name, const char * label, void (ofApp::*drawPage)()) {
		ImGuiTabItemFlags flags = startSubTab == name ? ImGuiTabItemFlags_SetSelected : 0;
		ImGui::PushFont(labelFont, 0.0f);
		bool open = ImGui::BeginTabItem((upper(i18n::tr(label)) + "###" + name).c_str(), nullptr, flags);
		ImGui::PopFont();
		if (!open) return;
		ImGui::BeginChild(name, ImVec2(0, 0), ImGuiChildFlags_AlwaysUseWindowPadding);
		(this->*drawPage)();
		ImGui::EndChild();
		ImGui::EndTabItem();
	};
	page("general", "General", &ofApp::drawGeneralSettings);
	page("graphics", "Graphics", &ofApp::drawGraphicsSettings);
	page("providers", "Providers", &ofApp::drawProviderSettings);
	page("alerts", "Alerts", &ofApp::drawAlertSettings);
	page("export", "Export", &ofApp::drawExportSettings);
	page("privacy", "Privacy", &ofApp::drawPrivacySettings);
	page("api", "API and phones", &ofApp::drawApiSettings);
	page("data", "Data", &ofApp::drawDataSettings);
	if (settings.showAdvanced) page("advanced", "Advanced", &ofApp::drawAdvancedSettings);
	page("copyright", "Copyright", &ofApp::drawCopyrightSettings);
	ImGui::EndTabBar();
	startSubTab.clear();
}

void ofApp::drawGeneralSettings() {
	section("Reading");
	ImGui::SetNextItemWidth(260);
	if (ImGui::SliderInt("Refresh every (seconds)", &settings.refreshSeconds, 10, 600)) applySettings();
	hint("How often the logs are read again. Only what was added since the last read is parsed, so short is cheap.");

	ImGui::Spacing();
	section("Start");
	std::vector<std::string> tabs = { "overview" };
	for (const auto & ps : settings.providers) tabs.push_back(ps.id);
	for (const char * t : { "queries", "settings" }) tabs.push_back(t);
	if (choose("Open on", settings.startTab, tabs, 160)) applySettings();
	hint("The tab shown when Backburn starts. --tab=name on the command line overrides it (--tab=settings/graphics for a "
	     "Settings page).");

	ImGui::Spacing();
	section("Language");
	std::string language = settings.language.empty() ? "en" : settings.language;
	if (choose("Interface", language, i18n::available(), 160)) {
		settings.language = language == "en" ? "" : language;
		i18n::load(settings.language);
		applySettings();
	}
	hint("Buttons, tabs and headings. A language is a file in data/lang (see lang/README.md); only English ships for now.");

	ImGui::Spacing();
	section("Advanced");
	if (ImGui::Checkbox("Show hints and descriptions", &settings.hints)) {
		hintsShown = settings.hints;
		applySettings();
	}
	ImGui::TextColored(DIM, "%s", "The grey explanations under settings and findings, and the explaining tooltips. How it works keeps its text.");
	if (ImGui::Checkbox("Show Advanced settings", &settings.showAdvanced)) applySettings();
	hint("An Advanced page in Settings listing every setting there is and what it is set to (read-only; "
	     "nothing changes there). Off, Settings keeps only the simple pages.");
}

void ofApp::drawGraphicsSettings() {
	section("Look");
	std::string theme = settings.theme;
	if (choose("Theme", theme, { "neon", "matrix", "amber", "classic" }, 160)) {
		settings.theme = theme;
		applySettings();
		setTheme();
	}
	hint("Neon: aqua type on dark teal with data drifting behind. Matrix: phosphor green with code falling in streams. "
	     "Amber: an amber terminal with smoke and embers rising. All three glow and can go red. Classic: Kingmaker's blue "
	     "panels.");
	ImGui::BeginDisabled(settings.theme == "classic");
	ImGui::SetNextItemWidth(260);
	if (ImGui::SliderFloat("Glow", &settings.glow, 0.0f, 2.0f, settings.glow <= 0 ? "off" : "%.2f")) applySettings();
	ImGui::SetNextItemWidth(260);
	if (ImGui::SliderInt("Glow quality", &settings.glowPasses, 1, 4, "%d passes")) applySettings();
	hint("More passes spread the glow wider and softer, for a little more work each frame.");
	if (ImGui::Checkbox("Background moving behind the panels", &settings.dataRain)) applySettings();
	if (ImGui::Checkbox("Red alert when things go bad", &settings.redAlert)) applySettings();
	hint("The whole screen turns red while a limit is at the warning level (Alerts), reading another computer fails, or "
	     "the server can't start. It turns back when they're fixed.");
	ImGui::EndDisabled();
	if (ImGui::Checkbox("Animations", &settings.animations)) applySettings();

	ImGui::Spacing();
	section("Frames");
	ImGui::SetNextItemWidth(260);
	if (ImGui::SliderInt("Max frame rate", &settings.frameRate, 0, 240, settings.frameRate == 0 ? "no limit" : "%d a second")) {
		if (settings.frameRate > 0 && settings.frameRate < 15) settings.frameRate = 15;
		applySettings();
	}
	if (ImGui::Checkbox("Vertical sync", &settings.vsync)) applySettings();
	if (ImGui::Checkbox("Show FPS (top right)", &settings.showFps)) applySettings();
	hint("Frames a second at the top right of the window, next to the tools' lights. It fades with the rest of "
	     "the interface (the intro dive, and a flight between towers).");
	hint("30 is smooth for panels and cheap on a laptop; 60 or more for the animations and the city. Vertical sync "
	     "stops tearing and holds the rate to the display's, so with it on there's no point going past the display.");
	ImGui::TextColored(DIM, "Now: %.0f frames a second, %s, %s.", ofGetFrameRate(), focused ? "in front" : "behind another app",
	                   visible ? "visible" : "hidden");
	ImGui::SetNextItemWidth(260);
	if (ImGui::SliderFloat("Interface size", &settings.uiScale, 0.8f, 1.6f, "%.2fx")) {
		applySettings();
		ImGui::GetStyle().FontScaleMain = settings.uiScale;
	}

	ImGui::Spacing();
	section("In the background");
	static const std::vector<std::string> modes = { "full", "slow", "freeze" };
	if (choose("Behind another app", settings.unfocused, modes, 160)) applySettings();
	ImGui::BeginDisabled(settings.unfocused == "full");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(180);
	if (ImGui::SliderInt("##unfocusedRate", &settings.unfocusedRate, 1, 30, "%d a second")) applySettings();
	ImGui::EndDisabled();
	hint("full: as in front. slow: fewer frames, animations still running. freeze: fewer frames and nothing moving.");
	if (ImGui::Checkbox("Stop drawing while hidden", &settings.stopHidden)) applySettings();
	hint("Minimised, on another Space or fully covered: nothing is drawn. The window picks up where it was when it shows.");
	if (ImGui::Checkbox("Keep reading the logs while hidden", &settings.readHidden)) applySettings();
	hint("On, the phone API stays up to date. Off, Backburn does nothing while hidden and reads again when it shows.");

	ImGui::Spacing();
	section("3D city");
	hint("Paused for now: these stay off unless you turn them on.");
	if (ImGui::Checkbox("Fly into the city at launch", &settings.intro)) applySettings();
	if (ImGui::Checkbox("The city behind the panels", &settings.city)) applySettings();
	if (ImGui::Checkbox("Fly to a tool's tower when its tab opens", &settings.flyTabs)) applySettings();
	hint("The interface fades out, the camera flies to the tower, then the interface fades back in. While it is "
	     "out it takes no clicks.");
	ImGui::SetNextItemWidth(260);
	static const std::vector<std::string> aa = { "0", "2", "4", "8" };
	std::string antialias = ofToString(settings.cityAntialias);
	if (choose("Antialiasing", antialias, aa, 120)) {
		settings.cityAntialias = ofToInt(antialias);
		applySettings();
	}
	hint("The city: a tower for each AI source, as tall as its share of the use, with its numbers and recent tasks "
	     "scrolling up it. Antialiasing smooths the towers' edges; 0 is fastest.");
	static const std::vector<std::string> windows = { "off", "texture", "native" };
	std::string cityWin = settings.cityWindow.empty() ? "off" : settings.cityWindow;
	if (choose("City window (debug)", cityWin, windows, 160)) {
		settings.cityWindow = cityWin == "off" ? "" : cityWin;
		applySettings();
	}
	hint("From the next start: the city in a second GLFW window sharing this one's context. texture shows this "
	     "window's city frame; native draws the scene again in the second context. --city-window[=native] opens it once.");

	ImGui::Spacing();
	section("Window");
	if (ImGui::Checkbox("Resizable", &settings.resizable)) {
		applySettings();
		applyWindow();
	}
	ImGui::SameLine(0, 30);
	if (ImGui::Checkbox("Title bar", &settings.titleBar)) {
		applySettings();
		applyWindow();
	}
	ImGui::SameLine(0, 30);
	if (ImGui::Checkbox("Full screen", &settings.fullscreen)) {
		applySettings();
		ofSetFullscreen(settings.fullscreen);
	}
	if (ImGui::Checkbox("Remember size and position", &settings.rememberWindow)) applySettings();
	if (!settings.titleBar) hint("Without a title bar the window can't be dragged: turn it back on here to move it.");
	static const std::vector<std::string> samples = { "0", "2", "4", "8" };
	std::string windowAA = ofToString(settings.windowAntialias);
	if (choose("Window antialiasing", windowAA, samples, 120)) {
		settings.windowAntialias = ofToInt(windowAA);
		applySettings();
	}
	ImGui::SameLine();
	ImGui::TextColored(DIM, "%s", "from the next start");
	ImGui::SetNextItemWidth(260);
	if (ImGui::SliderFloat("Window opacity", &settings.windowOpacity, 0.3f, 1.0f, "%.2f")) {
		applySettings();
		applyWindow();
	}
	hint("Fades the whole window, panels and all.");
	if (ImGui::Checkbox("See-through background", &settings.seeThrough)) applySettings();
	if (settings.seeThrough != seeThroughNow) {
		ImGui::SameLine();
		ImGui::TextColored(AMBER, "%s", "from the next start");
	}
	ImGui::BeginDisabled(!seeThroughNow);
	ImGui::SetNextItemWidth(260);
	if (ImGui::SliderFloat("Background", &settings.backgroundAlpha, 0.0f, 1.0f, "%.2f")) {
		applySettings();
		applyWindow();
	}
	ImGui::EndDisabled();
	hint("The desktop shows behind the panels; Background is how much of the theme's backdrop is drawn over it. The "
	     "window has to be made see-through, so turning it on or off takes a restart.");
	if (ImGui::Checkbox("Always on top", &settings.alwaysOnTop)) {
		applySettings();
		applyWindow();
	}
}

void ofApp::drawProviderSettings() {
	hint("Shown in this order, Claude first unless you move it. A tool that's off isn't read, shown or served.");
	ImGui::Spacing();
	int move = -1, moveBy = 0;
	for (size_t i = 0; i < settings.providers.size(); i++) {
		ProviderSettings & ps = settings.providers[i];
		ImGui::PushID(ps.id.c_str());
		ImGui::BeginChild("provider", ImVec2(0, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysUseWindowPadding);
		section(providerName(ps.id));
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - 150);
		ImGui::BeginDisabled(i == 0);
		if (button("Up")) move = int(i), moveBy = -1;
		ImGui::EndDisabled();
		ImGui::SameLine();
		ImGui::BeginDisabled(i + 1 == settings.providers.size());
		if (button("Down")) move = int(i), moveBy = 1;
		ImGui::EndDisabled();

		if (ImGui::Checkbox("Read this tool", &ps.enabled)) applySettings();

		// the folder its files are read from
		auto & field = folderFields[ps.id];
		if (field[0] == 0 && !ps.path.empty()) std::snprintf(field.data(), field.size(), "%s", ps.path.c_str());
		ImGui::SetNextItemWidth(420);
		ImGui::InputTextWithHint("Folder", tidyPath(defaultFolder(ps.id)).c_str(), field.data(), field.size());
		if (ImGui::IsItemDeactivatedAfterEdit()) {
			std::string path = ofTrim(field.data());
			if (path.rfind("~/", 0) == 0) path = (Paths::home() / path.substr(2)).string();
			ps.path = path;
			applySettings();
		}
		ImGui::SameLine();
		if (button("Choose...")) {
			auto result = ofSystemLoadDialog("Folder for " + std::string(providerName(ps.id)), true,
			                                 ps.path.empty() ? defaultFolder(ps.id) : ps.path);
			if (result.bSuccess) {
				ps.path = result.getPath();
				std::snprintf(field.data(), field.size(), "%s", ps.path.c_str());
				applySettings();
			}
		}
		if (!ps.path.empty()) {
			ImGui::SameLine();
			if (button("Default")) {
				ps.path.clear();
				field[0] = 0;
				applySettings();
			}
		}
		const char * env = ps.id == "claude" ? " (or CLAUDE_CONFIG_DIR)" : ps.id == "codex" ? " (or CODEX_HOME)" : "";
		hint(ps.path.empty() ? "Empty: " + tidyPath(defaultFolder(ps.id)) + env
		                     : "Reads " + tidyPath(ps.path) + " instead of " + tidyPath(defaultFolder(ps.id)) + ".");

		// a subscription's use as a share of its weekly limit, or dollars
		static const std::vector<std::pair<std::string, const char *>> billings = {
			{ "auto", "Automatic" }, { "subscription", "Subscription: % of the weekly limit" }, { "tokens", "Pay per token: dollars" } };
		const char * billingLabel = "Automatic";
		for (const auto & [k, label] : billings)
			if (k == ps.billing) billingLabel = label;
		ImGui::SetNextItemWidth(420);
		if (ImGui::BeginCombo("Billing", billingLabel)) {
			for (const auto & [k, label] : billings)
				if (ImGui::Selectable(label, k == ps.billing)) {
					ps.billing = k;
					applySettings();
				}
			ImGui::EndCombo();
		}
		const Provider * shownAs = report.find(ps.id);
		hint(std::string("Automatic: a subscription when the tool reports plan limits (Claude Code, Codex), else dollars. On a "
		                 "subscription, use is shown as a share of the weekly limit, measured from the limit's own readings")
		     + (shownAs && shownAs->weeklyRate > 0 ? ": 1% is about " + (shownAs->total.cost >= 0 ? money(1.0 / shownAs->weeklyRate) + " at API prices"
		                                                                                       : tokens(int64_t(1e6 / shownAs->weeklyRate)) + " weighted tokens")
		                                               + " (" + ofToString(shownAs->rateReadings) + " readings)."
		                                             : ". No reading yet."));

		// what the plan costs, for the "worth it" line in the audit
		float price = ps.planPrice > 0 ? float(ps.planPrice) : 0;
		ImGui::SetNextItemWidth(160);
		if (ImGui::InputFloat("Plan price a month (USD)", &price, 0, 0, "%.2f")) ps.planPrice = price > 0 ? price : -1;
		if (ImGui::IsItemDeactivatedAfterEdit()) applySettings();
		hint("0 for none. Compares what the plan cost over the days on record with what the same use costs per token, "
		     "where there's a price.");

		// the provider's own API key, kept in the Keychain
		ImGui::Spacing();
		ImGui::PushFont(labelFont, 0.0f);
		ImGui::TextColored(BRIGHT, "%s", upper(providerApi::keyName(ps.id)).c_str());
		ImGui::PopFont();
		const std::string saved = secrets::get(ps.id);
		auto & keyField = keyFields[ps.id];
		ImGui::SetNextItemWidth(420);
		ImGui::InputTextWithHint("##key", saved.empty() ? "paste the key" : secrets::masked(saved).c_str(), keyField.data(), keyField.size(),
		                         ImGuiInputTextFlags_Password);
		ImGui::SameLine();
		ImGui::BeginDisabled(keyField[0] == 0);
		if (button("Save key")) {
			if (secrets::set(ps.id, ofTrim(keyField.data()))) {
				toast(std::string(providerApi::keyName(ps.id)) + " saved in the Keychain.", GREEN);
				collector->testApis();
			} else {
				toast("Couldn't save the key in the Keychain.", RED);
			}
			std::fill(keyField.begin(), keyField.end(), 0);
		}
		ImGui::EndDisabled();
		if (!saved.empty()) {
			ImGui::SameLine();
			if (button("Remove")) {
				secrets::remove(ps.id);
				toast(std::string(providerApi::keyName(ps.id)) + " removed.", AMBER);
				collector->testApis();
			}
			if (providerApi::keyUsed(ps.id)) {
				ImGui::SameLine();
				if (button("Test")) collector->testApis();
			}
		}
		const std::string keyPage = providerApi::keyPage(ps.id);
		if (!keyPage.empty()) {
			if (button("Get a key")) ofLaunchBrowser(keyPage);
			if (ImGui::IsItemHovered()) explain("Where this key is made, in your browser");
			ImGui::SameLine();
			ImGui::TextColored(DIM, "%s", keyPage.c_str());
		}
		if (providerApi::needsAccount(ps.id)) {
			auto & account = accountFields[ps.id];
			if (account[0] == 0 && !ps.accountId.empty()) std::snprintf(account.data(), account.size(), "%s", ps.accountId.c_str());
			ImGui::SetNextItemWidth(420);
			ImGui::InputTextWithHint("Team id", "from console.x.ai", account.data(), account.size());
			if (ImGui::IsItemDeactivatedAfterEdit()) {
				ps.accountId = ofTrim(account.data());
				applySettings();
				collector->testApis();
			}
		}
		std::string status = collector->apiStatus(ps.id);
		if (!saved.empty()) ImGui::TextColored(ofIsStringInString(status, "failed") ? RED : GREEN, "%s",
		                                         ("In the Keychain (" + secrets::masked(saved) + ")" + (status.empty() ? "" : ". " + status)).c_str());
		hint(providerApi::keyUse(ps.id));
		ImGui::EndChild();
		ImGui::PopID();
		ImGui::Spacing();
	}
	// after the loop, so nothing is drawn from a moved entry
	if (move >= 0) {
		std::swap(settings.providers[move], settings.providers[move + moveBy]);
		applySettings();
	}
}

void ofApp::drawAlertSettings() {
	section("Limits");
	ImGui::SetNextItemWidth(260);
	if (ImGui::SliderInt("Warn at (% of a limit)", &settings.warnPercent, 50, 100)) applySettings();
	hint("A limit turns amber from here, red at 100%.");
	if (ImGui::Checkbox("Toast when a limit reaches it", &settings.toastLimits)) applySettings();
	ImGui::SetNextItemWidth(260);
	if (ImGui::SliderInt("Limits are stale after (hours)", &settings.staleHours, 1, 48)) applySettings();
	hint("Claude Code and Codex save their limits when they run; older than this, the audit says so.");

	ImGui::Spacing();
	section("Red alert");
	if (ImGui::Checkbox("Turn the screen red when things go bad", &settings.redAlert)) applySettings();
	hint(std::string("Glowing themes only. Red while a limit is at the warning level above, reading another computer fails, or "
	                 "the server can't start.") + (settings.redAlert && alarm ? " It's on now." : ""));

	ImGui::Spacing();
	section("Problems");
	if (ImGui::Checkbox("Toast for a new problem", &settings.toastProblems)) applySettings();
	hint("A balance running out, sign-in failing: anything the audit marks red.");
}

void ofApp::drawExportSettings() {
	section("Where");
	ImGui::SetNextItemWidth(420);
	ImGui::InputTextWithHint("Folder", Paths::defaultExports().string().c_str(), exportFolder, sizeof(exportFolder));
	if (ImGui::IsItemDeactivatedAfterEdit()) {
		settings.exportFolder = ofTrim(exportFolder);
		applySettings();
	}
	ImGui::SameLine();
	if (button("Choose...")) {
		auto result = ofSystemLoadDialog("Export folder", true, settings.exports().string());
		if (result.bSuccess) {
			settings.exportFolder = result.getPath();
			std::snprintf(exportFolder, sizeof(exportFolder), "%s", settings.exportFolder.c_str());
			applySettings();
		}
	}
	ImGui::SameLine();
	if (button("Open")) {
		std::error_code ec;
		fs::create_directories(settings.exports(), ec);
		ofLaunchBrowser("file://" + settings.exports().string());
	}

	ImGui::SetNextItemWidth(260);
	ImGui::InputText("File name", fileName, sizeof(fileName));
	if (ImGui::IsItemDeactivatedAfterEdit()) {
		settings.fileName = ofTrim(fileName);
		applySettings();
	}
	hint("{query} the query's name, {date} today, {time} hours and minutes, {provider} the tools. The format sets the "
	     "extension. The same name on the same day replaces the file; add {time} to keep each.");

	ImGui::Spacing();
	section("What");
	if (ImGui::Checkbox("Add a totals line", &settings.exportTotals)) applySettings();
	if (ImGui::Checkbox("Show the file after exporting", &settings.revealAfterExport)) applySettings();
	ImGui::SetNextItemWidth(160);
	if (ImGui::SliderInt("Keep per query", &settings.keepExports, 0, 365, settings.keepExports == 0 ? "all" : "newest %d")) applySettings();
	hint("Older exports of the same query are deleted beyond this.");

	ImGui::Spacing();
	section("Daily exports");
	int daily = 0;
	for (const auto & q : settings.queries)
		if (q.autoExport) {
			daily++;
			ImGui::BulletText("%s, last %s", q.name.c_str(), q.lastExport ? relativeTime(q.lastExport).c_str() : "never");
		}
	if (!daily) hint("None yet. Tick Export daily on a query in Queries.");
	if (button("Edit queries")) startTab = "queries";

	ImGui::Spacing();
	section("Formats");
	hint("CSV: one line per group with calls, input, output, cache read and write (5-minute, 1-hour), thinking and cost; "
	     "opens in Numbers or Excel.\nJSON: the same with the query that made it, for scripts.\nMarkdown: a table for notes "
	     "and issues.");
}

void ofApp::drawPrivacySettings() {
	section("Task titles");
	hint("Each tool names its sessions: Claude Code makes a title, Codex names the thread from your first words, Grok "
	     "writes a summary, Muse keeps a session name. They show what was run each day, and they're your words, so "
	     "choose where they go. Without them a task is shown by its session id.");
	if (ImGui::Checkbox("Show titles in this window", &settings.showTitles)) applySettings();
	if (ImGui::Checkbox("Put titles in exports", &settings.exportTitles)) applySettings();
	if (ImGui::Checkbox("Send titles over the API (to your phone)", &settings.serveTitles)) applySettings();
	hint("Off by default: anything on your network with the token could read them.");

	ImGui::Spacing();
	section("Prompts");
	hint("Open a task to see what each prompt in it used: replies, tool calls, tokens, cost. The numbers need nothing "
	     "from here. These show and export the prompts' own words, read from the tool's transcript when you open a "
	     "task. They're never kept in the ledger and never sent over the API.");
	if (ImGui::Checkbox("Show prompts in task details", &settings.readPrompts)) applySettings();
	ImGui::BeginDisabled(!settings.readPrompts);
	if (ImGui::Checkbox("Put prompts in exports", &settings.exportPrompts)) applySettings();
	ImGui::EndDisabled();
	hint("Exports are files on this computer: anything you share them with sees your prompts.");

	ImGui::Spacing();
	section("What's never read or sent");
	hint("Replies, code and tool output stay in the tools' own files, and prompts too unless you turn them on above. "
	     "Backburn reads token counts, models, folders, times, tool names and the titles above. API keys (Settings > Providers) are kept in the macOS Keychain, not in "
	     "settings.json, and never served.");
}

void ofApp::drawApiSettings() {
	section("Serve");
	if (collector->remote()) {
		hint("Off while this copy reads another computer.");
	} else {
		bool changed = ImGui::Checkbox("Serve the API", &settings.serveApi);
		if (changed && !settings.serveApi) terminate(false);
		changed |= ImGui::Checkbox("Allow other devices on this network", &settings.allowLan);
		if (ImGui::IsItemHovered()) explain("Off: only this computer can read it. On: anything on your network that has the token.");
		changed |= ImGui::Checkbox("No token needed from this computer", &settings.trustLocal);
		if (ImGui::IsItemHovered()) explain("So a browser here can open the page. Other devices always need the token.");
		ImGui::SetNextItemWidth(160);
		ImGui::InputInt("Port", &settings.apiPort, 0);
		settings.apiPort = std::clamp(settings.apiPort, 1024, 65535);
		if (changed || ImGui::IsItemDeactivatedAfterEdit()) applySettings(true);
		ImGui::TextColored(api->running() ? GREEN : DIM, "%s", api->running() ? ("running, " + ofToString(api->requests()) + " requests").c_str() : "stopped");
		ImGui::Checkbox("Show token", &showToken);
		if (showToken) address("token", "Token", settings.apiToken);
		if (button("New token")) {
			settings.apiToken = Settings::newToken();
			applySettings(true);
			toast("New token: phones need it again.", ACCENT);
		}
		std::string host = settings.allowLan && !lanIp.empty() ? lanIp : "127.0.0.1";
		std::string first = settings.queries.empty() ? std::string() : settings.queries.front().name;
		ofStringReplace(first, " ", "%20");
		ImGui::Spacing();
		address("api page", "The page (this computer)", "http://127.0.0.1:" + ofToString(settings.apiPort) + "/");
		if (settings.allowLan && !lanIp.empty())
			address("api phone", "For a phone's browser (holds the token: keep it to yourself)",
			        "http://" + lanIp + ":" + ofToString(settings.apiPort) + "/?token=" + settings.apiToken);
		address("api usage", "The report", "http://" + host + ":" + ofToString(settings.apiPort) + "/api/usage");
		address("api query", "A saved query", "http://" + host + ":" + ofToString(settings.apiPort) + "/api/query?name=" + first + "&format=csv");
	}

	ImGui::Spacing();
	section("Read another computer");
	hint("Show another Backburn's report here instead of this computer's logs. The phone apps work this way.");
	ImGui::SetNextItemWidth(360);
	ImGui::InputTextWithHint("Address", "http://192.168.1.20:8790", remoteUrl, sizeof(remoteUrl));
	ImGui::SetNextItemWidth(360);
	ImGui::InputTextWithHint("Its token", "from its Settings", remoteToken, sizeof(remoteToken), ImGuiInputTextFlags_Password);
	if (button("Apply")) {
		settings.remoteUrl = ofTrim(remoteUrl);
		settings.remoteToken = ofTrim(remoteToken);
		applySettings(true);
		collector->refreshNow();
	}
	ImGui::SameLine();
	if (button("Use this computer")) {
		remoteUrl[0] = remoteToken[0] = 0;
		settings.remoteUrl.clear();
		settings.remoteToken.clear();
		applySettings(true);
		collector->refreshNow();
	}
}

void ofApp::drawDataSettings() {
	section("Ledger");
	hint("Every day's use per project and model is kept in the ledger, so history goes on after the tools delete their "
	     "logs. " + ofToString(collector->ledgerRows()) + " rows now.");
	ImGui::SetNextItemWidth(200);
	if (ImGui::SliderInt("Keep", &settings.ledgerDays, 0, 1825, settings.ledgerDays == 0 ? "forever" : "%d days")) applySettings();
	bool asked = ImGui::GetTime() - clearLedgerAsked < 4;
	if (button(asked ? "Click again to clear###clear" : "Clear the ledger###clear", ImVec2(0, 0), &DANGER)) {
		if (asked) {
			collector->clearLedger();
			clearLedgerAsked = -10;
			toast("Ledger cleared; it fills again from the logs still there.", AMBER);
		} else {
			clearLedgerAsked = ImGui::GetTime();
		}
	}
	hint("Clearing forgets days whose logs are gone.");

	ImGui::Spacing();
	section("Files");
	address("data", "Settings, ledger and log", Paths::appData().string());
	if (button("Open data folder")) ofLaunchBrowser("file://" + Paths::appData().string());
	ImGui::SameLine();
	if (button("Reload settings")) {
		settings = Settings::load();
		selectQuery(selectedQuery);
		applySettings(true);
	}
	if (ImGui::IsItemHovered()) explain("After editing settings.json by hand");
}

void ofApp::drawCopyrightSettings() {
	auto para = [](const char * text) {
		ImGui::PushTextWrapPos(0);
		ImGui::TextUnformatted(text);
		ImGui::PopTextWrapPos();
	};
	// one row per name: what Backburn calls it, who owns that name, and a link to the owner.
	// The URL is shown as the link text so it can be read and checked before clicking (anti-phishing), and
	// every caller passes OBFSTR(...) so no link target sits in the binary in plaintext to be patched.
	auto owner = [](const char * name, const char * ownerName, const std::string & url) {
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::TextColored(BRIGHT, "%s", name);
		ImGui::TableNextColumn();
		ImGui::TextUnformatted(ownerName);
		ImGui::TableNextColumn();
		ImGui::PushID(url.c_str());
		ImGui::TextColored(ACCENT, "%s", url.c_str());
		if (ImGui::IsItemHovered()) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			explain("Open %s in your browser", url.c_str());
		}
		if (ImGui::IsItemClicked()) ofLaunchBrowser(url);
		ImGui::PopID();
	};

	section("This app");
	para("Backburn Tokens — Copyright (c) 2026 Daniel Rosser. Released under the MIT License: you may use, copy, "
	     "modify and distribute it, with the copyright notice kept. It comes with no warranty.");
	if (button("MIT License")) ofLaunchBrowser(OBFSTR("https://opensource.org/license/mit"));
	if (ImGui::IsItemHovered()) explain("The full licence text at opensource.org");

	ImGui::Spacing();
	section("Names and their owners");
	para("Backburn reads the log files of the tools below and names them so you can tell them apart. Every name, "
	     "product name and logo is the trademark of its owner. Backburn is independent: it is not made, sponsored, "
	     "endorsed or affiliated with any of them, and the owners do not endorse Backburn. No data is sent to them "
	     "by naming them here; the only calls Backburn makes are the provider APIs you add a key for, to the owner "
	     "of that key.");
	if (ImGui::BeginTable("owners", 3, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, 1.0f);
		ImGui::TableSetupColumn("Owner", ImGuiTableColumnFlags_WidthStretch, 1.4f);
		ImGui::TableSetupColumn("Link", ImGuiTableColumnFlags_WidthStretch, 1.6f);
		ImGui::TableHeadersRow();
		owner("Claude, Claude Code", "Anthropic, PBC", OBFSTR("https://www.anthropic.com"));
		owner("Codex, ChatGPT, OpenAI", "OpenAI", OBFSTR("https://openai.com"));
		owner("Grok", "xAI", OBFSTR("https://x.ai"));
		owner("Muse, Muse Spark", "Meta Platforms, Inc.", OBFSTR("https://www.meta.com"));
		ImGui::EndTable();
	}
	para("The links above go to each owner's own site, where their current terms, trademark and brand guidance live.");

	ImGui::Spacing();
	section("Built with");
	para(("Made with openFrameworks " + ofToString(OF_VERSION_MAJOR) + "." + ofToString(OF_VERSION_MINOR) + "."
	      + ofToString(OF_VERSION_PATCH)
	      + " — an open-source C++ toolkit for creative coding, MIT-licensed and kept by the openFrameworks "
	        "Community. Backburn's graphics, windowing and file reading are openFrameworks.").c_str());
	if (button("openFrameworks")) ofLaunchBrowser(OBFSTR("https://openframeworks.cc"));
	if (ImGui::IsItemHovered()) explain("openframeworks.cc, in your browser");
	ImGui::Spacing();
	para("Libraries in this build, with the versions in the openFrameworks tree it was built from and who "
	     "licenses each. Their copyrights and licences stay with their authors; Backburn's MIT licence covers "
	     "Backburn's own code only. The system SQLite library and Apple frameworks are Apple's and are not listed.");
	if (ImGui::BeginTable("licensors", 3, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("Library", ImGuiTableColumnFlags_WidthStretch, 1.0f);
		ImGui::TableSetupColumn("Licensor / licence", ImGuiTableColumnFlags_WidthStretch, 1.4f);
		ImGui::TableSetupColumn("Link", ImGuiTableColumnFlags_WidthStretch, 1.6f);
		ImGui::TableHeadersRow();
		owner("openFrameworks", "openFrameworks Community — MIT", OBFSTR("https://openframeworks.cc"));
		owner("GLFW 3.5.1", "The GLFW project — zlib/libpng licence", OBFSTR("https://www.glfw.org"));
		owner("GLEW 2.3.1", "GLEW authors — BSD / MIT", OBFSTR("https://glew.sourceforge.net"));
		owner("GLM 1.0.3", "G-Truc Creation — MIT", OBFSTR("https://github.com/g-truc/glm"));
		owner("Dear ImGui 1.93.0", "Omar Cornut and contributors — MIT", OBFSTR("https://github.com/ocornut/imgui"));
		owner("ImAnim (ofxImAnim)", "Danoli3 / ImAnim contributors — MIT", OBFSTR("https://github.com/Danoli3/ofxImAnim"));
		owner("fmt 12.2.0", "Victor Zverovich — MIT", OBFSTR("https://fmt.dev"));
		owner("nlohmann/json 3.12.0", "Niels Lohmann — MIT", OBFSTR("https://github.com/nlohmann/json"));
		owner("pugixml 1.16", "Arseny Kapoulkine — MIT", OBFSTR("https://pugixml.org"));
		owner("POCO 1.15.4", "POCO Project — Boost Software License", OBFSTR("https://pocoproject.org"));
		owner("FreeType 2.15.0", "The FreeType Project — FreeType Licence", OBFSTR("https://freetype.org"));
		owner("libpng 1.6.59", "PNG Reference Library authors — libpng licence", OBFSTR("http://www.libpng.org"));
		owner("zlib 1.3.2", "Jean-loup Gailly and Mark Adler — zlib licence", OBFSTR("https://zlib.net"));
		owner("Brotli 1.2.0", "Google — MIT", OBFSTR("https://github.com/google/brotli"));
		owner("Cairo 1.18.4", "freedesktop.org — LGPL-2.1 / MPL-1.1", OBFSTR("https://cairographics.org"));
		owner("Pixman 0.46.4", "freedesktop.org — MIT", OBFSTR("https://pixman.org"));
		owner("curl 8.23.0", "Daniel Stenberg and contributors — curl licence", OBFSTR("https://curl.se"));
		owner("OpenSSL 4.1.0", "OpenSSL Project — Apache-2.0", OBFSTR("https://www.openssl.org"));
		owner("FreeImage 3.19", "FreeImage Team — FreeImage Public Licence", OBFSTR("https://freeimage.sourceforge.io"));
		owner("RtAudio 6.0.1", "Gary P. Scavone — RtAudio Licence (MIT-like)", OBFSTR("https://www.music.mcgill.ca/~gary/rtaudio/"));
		owner("libtess2", "SGI / Mikko Mononen — SGI Free Software Licence", OBFSTR("https://github.com/memononen/libtess2"));
		owner("uriparser 1.0.2", "uriparser authors — BSD", OBFSTR("https://uriparser.github.io"));
		ImGui::EndTable();
	}
}

void ofApp::drawAdvancedSettings() {
	// Every setting there is, with what it is set to now. Read-only on purpose: the controls live on
	// the themed pages (and in settings.json, Settings > Data > Reload settings after editing it), so
	// there is one place each can change and nothing here can disagree with it. Secrets are never
	// shown: a token reads as set or not set, and API keys stay in the Keychain.
	auto para = [](const char * text) {
		ImGui::PushTextWrapPos(0);
		ImGui::TextUnformatted(text);
		ImGui::PopTextWrapPos();
	};
	struct Group {
		const char * title;
		std::vector<std::pair<std::string, std::string>> rows;
	};
	std::vector<Group> groups;
	auto add = [&](const char * group, const std::string & name, const std::string & value) {
		for (auto & g : groups)
			if (std::string(g.title) == group) {
				g.rows.push_back({ name, value });
				return;
			}
		groups.push_back({ group, { { name, value } } });
	};
	auto on = [](bool b) { return b ? "on" : "off"; };
	auto text = [](const std::string & s) { return s.empty() ? std::string("(not set)") : s; };
	auto moneyOr = [](double v) { return v < 0 ? std::string("not set") : "$" + ofToString(v, 2); };
	const Settings & s = settings;

	add("General", "refreshSeconds", ofToString(s.refreshSeconds) + " s");
	add("General", "animations", on(s.animations));
	add("General", "hints", on(s.hints));
	add("General", "showAdvanced", on(s.showAdvanced));
	add("General", "language", s.language.empty() ? "English" : s.language);
	add("General", "startTab", s.startTab);
	add("General", "welcomed (internal)", on(s.welcomed));
	add("Theme and glow", "theme", s.theme);
	add("Theme and glow", "glow", ofToString(s.glow, 2));
	add("Theme and glow", "glowPasses", ofToString(s.glowPasses));
	add("Theme and glow", "dataRain", on(s.dataRain));
	add("Theme and glow", "redAlert", on(s.redAlert));
	add("3D city", "intro (fly in at launch)", on(s.intro));
	add("3D city", "city (behind the panels)", on(s.city));
	add("3D city", "flyTabs", on(s.flyTabs));
	add("3D city", "cityAntialias", ofToString(s.cityAntialias));
	add("3D city", "cityWindow", text(s.cityWindow));
	add("Frames", "frameRate", s.frameRate == 0 ? "no limit" : ofToString(s.frameRate) + " a second");
	add("Frames", "vsync", on(s.vsync));
	add("Frames", "showFps", on(s.showFps));
	add("Frames", "interface size (uiScale)", ofToString(s.uiScale, 2) + "x");
	add("In the background", "unfocused", s.unfocused);
	add("In the background", "unfocusedRate", ofToString(s.unfocusedRate) + " a second");
	add("In the background", "stopHidden", on(s.stopHidden));
	add("In the background", "readHidden", on(s.readHidden));
	add("Window", "resizable", on(s.resizable));
	add("Window", "titleBar", on(s.titleBar));
	add("Window", "fullscreen", on(s.fullscreen));
	add("Window", "rememberWindow", on(s.rememberWindow));
	add("Window", "window position/size", ofToString(s.windowX) + "," + ofToString(s.windowY) + "  " + ofToString(s.windowW) + "x" + ofToString(s.windowH));
	add("Window", "windowAntialias", ofToString(s.windowAntialias));
	add("Window", "windowOpacity", ofToString(s.windowOpacity, 2));
	add("Window", "seeThrough", on(s.seeThrough));
	add("Window", "backgroundAlpha", ofToString(s.backgroundAlpha, 2));
	add("Window", "alwaysOnTop", on(s.alwaysOnTop));
	for (const auto & ps : s.providers) {
		add("Providers", ps.id + ": enabled", on(ps.enabled));
		add("Providers", ps.id + ": folder", ps.path.empty() ? "(default)" : ps.path);
		add("Providers", ps.id + ": billing", ps.billing);
		add("Providers", ps.id + ": plan price a month", moneyOr(ps.planPrice));
		add("Providers", ps.id + ": API budget", moneyOr(ps.apiBudget));
		add("Providers", ps.id + ": account id (xAI team)", text(ps.accountId));
		add("Providers", ps.id + ": API key", secrets::get(ps.id).empty() ? "not set" : "in the Keychain");
	}
	add("Alerts", "warnPercent", ofToString(s.warnPercent) + "%");
	add("Alerts", "toastLimits", on(s.toastLimits));
	add("Alerts", "toastProblems", on(s.toastProblems));
	add("Alerts", "staleHours", ofToString(s.staleHours) + " h");
	add("Export", "exportFolder", s.exportFolder.empty() ? "(Documents/BackBurnTokens)" : s.exportFolder);
	add("Export", "fileName pattern", s.fileName);
	add("Export", "exportTotals", on(s.exportTotals));
	add("Export", "revealAfterExport", on(s.revealAfterExport));
	add("Export", "keepExports per query", s.keepExports == 0 ? "keep all" : ofToString(s.keepExports));
	add("Export", "saved queries", ofToString(int(s.queries.size())));
	add("Privacy", "showTitles", on(s.showTitles));
	add("Privacy", "exportTitles", on(s.exportTitles));
	add("Privacy", "serveTitles (over the API)", on(s.serveTitles));
	add("Privacy", "readPrompts", on(s.readPrompts));
	add("Privacy", "exportPrompts", on(s.exportPrompts));
	add("Provider APIs", "apiMinutes (how often asked)", ofToString(s.apiMinutes) + " min");
	add("API and phones", "serveApi", on(s.serveApi));
	add("API and phones", "allowLan", on(s.allowLan));
	add("API and phones", "trustLocal", on(s.trustLocal));
	add("API and phones", "apiPort", ofToString(s.apiPort));
	add("API and phones", "apiToken", s.apiToken.empty() ? "not set" : "set (hidden)");
	add("API and phones", "remoteUrl (reading another computer)", text(s.remoteUrl));
	add("API and phones", "remoteToken", s.remoteToken.empty() ? "not set" : "set (hidden)");
	add("Data", "ledgerDays (history kept)", s.ledgerDays == 0 ? "forever" : ofToString(s.ledgerDays) + " days");
	add("Data", "settings file", Paths::settingsFile().string());

	para("Every setting Backburn has, and what it is set to. This page is a list, not controls: change "
	     "things on the other Settings pages, or edit the settings file by hand and use Settings > Data > "
	     "Reload settings. Keys and tokens are never shown here.");
	for (const auto & g : groups) {
		ImGui::Spacing();
		section(g.title);
		if (ImGui::BeginTable(g.title, 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp)) {
			ImGui::TableSetupColumn("Setting", ImGuiTableColumnFlags_WidthStretch, 1.0f);
			ImGui::TableSetupColumn("Now", ImGuiTableColumnFlags_WidthStretch, 1.6f);
			for (const auto & [name, value] : g.rows) {
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(name.c_str());
				ImGui::TableNextColumn();
				ImGui::TextColored(BRIGHT, "%s", value.c_str());
			}
			ImGui::EndTable();
		}
	}
	ImGui::Spacing();
	section("Keeping Settings simple");
	para("The plan for splitting this list: Simple keeps theme, providers (on/off, folder, key), the "
	     "warning level, export folder and the privacy basics. Everything else — frames, background "
	     "behaviour, window looks, city tuning, the phone API, ledger and this list — belongs in "
	     "Advanced, shown only when asked for. Until those controls move here, they stay on their "
	     "themed pages so nothing loses its control.");
}

void ofApp::drawHow() {
	// the page is the explanation: its hints show whatever the setting
	struct Always {
		bool was = hintsShown;
		Always() { hintsShown = true; }
		~Always() { hintsShown = was; }
	} always;
	auto heading = [](const char * text) {
		ImGui::Spacing();
		section(text);
	};
	auto para = [](const char * text) {
		ImGui::PushTextWrapPos(0);
		ImGui::TextUnformatted(text);
		ImGui::PopTextWrapPos();
	};

	heading("What this is");
	para("Backburn reads the files your AI coding tools already keep on this computer and shows what each has used, "
	     "per project, what each limit has left, and when it resets. Claude Code comes first. Nothing is sent anywhere: the "
	     "only network part is the optional API, which your own phone reads.");

	heading("Where the numbers come from");
	para("Claude Code");
	hint("  Limits: ~/.claude.json, cachedUsageUtilization - the 5-hour session and weekly limits, usage credits and spend, "
	     "as of the last time Claude Code checked them. Run /usage in Claude Code (or just use it) to bring them up to date.\n"
	     "  Tokens: ~/.claude/projects/*/*.jsonl - every reply's usage, with the folder it ran in.\n"
	     "  Cost: those tokens at Anthropic's API prices. On a Pro or Max plan you pay the plan, not per token; the figure "
	     "shows what the same work would cost on the API. Put the plan's price in Settings > Providers to compare.");
	para("Codex");
	hint("  ~/.codex/sessions/**/*.jsonl - token_count events (running totals, limits, credits, plan) and the session's folder.");
	para("Grok Build");
	hint("  ~/.grok/sessions/<folder>/*/updates.jsonl - each finished turn's tokens and the cost xAI reported, and errors: "
	     "a 402 means the usage balance ran out.");
	para("Muse");
	hint("  ~/.local/share/muse/sessions/**/snapshot-*.json - each session's token ledger and workspace. No price or limits.");
	hint("A tool kept somewhere else: Settings > Providers > Folder.");

	heading("Projects, queries and exports");
	para("Every reading is kept as rows: one model's use in one folder on one day. Click a project in a tool's tab for its "
	     "days and models, and export it. Queries asks any question of the rows - which tools, which projects, which models, "
	     "how many days, grouped by project, day, model or tool - shows the answer, and exports it as CSV, JSON or Markdown. "
	     "Save a query to run it again, or tick Export daily to have it written every day. Settings > Export sets the folder "
	     "and file names.");

	heading("Keeping history");
	para("Claude Code deletes transcripts older than 30 days unless cleanupPeriodDays says otherwise, and the other tools "
	     "prune too. Backburn's ledger keeps the rows, so projects and days stay after the logs are gone. To keep "
	     "Claude's transcripts as well, add to ~/.claude/settings.json:");
	address("cleanup", "", "\"cleanupPeriodDays\": 365");

	heading("Usage credits");
	para("Claude's usage credits (extra usage) cover you when you reach the plan's limits, billed per token, with a monthly "
	     "cap you set. The sidebar shows whether they're on and what they've used this month. Codex shows its credit balance.");

	heading("Your phone (iOS / Android)");
	para("The phone apps don't read files: they ask this computer. In Settings > API and phones, turn on Allow other devices, "
	     "then in the phone app enter this address and token:");
	address("lanurl", "Address", "http://" + (lanIp.empty() ? std::string("<this computer>") : lanIp) + ":" + ofToString(settings.apiPort));
	if (showToken) address("lantoken", "Token", settings.apiToken);
	else hint("(Settings > API and phones > Show token to see it.)");
	hint("Same Wi-Fi only; away from home use a private network such as Tailscale and its address instead.");

	heading("The API");
	para("GET /api/usage - the report as JSON (?provider=claude for one tool).\n"
	     "GET /api/query - a query's answer: ?name=<saved query>, or group=project|day|model|provider|row, provider=claude,codex, "
	     "project=..., exact=1, model=..., days=30, format=csv|json|md.\n"
	     "Send the token as \"Authorization: Bearer <token>\". GET /api/health needs none.");
	address("curl", "Try it", "curl -H \"Authorization: Bearer " + std::string(showToken ? settings.apiToken : "<token>") + "\" \"http://127.0.0.1:"
	                              + ofToString(settings.apiPort) + "/api/query?group=project&days=30&format=csv\"");
	hint("The report holds counts, costs, limits and folder paths - never prompts, code or replies.");

	heading("What it can't see");
	hint("  Claude limits are only as fresh as Claude Code's last check; Backburn doesn't sign in to your account.\n"
	     "  Use from claude.ai chats or the desktop app isn't in these logs; the weekly limit's \"used by\" line shows its share.\n"
	     "  An Anthropic API key's billing is in the Claude Console (Usage and Cost), not on this computer.\n"
	     "  Another computer's use shows only on that computer, or on a copy pointed at it.");
}

void ofApp::toast(const std::string & text, const ImVec4 & colour) {
	std::lock_guard<std::mutex> lock(toastMutex);
	toasts.push_back({ text, colour, -1.0, ++toastCount });
}

void ofApp::drawToasts() {
	std::lock_guard<std::mutex> lock(toastMutex);
	const double life = 5.0, fade = 0.4;
	double t0 = ImGui::GetTime();
	toasts.erase(std::remove_if(toasts.begin(), toasts.end(), [&](const Toast & t) { return t.start >= 0 && t0 - t.start > life; }),
	             toasts.end());
	float y = 92;
	for (Toast & t : toasts) {
		if (t.start < 0) t.start = t0;
		double age = t0 - t.start;
		float alpha = age > life - fade ? float((life - age) / fade) : 1.0f;
		float slide = tween(key("toast " + ofToString(t.id)), 0.0f, 0.35f, iam_ease_out_back, 1.0f);
		const float width = 380;
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - width - 24 + slide * (width + 40), y));
		ImGui::SetNextWindowSize(ImVec2(width, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.04f, 0.07f, 0.10f, 0.97f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(t.colour.x, t.colour.y, t.colour.z, 0.6f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
		std::string name = "##toast" + ofToString(t.id);
		ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings
		                                        | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav
		                                        | ImGuiWindowFlags_AlwaysAutoResize);
		ImVec2 a = ImGui::GetWindowPos(), size = ImGui::GetWindowSize();
		ImGui::GetWindowDrawList()->AddRectFilled(a, ImVec2(a.x + 4, a.y + size.y), ImGui::GetColorU32(t.colour));
		ImGui::PushTextWrapPos(width - 20);
		ImGui::TextUnformatted(t.text.c_str());
		ImGui::PopTextWrapPos();
		y += ImGui::GetWindowSize().y + 8;
		ImGui::End();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);
	}
}

void ofApp::exit() {
	saveWindow();
	api.reset();
	collector.reset();
}

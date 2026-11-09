// Fonts.h — the app's font families as one named struct, with candidate files per platform.
// macOS ships the faces the themes were designed in; Windows and Linux name the nearest system
// equivalents. Callers take the first candidate that exists (firstFontFile), and fall back to
// the toolkit's default font when none does — so a missing face degrades, never fails to start.

#pragma once

#include <filesystem>
#include <string>
#include <vector>

struct AppFonts {
	std::vector<std::string> mono;       // the data: numbers, tower faces, the rain
	std::vector<std::string> wide;       // neon headings/labels: wide and heavy
	std::vector<std::string> condensed;  // classic headings: condensed bold
	std::vector<std::string> label;      // classic labels
	std::vector<std::string> unicode;    // merged behind the others for glyphs they lack
};

// the one table; add a platform by adding its block, a family by adding its names
inline const AppFonts & appFonts() {
	static const AppFonts fonts = [] {
		AppFonts f;
#if defined(TARGET_WIN32)
		f.mono = { "C:/Windows/Fonts/consola.ttf", "C:/Windows/Fonts/cour.ttf" };
		f.wide = { "C:/Windows/Fonts/ariblk.ttf", "C:/Windows/Fonts/arialbd.ttf" };
		f.condensed = { "C:/Windows/Fonts/arialnb.ttf", "C:/Windows/Fonts/arialbd.ttf" };
		f.label = { "C:/Windows/Fonts/arialbd.ttf", "C:/Windows/Fonts/arial.ttf" };
		f.unicode = { "C:/Windows/Fonts/arialuni.ttf", "C:/Windows/Fonts/arial.ttf" };
#elif defined(TARGET_LINUX)
		f.mono = { "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf", "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf",
		           "/usr/share/fonts/TTF/DejaVuSansMono.ttf" };
		f.wide = { "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf",
		           "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf" };
		f.condensed = { "/usr/share/fonts/truetype/liberation/LiberationSansNarrow-Bold.ttf", "/usr/share/fonts/truetype/dejavu/DejaVuSansCondensed-Bold.ttf",
		                "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf" };
		f.label = { "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf", "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf" };
		f.unicode = { "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf" };
#else   // macOS and the other Apple platforms
		f.mono = { "/System/Library/Fonts/Monaco.ttf", "/System/Library/Fonts/Menlo.ttc" };
		f.wide = { "/System/Library/Fonts/Supplemental/Krungthep.ttf", "/System/Library/Fonts/Supplemental/Arial Black.ttf" };
		f.condensed = { "/System/Library/Fonts/Supplemental/DIN Condensed Bold.ttf", "/System/Library/Fonts/Supplemental/Arial Narrow Bold.ttf" };
		f.label = { "/System/Library/Fonts/Supplemental/DIN Alternate Bold.ttf", "/System/Library/Fonts/Supplemental/Arial Bold.ttf" };
		f.unicode = { "/System/Library/Fonts/Supplemental/Arial Unicode.ttf" };
#endif
		return f;
	}();
	return fonts;
}

// the first of these files that is on this machine, or "" when none is
inline std::string firstFontFile(const std::vector<std::string> & candidates) {
	for (const std::string & path : candidates)
		if (!path.empty() && std::filesystem::exists(path)) return path;
	return "";
}

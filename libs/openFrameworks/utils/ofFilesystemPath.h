#ifndef OF_FILESYSTEM_PATH_H
#define OF_FILESYSTEM_PATH_H

#include <filesystem>
#include <string>
#include <iostream>
#include <utility>  

namespace of::filesystem {

class path {
	
	// as per https://github.com/cplusplus/draft/releases/tag/n4917

	// general approach to return type:
	//  - use `path` and path &` to wrap std:: return values
	//  - use bool when bool (for self-documenting API)
	//  - use (const) auto (inheriting from std:: implementation) for others

private:
	std::filesystem::path path_; // simple composition
	
public:
	// MARK: construction
	path() = default;
	path(const std::filesystem::path& p) : path_(p) {}
	path(std::filesystem::path&& p) noexcept : path_(std::move(p)) {}
	path(const std::string& s) : path_(s) {}
	path(const char* s) : path_(s) {}
	path(const wchar_t* s) : path_(s) {}
	path(const path& other) = default;
	path& operator=(const path& other) = default;
	path(path&& other) noexcept = default;
	path& operator=(path&& other) noexcept = default;
	
	void clear() noexcept {
		path_.clear();
	}
	
	operator std::filesystem::path() const { return path_; }
	explicit operator const std::filesystem::path::value_type*() const { return path_.c_str(); }
	
	// MARK: string conversions
	
	auto wstring() const { return path_.wstring(); }
	auto generic_string() const { return path_.generic_string(); }
	auto generic_wstring() const { return path_.generic_wstring(); }
	auto generic_u8string() const { return path_.generic_u8string(); }
	auto generic_u16string() const { return path_.generic_u16string(); }
	auto generic_u32string() const { return path_.generic_u32string(); }
	auto string() const { return path_.string(); }
	auto native() const noexcept { return path_.native(); }
	auto u8string() const { return path_.u8string(); }
	auto u16string() const { return path_.u16string(); }
	auto u32string() const { return path_.u32string(); }
	const auto c_str() const noexcept { return path_.c_str(); }
	
	const std::filesystem::path& native_path() const { return path_; }
	
	static constexpr auto preferred_separator = std::filesystem::path::preferred_separator;
	
	bool empty() const noexcept { return path_.empty(); }
	bool is_absolute() const { return path_.is_absolute(); }
	bool is_relative() const { return path_.is_relative(); }
	
#if defined(TARGET_WIN32) // superfluous but usefull to facilitate testing on mac/linux
	
	// TODO better (ideal) impl this just copy-pasted for proof of concept
	mutable std::string cached_narrow_str_;
	const char* to_narrow_cstr() const {
		size_t size_needed = std::wcstombs(nullptr, wstring().c_str(), 0) + 1;
		if (size_needed == static_cast<size_t>(-1)) {
			throw std::runtime_error("Conversion error from wstring to string");
		}
		cached_narrow_str_.resize(size_needed);
		std::wcstombs(&cached_narrow_str_[0], wstring().c_str(), size_needed);
		return cached_narrow_str_.c_str();
	}
	
	operator std::wstring() const { return path_.wstring(); }
	operator const char*() const { return to_narrow_cstr(); } // should try catch on win
	operator std::string() const { return path_.string(); } // should try catch on win
	explicit operator const std::string() const { return path_.string(); } // should try catch on win
#else
	operator std::filesystem::path::string_type() const { return path_.string(); }
	explicit operator const std::filesystem::path::string_type() const { return path_.string(); }
#endif
	

	
	// MARK: std::filesystem forwards
	bool exists() const { return std::filesystem::exists(path_); }
	bool is_directory() const { return std::filesystem::is_directory(path_); }
	bool is_regular_file() const { return std::filesystem::is_regular_file(path_); }
	bool is_symlink() const { return std::filesystem::is_symlink(path_); }
	bool is_block_file() const { return std::filesystem::is_block_file(path_); }
	bool is_character_file() const { return std::filesystem::is_character_file(path_); }
	bool is_empty() const { return std::filesystem::is_empty(path_); }
	auto file_size() const { return std::filesystem::file_size(path_); }
	auto last_write_time() const { return std::filesystem::last_write_time(path_); }
	auto get_permissions() const { return std::filesystem::status(path_).permissions(); }
	
	// MARK: path type
	path lexically_normal() const {
		return path(path_.lexically_normal());
	}
	
	template <typename... Args>
	path lexically_relative(Args&&... args) const {
		return path(path_.lexically_relative(std::forward<Args>(args)...));
	}
	
	template <typename... Args>
	path lexically_proximate(Args&&... args) const {
		return path(path_.lexically_proximate(std::forward<Args>(args)...));
	}
	
	// MARK: comparison
	// TODO: C++20: spaceship simplification
	template <typename T> bool operator==(T&& other) const noexcept { return path_ == std::forward<T>(other); }
	template <typename T> bool operator!=(T&& other) const noexcept { return path_ != std::forward<T>(other); }
	template <typename T> bool operator<(T&& other) const noexcept { return path_ < std::forward<T>(other); }
	template <typename T> bool operator<=(T&& other) const noexcept { return path_ <= std::forward<T>(other); }
	template <typename T> bool operator>(T&& other) const noexcept { return path_ > std::forward<T>(other); }
	template <typename T> bool operator>=(T&& other) const noexcept { return path_ >= std::forward<T>(other); }
	
	bool operator!() const noexcept { return empty(); }
	
	template <typename... Args>
	auto compare(Args&&... args) const { return path_.compare(std::forward<Args>(args)...); }
	
	void swap(path & other) noexcept { path_.swap(other.path_); }
	
	
	// MARK: path transformation (return *this)
	path& replace_extension(const path & ext = std::filesystem::path()) {
		path_.replace_extension(ext);
		return *this;
	}
	
	path& replace_filename(const path & ext = std::filesystem::path()) {
		path_.replace_filename(ext);
		return *this;
	}
	
	template <typename T>
	path& assign(T&& p) noexcept {
		path_.assign(std::move(std::forward<T>(p)));
		return *this;
	}
	
	path& append(path&& p) noexcept {
		path_ /= std::move(p.path_);
		return *this;
	}
	
	path& operator/=(path&& p) noexcept {
		path_ /= std::move(p.path_);
		return *this;
	}
	
	template <typename T>
	path& operator+=(const T& rhs) {
		path_ += rhs;
		return *this;
	}
	
	template <typename T>
	path& concat(const T& rhs) {
		path_ += rhs;
		return *this;
	}
	
	path& make_preferred() {
		path_.make_preferred();
		return *this;
	}
	
	path& remove_filename() {
		path_.remove_filename();
		return *this;
	}
	
	// MARK: other operators
	template <typename LHS>
	const friend path operator/(const LHS& lhs, const path& rhs) {
		return path(lhs / rhs.path_);
	}
	
	template <typename RHS>
	const friend path operator/(const path& lhs, const RHS& rhs) {
		return path(lhs.path_ / rhs);
	}
	
	const friend path operator/(const path& lhs, const path& rhs) {
		return path(lhs.path_ / rhs.path_);
	}
	
	template <typename LHS>
	const friend path operator+(const LHS& lhs, const path& rhs) {
		return path(lhs + rhs.path_.string());
	}
	
	template <typename RHS>
	const friend path operator+(const path& lhs, const RHS& rhs) {
		return path(lhs.path_.string() + rhs);
	}
	
	// MARK: other sub paths
	path root_name() const { return path(path_.root_name()); }
	path root_directory() const { return path(path_.root_directory()); }
	path root_path() const { return path(path_.root_path()); }
	path relative_path() const { return path(path_.relative_path()); }
	path parent_path() const { return path(path_.parent_path()); }
	path filename() const { return path(path_.filename()); }
	path stem() const { return path(path_.stem()); }
	path extension() const { return path(path_.extension()); }
	
	// MARK: file info
	bool has_extension() const { return path_.has_extension(); }
	bool has_filename() const { return path_.has_filename(); }
	bool has_root_path() const { return path_.has_root_path(); }
	bool has_root_name() const { return path_.has_root_name(); }
	bool has_root_directory() const { return path_.has_root_directory(); }
	bool has_relative_path() const { return path_.has_relative_path(); }
	bool has_stem() const { return path_.has_stem(); }
	
	friend std::ostream& operator<<(std::ostream& os, const path& p) {
		return os << p.string();
	}
};
}
#endif // OF_FILESYSTEM_PATH_H

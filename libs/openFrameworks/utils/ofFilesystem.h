#include <filesystem>
#include <string>
#include <iostream>
#include <utility>  

namespace of {
namespace filesystem {

class path {
private:
	std::filesystem::path path_; // simple composition
	
#if defined(TARGET_WIN32)
	mutable std::string cached_narrow_str_;
	const char* to_narrow_cstr() const {
		std::mbstate_t state = std::mbstate_t();
		size_t size_needed = std::wcstombs(nullptr, wstring().c_str(), 0) + 1;
		if (size_needed == static_cast<size_t>(-1)) {
			throw std::runtime_error("Conversion error from wstring to string");
		}
		cached_narrow_str_.resize(size_needed);
		std::wcstombs(&cached_narrow_str_[0], wstring().c_str(), size_needed);
		return cached_narrow_str_.c_str();
	}
#endif

public:
	path() = default;
	path(const std::filesystem::path& p) : path_(p) {}
	path(std::filesystem::path&& p) noexcept : path_(std::move(p)) {}
	path(const std::string& s) : path_(s) {}
	path(const char* s) : path_(s) {}
	path(const path& other) = default;
	path& operator=(const path& other) = default;
	path(path&& other) noexcept = default;
	path& operator=(path&& other) noexcept = default;
	
	operator std::filesystem::path() const { return path_; }
	explicit operator const std::filesystem::path::value_type*() const { return path_.c_str(); }
	
#if defined(TARGET_WIN32)
	operator std::wstring() const { return path_.wstring(); }
	operator const char*() const { return to_narrow_cstr(); } // should try catch on win
	operator std::string() const { return path_.string(); } // should try catch on win
	explicit operator const std::string() const { return path_.string(); } // should try catch on win
#else
	operator std::filesystem::path::string_type() const { return path_.string(); }
	explicit operator const std::filesystem::path::string_type() const { return path_.string(); } 
#endif
	
	auto wstring() const { return path_.wstring(); }
	auto generic_string() const { return path_.generic_string(); }
	auto string() const { return path_.string(); }
	auto native() const { return path_.native(); }
	auto u8string() const { return path_.u8string(); }
	auto u16string() const { return path_.u16string(); }
	auto u32string() const { return path_.u32string(); }
	const auto c_str() const { return path_.c_str(); }

	const std::filesystem::path& native_path() const { return path_; }
	
	path replace_extension(const path & ext = std::filesystem::path()) { return path(path_.replace_extension(ext)); }
	
	static constexpr auto preferred_separator = std::filesystem::path::preferred_separator;
	
	bool empty() const noexcept { return path_.empty(); }
	bool is_absolute() const { return path_.is_absolute(); }
	bool is_relative() const { return path_.is_relative(); }
	bool exists() const { return std::filesystem::exists(path_); }
	
	bool is_directory() const { return std::filesystem::is_directory(path_); }
	bool is_regular_file() const { return std::filesystem::is_regular_file(path_); }
	bool is_symlink() const { return std::filesystem::is_symlink(path_); }
	bool is_block_file() const { return std::filesystem::is_block_file(path_); }
	bool is_character_file() const { return std::filesystem::is_character_file(path_); }
	bool is_empty() const { return std::filesystem::is_empty(path_); }
	std::uintmax_t file_size() const { return std::filesystem::file_size(path_); }
	std::filesystem::file_time_type last_write_time() const { return std::filesystem::last_write_time(path_); }
	
	path lexically_normal() const {
		return path(path_.lexically_normal());
	}

	template <typename... Args>
	path lexically_relative(Args&&... args) const {
		return path(path_.lexically_relative(std::forward<Args>(args)...));
	}
	
	template <typename... Args>
	path absolute(Args&&... args) const {
		return path(std::filesystem::absolute(path_, std::forward<Args>(args)...));
	}
	
	template <typename... Args>
	path canonical(Args&&... args) const {
		return path(std::filesystem::canonical(path_, std::forward<Args>(args)...));
	}
	
	std::filesystem::perms get_permissions() const { return status(path_).permissions(); }
	
	template <typename T> bool operator==(T&& other) const noexcept { return path_ == std::forward<T>(other); }
	template <typename T> bool operator!=(T&& other) const noexcept { return path_ != std::forward<T>(other); }
	template <typename T> bool operator<(T&& other) const noexcept { return path_ < std::forward<T>(other); }
	template <typename T> bool operator<=(T&& other) const noexcept { return path_ <= std::forward<T>(other); }
	template <typename T> bool operator>(T&& other) const noexcept { return path_ > std::forward<T>(other); }
	template <typename T> bool operator>=(T&& other) const noexcept { return path_ >= std::forward<T>(other); }

	path& operator/=(const path& p) {
		path_ /= p.path_;
		return *this;
	}
	
	template <typename T>
	of::filesystem::path& operator+=(const T& rhs) {
		path_ += rhs;
		return *this;
	}
	
	path& make_preferred() {
		path_.make_preferred();
		return *this;
	}
	
	template <typename LHS>
	const friend of::filesystem::path operator/(const LHS& lhs, const path& rhs) {
		return path(lhs / rhs.path_);
	}
	
	template <typename RHS>
	const friend of::filesystem::path operator/(const path& lhs, const RHS& rhs) {
		return path(lhs.path_ / rhs);
	}
	
	const friend of::filesystem::path operator/(const path& lhs, const path& rhs) {
		return path(lhs.path_ / rhs.path_);
	}
	
	template <typename LHS>
	const friend of::filesystem::path operator+(const LHS& lhs, const path& rhs) {
		return path(lhs + rhs.path_.string());
	}
	
	template <typename RHS>
	const friend of::filesystem::path operator+(const path& lhs, const RHS& rhs) {
		return path(lhs.path_.string() + rhs);
	}
	
	path remove_filename() { return path_.remove_filename(); }
	
	path root_path() const { return path(path_.root_path()); }
	path parent_path() const { return path(path_.parent_path()); }
	path filename() const { return path(path_.filename()); }
	path stem() const { return path(path_.stem()); }
	auto extension() const { return path(path_.extension()); }
	bool has_extension() const { return path_.has_extension(); }
	bool has_filename() const { return path_.has_filename(); }
	bool operator!() const noexcept { return empty(); }
	
	friend std::ostream& operator<<(std::ostream& os, const path& p) {
		return os << p.string();
	}
};

}  // namespace filesystem
}  // namespace of
